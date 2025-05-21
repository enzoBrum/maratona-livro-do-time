using namespace std;

/*
 *
 * vull --> vector<unsigned long long>
 *
*A factory has n machines which can be used to make products. Your goal is to
make a total of t products. *For each machine, you know the number of seconds it
needs to make a single product. The machines can work simultaneously, and you
can freely decide their schedule. *What is the shortest time needed to make t
products?
*
*The first input line has two integers n and t: the number of machines and
products. The next line has n integers k_1,k_2,\dots,k_n: the time needed to
make a product using each machine.
*/

bool valid(ull time, ull goal, vull &machines) {
  ull sum = 0;
  for (auto &m : machines)
    sum += time / m;

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

  ull boundary = t * (*max_element(machines.begin(), machines.end())) + 1;
  DEBUG(boundary);
  ull k = 0;
  for (ull b = boundary / 2; b >= 1; b /= 2) {
    DEBUG(valid(k + b, t, machines));
    while (!valid(k + b, t, machines))
      k += b;
  }

  cout << k + 1 << '\n';
}
