  // ub4 vmMilli() { .. }
  long long milli;
  struct timeval tv;
  gettimeofday(&tv,NULL);
  milli=(((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
  return (ub4)milli;
