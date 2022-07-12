const double PI=3.1415926535897932384650288;

double sin(double x){
  double sign=1;
  if (x<0){
    sign=-1.0;
    x=-x;
  }
  if (x>360) x -= (x/360)*360;
  x*=PI/180.0;
  double res=0;
  double term=x;
  int k=1;
  while (res+term!=res){
    res+=term;
    k+=2;
    term*=-x*x/k/(k-1);
  }

  return sign*res;
}

double cos(double x){
  if (x<0) x=-x;
  if (x>360) x -= (x/360)*360;
  x*=PI/180.0;
  double res=0;
  double term=1;
  int k=0;
  while (res+term!=res){
    res+=term;
    k+=2;
    term*=-x*x/k/(k-1);
  }  
  return res;
}
u16 sqrt(int x){
    if(x == 0 || x == 1){
        return x;
    }

    u32 start = 1, end = 4294967295;

    while (start + 1 < end){
        int mid = start + (end - start) / 2;
        if(mid == x/mid){
            return mid;
        } else if(mid > x/mid){
            end = mid;
        } else{
            start = mid;
        }
    }      
    return start;
}