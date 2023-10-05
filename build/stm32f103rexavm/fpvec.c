#include "config.h"
#include "standard.h"
#include "vm.h"
#include "vmio.h"
#include "fpvec.h"

// src: IOS -(index+1) or CS offset, dst: CS offset
void vecload(sb2 src, sb2 srcoff, sb2 dst) {
  sb2 *psrc=NULL,*pdst=NULL;
  ub2 srclen=0,dstlen=0;
  // printf("vecload (%x) (%x)\n",src,dst);
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (src<0) { 
    // IOS Data
    src=-src-1; 
    if (src>=64 || ios[src].type!='D' || ios[src].size!=2) return vmraiseerror(EDATA); 
    psrc=DIOSDATADPTR(sb2,src)+srcoff;
    srclen=ios[src].cells;
  } else {
    // CS array
    psrc=(sb2*)&CS[src];
    srclen=*((sb2*)&CS[src-2]);
  }
  if (srclen<dstlen) return vmraiseerror(EDATA);
  memcpy(pdst,psrc,dstlen*2);
  // printf("vecload $%x[%d]+%d $%x[%d]\n",psrc,srclen,srcoff,pdst,dstlen);
};

// src: CS offset, dst: CS offset
void vecscale(sb2 src, sb2 dst, sb2 k) {
  ub2 index;
  sb2 ks=0;
  sb2 *psrc=NULL,*pdst=NULL, *pk=NULL;
  ub2 srclen=0,dstlen=0,klen=0;
  // printf("vecscale (%x) (%x) (%x)\n",src,dst,k);
  if (src<0) return vmraiseerror(EDATA); else { 
    // CS array
    psrc=(sb2*)&CS[src]; 
    srclen=*((sb2*)&CS[src-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (k<0) {ks=-k;} else if (k!=0) { 
    // CS array
    pk=(sb2*)&CS[k]; 
    klen=*((sb2*)&CS[k-2]);
  };
  if (srclen<dstlen || (klen && klen!=dstlen)) return vmraiseerror(EDATA);
  if (ks==0) {
    for(index=0;index<dstlen;index++) {
      ks=pk[index];
      if (ks<0) pdst[index]=psrc[index]/(-ks);
      else      pdst[index]=psrc[index]*ks;
    }; 
  } else {
    // constant down-scaling
    for(index=0;index<dstlen;index++) {
      pdst[index]=psrc[index]/ks;
    }; 
  }
  // printf("vecscale $%x[%d] $%x[%d] $%x[%d]\n",psrc,srclen,pdst,dstlen,pk,klen);
};


// src: CS offset (array), dst: CS offset (array), k: CS offset (array), |k|=|dst|
void vecfold(sb2 src, sb2 weights, sb2 dst, sb2 k) {
  ub2 index,j,wo;
  sb4 ks=0;
  sb4 t;
  sb2 *psrc=NULL,*pweights, *pdst=NULL, *pk=NULL;
  ub2 srclen=0,weightslen=0,dstlen=0,klen=0;
  // printf("vecfold (%x) (%x) (%x) (%x)\n",src,weights,dst,k);
  if (src<0) return vmraiseerror(EDATA); else { 
    // CS array
    psrc=(sb2*)&CS[src]; 
    srclen=*((sb2*)&CS[src-2]);
  };
  if (weights<0) return vmraiseerror(EDATA); else { 
    // CS array
    pweights=(sb2*)&CS[weights]; 
    weightslen=*((sb2*)&CS[weights-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (k<0) {ks=-k;}  else if (k!=0) { 
    // CS array
    pk=(sb2*)&CS[k]; 
    klen=*((sb2*)&CS[k-2]);
  };
  // printf("vecfold src $%x[%d] weights $%x[%d] dst $%x[%d] k $%x[%d]\n",psrc,srclen,pweights,weightslen,pdst,dstlen,pk,klen);
  if (srclen*dstlen!=weightslen || (klen && klen != dstlen)) return vmraiseerror(EDATA);
  wo=0;
  for(index=0;index<dstlen;index++) {
    t=0;
    for(j=0;j<srclen;j++) {
      t=t+(pweights[wo+j]*psrc[j]);
    }
    if (pk) {
      ks=(sb4)pk[index];
      if (ks<0) pdst[index]=(sb2)(t/(-ks));
      else      pdst[index]=(sb2)(t*ks);
    } else if (ks) {
      pdst[index]=(sb2)(t/ks);
    } else {
      pdst[index]=(sb2)t;
    };
    wo += srclen;
  };
};

// src: CS offset, dst: CS offset, k: here scalar for all dst array elements!
// if kwidth < 0 then weights is pooling layer paramater: kheight | 256: min, 512:max, 1024:mean
void vecconv(sb2 src, sb2 weights, sb2 dst, sb2 sk, sb2 swidth, sb2 kwidth, sb2 stride, sb2 pad) {
  sb2 x,y,i,j,k,l,q,r,n,index,wo,pooling=0;
  sb4 ks;
  sb4 t,v;
  sb2 *psrc=NULL,*pweights, *pdst=NULL;
  ub2 srclen=0,weightslen=0,dstlen=0,klen=0,kheight,sheight,dwidth,dheight;
  // printf("vecfold (%x) (%x) (%x) (%x)\n",src,weights,dst,k);
  // printf("weights=%d kwidth=%d\n",weights,kwidth);
  if (src<0) return vmraiseerror(EDATA); else { 
    // CS array
    psrc=(sb2*)&CS[src]; 
    srclen=*((sb2*)&CS[src-2]);
  };
  if (kwidth>0) {
    // convolution
    if (weights<0) return vmraiseerror(EDATA); else { 
      // CS array
      pweights    = (sb2*)&CS[weights]; 
      weightslen  = *((sb2*)&CS[weights-2]);
      kheight     = weightslen/(ub2)kwidth;
   }
  } else {
    // pooling
    pooling = weights>>8;
    kwidth  = -kwidth;
    kheight = weights&255;
    pweights = 0;
    weightslen = kwidth * kheight; 
  }
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  // if (srclen*dstlen!=weightslen || (klen && klen != dstlen)) return vmraiseerror(EDATA);

  sheight = srclen/swidth;
  // TODO TBC
  dwidth  = (swidth+pad*2-kwidth)/stride+(kwidth%2==0?0:1);
  dheight = (sheight+pad*2-kheight)/stride+(kheight%2==0?0:1);


  if ((kwidth*kheight)!=weightslen) return vmraiseerror(EDATA);
  if ((dwidth*dheight)>dstlen) return vmraiseerror(EDATA);
  
  x=-pad;
  // i and j are detsination column and row index
  if (!pooling) {
    // convolution operation
    for(i=0;i<dwidth;i++) {
      y=-pad;
      for(j=0;j<dheight;j++) {
        t=0;
        for (l=0;l<kheight;l++) {
          for(k=0;k<kwidth;k++) {
            if ((x+k)<0 || (y+l)<0 || (x+k)>=swidth || (y+l)>=sheight) continue;
            // source array index
            index=(x+k)+(y+l)*swidth;
            // printf("%d %d %d %d srcindex=%d\n",i,j,k,l,index);
            t=t+(pweights[l*kwidth+k]*psrc[index]);
          }
        }
        // destination array index
        index=(j*dwidth)+i;
        // printf("%d %d dstindex=%d\n",i,j,index);
        if (sk!=0) {
          ks=(sb4)sk;
          if (ks<0) pdst[index]=(sb2)(t/(-ks));
          else      pdst[index]=(sb2)(t*ks);
        } else {
          pdst[index]=(sb2)t;
        };
        y+=stride;
      }
      x+=stride;
    };
  } else {
    // pooling operation
    for(i=0;i<dwidth;i++) {
      y=-pad;
      for(j=0;j<dheight;j++) {
        n=0;
        for (l=0;l<kheight;l++) {
          for(k=0;k<kwidth;k++) {
            if ((x+k)<0 || (y+l)<0 || (x+k)>=swidth || (y+l)>=sheight) continue;
            // source array index
            index=(x+k)+(y+l)*swidth;
            // printf("%d %d %d %d srcindex=%d\n",i,j,k,l,index);
            v=psrc[index];
            if (n==0) {
              t=v;
            } else switch (pooling) {
              case OPMIN: t=v<t?v:t; break;
              case OPMAX: t=v>t?v:t; break;
              case OPAVG: 
              case OPSUM: t+=v; break;
            }
            n++;          
          }
        }
        // destination array index
        index=(j*dwidth)+i;
        switch (pooling) {
          case OPAVG: t/=n; break;
        }
        // printf("%d %d dstindex=%d\n",i,j,index);
        if (sk!=0) {
          ks=(sb4)sk;
          if (ks<0) pdst[index]=(sb2)(t/(-ks));
          else      pdst[index]=(sb2)(t*ks);
        } else {
          pdst[index]=(sb2)t;
        };
        y+=stride;
      }
      x+=stride;
    };  
  }
};

// src: CS offset, dst: CS offset
void vecmap(sb2 src, sb2 dst, sb2 func, sb2 k) {
  ub2 index;
  sb2 ks=0;
  sb2 t;
  sb2 *psrc=NULL, *pdst=NULL, *pk=NULL;
  void *pfunc=NULL;
  sb2  findex;
  ub2 srclen=0,dstlen=0,klen=0;
  
  // printf("vecmap (%x) (%x) (%x:%d) (%x)\n",src,dst,func,func,k);
  if (src<0) return vmraiseerror(EDATA); else { 
    // CS array
    psrc=(sb2*)&CS[src]; 
    srclen=*((sb2*)&CS[src-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (func < 0) {
    // IOS func index
    findex=-func-1;
    if (ios[findex].type!='F') return vmraiseerror(EDATA);
    pfunc=ios[findex].callback;
  } else {
    // Forth word CS
    // Not supported yet
    return vmraiseerror(EDATA);
  }
  if (k<0) { ks=-k; } else if (k!=0) { 
    // CS array
    pk=(sb2*)&CS[k]; 
    klen=*((sb2*)&CS[k-2]);
  };
  if (pk) {
    for(index=0;index<dstlen;index++) {
      ks=pk[index];
      t=FIOSCALL2(findex,psrc[index]);
      if (ks<0) pdst[index]=(sb2)(t/(-ks));
      else      pdst[index]=(sb2)(t*ks);
    };
   } else if (ks) {
    for(index=0;index<dstlen;index++) {
      t=FIOSCALL2(findex,psrc[index]);
      pdst[index]=(sb2)(t/ks);
    };   
   } else {
    for(index=0;index<dstlen;index++) {
      pdst[index]=FIOSCALL2(findex,psrc[index]);
    };
  } 
  // printf("vecmap src $%x[%d] dst $%x[%d] func $%x k $%x[%d]\n",psrc,srclen,pdst,dstlen,pfunc,pk,klen);
};

// src: CS offset, dst: CS offset
void vecadd(sb2 op1, sb2 op2, sb2 dst, sb2 k) {
  ub2 index;
  sb4 ks=0;
  sb4 t;
  sb2 *pop1=NULL,*pop2=NULL,*pdst=NULL, *pk=NULL;
  ub2 op1len=0,op2len=0,dstlen=0,klen=0;
  // printf("vecadd (%x) (%x) (%x) (%x)\n",op1,op2,dst,k);
  if (op1<0) return vmraiseerror(EDATA); else { 
    // CS array
    pop1=(sb2*)&CS[op1]; 
    op1len=*((sb2*)&CS[op1-2]);
  };
  if (op2<0) return vmraiseerror(EDATA); else { 
    // CS array
    pop2=(sb2*)&CS[op2]; 
    op2len=*((sb2*)&CS[op2-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (k<0) { ks=-k; } else if (k!=0) { 
    // CS array
    pk=(sb2*)&CS[k]; 
    klen=*((sb2*)&CS[k-2]);
  };
  if (op1len!=op2len) return vmraiseerror(EDATA);
  if (dstlen>op1len) dstlen=op1len;
  if (k!=0 && klen<dstlen) return vmraiseerror(EDATA);
  if (pk) {
    for(index=0;index<dstlen;index++) {
      ks=(sb4)pk[index];
      t=(sb4)pop1[index]+(sb4)pop2[index];
      if (ks<0) pdst[index]=(sb2)(t/(-ks));
      else      pdst[index]=(sb2)(t*ks);
    };
   } else if (ks) {
    for(index=0;index<dstlen;index++) {
      t=(sb4)pop1[index]+(sb4)pop2[index];
      pdst[index]=(sb2)(t/ks);
    };   
   } else {
    for(index=0;index<dstlen;index++) {
      pdst[index]=pop1[index]+pop2[index];
    };
  } 
  // printf("vecadd $%x[%d] $%x[%d] $%x[%d] $%x[%d]\n",pop1,op1len,pop2,op2len,pdst,dstlen,pk,klen);
};

// src: CS offset, dst: CS offset
void vecmul(sb2 op1, sb2 op2, sb2 dst, sb2 k) {
  ub2 index;
  sb4 ks=0;
  sb4 t;
  sb2 *pop1=NULL,*pop2=NULL,*pdst=NULL, *pk=NULL;
  ub2 op1len=0,op2len=0,dstlen=0,klen=0;
  // printf("vecmul (%x) (%x) (%x) (%x)\n",op1,op2,dst,k);
  if (op1<0) return vmraiseerror(EDATA); else { 
    // CS array
    pop1=(sb2*)&CS[op1]; 
    op1len=*((sb2*)&CS[op1-2]);
  };
  if (op2<0) return vmraiseerror(EDATA); else { 
    // CS array
    pop2=(sb2*)&CS[op2]; 
    op2len=*((sb2*)&CS[op2-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (k<0) { ks=-k; /* applied to all elements, only reduction*/ } else if (k!=0) { 
    // CS array
    pk=(sb2*)&CS[k]; 
    klen=*((sb2*)&CS[k-2]);
  };
  if (op1len!=op2len) return vmraiseerror(EDATA);
  if (dstlen>op1len) dstlen=op1len;
  if (k!= 0 && klen<dstlen) return vmraiseerror(EDATA);
  if (pk) {
    for(index=0;index<dstlen;index++) {
      ks=(sb4)pk[index];
      t=(sb4)pop1[index]*(sb4)pop2[index];
      if (ks<0) pdst[index]=(sb2)(t/(-ks));
      else      pdst[index]=(sb2)(t*ks);
    };
  } else if (ks) {
    for(index=0;index<dstlen;index++) {
      t=(sb4)pop1[index]*(sb4)pop2[index];
      pdst[index]=(sb2)(t/ks);
    };   
  } else {
    for(index=0;index<dstlen;index++) {
      pdst[index]=pop1[index]*pop2[index];
    };
  } 
  // printf("vecmul $%x[%d] $%x[%d] $%x[%d] $%x[%d]\n",pop1,op1len,pop2,op2len,pdst,dstlen,pk,klen);
};

// ( vec vecoff len op -- index min )
// op: OPMIN, OPMAX, OPAVG, OPSUM
sb4 vecreduce  (sb2 vec, sb2 vecoff, sb2 len, sb2 op) {
  ub2 index;
  sb2 *pvec=NULL;
  sb2 accu=0;
  sb2 veclen;
  sb4 accu4=0;
  
  if (vec<0) { 
    // IOS Data
    vec=-vec-1; 
    if (vec>=64 || ios[vec].type!='D' || ios[vec].size!=2) { vmraiseerror(EDATA); return 0; } 
    pvec=DIOSDATADPTR(sb2,vec)+vecoff;
    veclen=(sb2)ios[vec].cells;
  } else { 
    // CS array
    pvec=(sb2*)&CS[vec]; 
    veclen=*((sb2*)&CS[vec-2]);
  };
  if (pvec && len<=veclen) {
    index=(ub2)vecoff;
    if (len==0) len=veclen;
    switch (op) {
      case OPMIN: 
        accu=pvec[index++];
        for(;index<len;index++) {
          accu=pvec[index]<accu?pvec[index]:accu;
        }
        break;
      case OPMAX:  
        accu=pvec[index++];
        for(;index<len;index++) {
          accu=pvec[index]>accu?pvec[index]:accu;
        }
        break;
      case OPAVG:
      case OPSUM: 
        accu4=0;
        for(;index<len;index++) {
          accu4+=(sb4)pvec[index];
        }
        if (op==OPAVG) accu4/=len;
        return accu4;
        break;
    }
  } else return 0;
  // Encode two return values (index, value)
  return (sb4)(((ub2)index<<16)+(ub2)accu);
};

// src: CS offset, dst: CS offset
void vecreshape(sb2 src,sb2 dst, sb2 k) {
  ub2 index,index2;
  sb2 *psrc=NULL,*pdst=NULL;
  ub2 srclen=0,dstlen=0,newlen=0;
  // printf("vecadd (%x) (%x) (%x) (%x)\n",op1,op2,dst,k);
  if (src<0) return vmraiseerror(EDATA); else { 
    // CS array
    psrc=(sb2*)&CS[src]; 
    srclen=*((sb2*)&CS[src-2]);
  };
  if (dst<0) return vmraiseerror(EDATA); else { 
    // CS array
    pdst=(sb2*)&CS[dst]; 
    dstlen=*((sb2*)&CS[dst-2]);
  };
  if (k<0) {
    // shrink
    k=-k;
    newlen=srclen/k;
    if (newlen>dstlen) return vmraiseerror(EDATA);
    for(index=0;index<newlen;index++) {
      pdst[index]=psrc[index/k];
    }
  } else if (k>0) {
    // expand
    newlen=srclen*k;
    if (newlen>dstlen) return vmraiseerror(EDATA);
    for(index=0;index<newlen;index++) {
      for(index2=0;index2<k;index2++)
        pdst[index2]=psrc[index*k];
    }
  }
};
void vecprint  (sb2 vec) {
  ub2 index;
  sb2 *pvec=NULL;
  sb2 veclen=0;
  if (vec<0) { 
    // IOS Data
    vec=-vec-1; 
    if (vec>=64 || ios[vec].type!='D' || ios[vec].size!=2) return vmraiseerror(EDATA); 
    pvec=DIOSDATADPTR(sb2,vec);
    veclen=(sb2)ios[vec].cells;
  } else { 
    // CS array
    pvec=(sb2*)&CS[vec]; 
    veclen=*((sb2*)&CS[vec-2]);
  };
  if (pvec) {
    for(index=0;index<veclen;index++) {
      { char buf[100]; sprintf(buf," %d ",(sb4)pvec[index]); serialPrint(buf); };
    }
  }
};

