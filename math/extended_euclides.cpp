// ax + by = gcd(a, b)
// What is the value of x and y?
int extEuclid(int a, int b, int &x, int &y) { // pass x and y by ref
  int xx = y = 0;
  int yy = x = 1;
  while (b) { // repeats until b == 0
    int q = a / b;
    int t = b;
    b = a % b;
    a = t;
    t = xx;
    xx = x - q * xx;
    x = t;
    t = yy;
    yy = y - q * yy;
    y = t;
  }
  return a; // returns gcd(a, b)
}
