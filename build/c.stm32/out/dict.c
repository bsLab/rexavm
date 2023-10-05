#include "config.h"
#include "dict.h"

dict_cell_t dictionary[256];

static ub2 lastfree   = NOTFOUND;
ub2 dictcollision=0;
static ub2 lastindex  = NOTFOUND;
static ub2 lastprev   = NOTFOUND;

// Naive hashing ;but  better than iteration over entire table each time
ub2 dicthash(char *name,ub2 length,ub2 tablesize) {
  ub2 sum=0,i=0;
  while (i<length) { sum+=name[i]; i++; };
  return sum % tablesize;
}

ub2 dicthash_jenkins(char *key, ub2 len, ub2 tablesize)
{
  ub4 hash, i;
  for(hash = i = 0; i < len; ++i)
  {
      hash += key[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return (ub2)(hash % tablesize);    
}

// Implementation of Murmur hash for 32-bit size_t.
ub2 dicthash_murmur(char* ptr, ub2 len, ub4 seed, ub2 tablesize)
{
  ub1 *buf=(ub1 *)ptr;
  const ub4 m = 0x5bd1e995;
  ub4 hash = seed ^ len;

  // Mix 4 bytes at a time into the hash.
  while (len >= 4)
  {
    ub4 k = *((ub4 *)buf);
    k *= m;
    k ^= k >> 24;
    k *= m;
    hash *= m;
    hash ^= k;
    buf += 4;
    len -= 4;
  }

  // Handle the last few bytes of the input array.
  switch (len)
  {
    case 3:
      hash ^= ((ub1)buf[2]) << 16;
    case 2:
      hash ^= ((ub1)buf[1]) << 8;
    case 1:
      hash ^= ((ub1)buf[0]);
      hash *= m;
  };

  // Do a few final mixes of the hash.
  hash ^= hash >> 13;
  hash *= m;
  hash ^= hash >> 15;
  return (ub2)(hash % tablesize);
}


/*
  Hashtable Dictionary with in-place collision handling
  and linked collision lists (next hops)
*/
// Search for a dictionray entry
ub2 dictlookup(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length) {
  ub2 i,h = dicthash(name,length,dictsize);
  if (length>DICTNAMELENGTH) length=DICTNAMELENGTH;
  lastindex = NOTFOUND;
  lastprev  = NOTFOUND;
  lastfree  = NOTFOUND;
  if (dictionary[h].name[0]==0 && !dictionary[h].next) {
    lastfree=h; return NOTFOUND;
  } else if (strncmp(dictionary[h].name,name,length)!=0 || 
             (length==DICTNAMELENGTH || dictionary[h].name[length]!=0)) {
    lastindex = h;
    if (dictionary[h].next==0) return NOTFOUND; // fast return
    // incremental search
    i=0;
    while (i<dictsize) {
      lastprev = h;
      h=(h+dictionary[h].next)%dictsize;i++;
      if (strncmp(dictionary[h].name,name,length)==0 && (length==DICTNAMELENGTH || dictionary[h].name[length]==0)) return h;
      if (dictionary[h].next==0) { lastindex=h; return NOTFOUND; }
    }
    return NOTFOUND;
  } else return h;
}
//  Add new entry to dictionary or update existing (nop)
ub2 dictadd(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length) {
  ub2 i,h = dicthash(name,length,dictsize);
  if (length>DICTNAMELENGTH) length=DICTNAMELENGTH;
  if (dictionary[h].name[0]==0) {
    // free slot
    // allocate slot
    strncpy(dictionary[h].name,name,length);
    return h;
  } else {
    // incremental search for free slot
    dictcollision++;
    h=dictlookup(dictionary,dictsize,name,length);
    if (h!=NOTFOUND) return h; // exists already
    if (lastfree!=NOTFOUND) {
      h = lastfree;
      strncpy(dictionary[h].name,name,length);
      return h;
    } else if (lastindex!=NOTFOUND) {
      h=lastindex;
      i=0;
      while (i<dictsize) {
        h=(h+1)%dictsize;i++;
        if (dictionary[h].name[0]==0) {
          dictionary[lastindex].next=h-lastindex;
          strncpy(dictionary[h].name,name,length);
          return h;
        }
      }
    }
    return NOTFOUND;
  }
}
// Remove a dictionary entry
ub2 dictrem(dict_cell_t *dictionary, ub2 dictsize, char *name, ub2 length) {
  ub2 i,h=dictlookup(dictionary,dictsize,name,length);
  if (h==NOTFOUND) return NOTFOUND;
  memset(dictionary[h].name, 0, DICTNAMELENGTH);
  if (dictionary[h].next && lastprev!=NOTFOUND) {
    dictionary[lastprev].next=h-lastprev;
  }
  return h;
}


// Remove all words in the address range [start,end](CS)
void dictclean(dict_cell_t *dictionary, ub2 dictsize, ub2 start, ub2 end) {
  ub2 i=0,length;
  while (i<dictsize) {
    if (dictionary[i].name[0]==0) continue;
    if (dictionary[i].addr>=start && dictionary[i].addr<=end) {
      if (dictionary[i].name[DICTNAMELENGTH-1]!=0) length=DICTNAMELENGTH;
      else length=strlen(dictionary[i].name);
      dictrem(dictionary,dictsize,dictionary[i].name,length);
    }
  }
}
void dictinit(dict_cell_t *dictionary, ub2 dictsize) {
  lastindex  = NOTFOUND;
  lastprev   = NOTFOUND;
  lastfree   = NOTFOUND;
  memset(dictionary,0,dictsize*sizeof(dict_cell_t));
}


