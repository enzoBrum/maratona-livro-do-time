using namespace std;
 
 
bool valid(ull time, ull goal,vull& machines) {
  ull sum = 0;
  for (auto& m : machines) 
    sum += time/m;
 
  return sum >= goal;
}
 
 
int main() {
  fast_io();
 
  ull n, t;
  cin >> n >> t;
 
  vull machines;
  while (n--) {
    ull tmp;
    cin >> tmp;
    machines.push_back(tmp);
  }
 
  
  ull boundary = t*(*max_element(machines.begin(), machines.end())) + 1;
  DEBUG(boundary);
  ull k = 0;
  for (ull b = boundary/2; b >= 1; b /= 2) {
    DEBUG(valid(k+b, t,machines));
    while (!valid(k+b, t,machines)) k+=b;
  }
 
  cout << k+1 << '\n';
 
 
}
