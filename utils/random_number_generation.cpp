#include <random>

using namespace std;

const int L = 1;
const int R = 1e9;

default_random_engine gen;

uniform_int_distribution<int> distribution(L, R);

int num() { return distribution(gen); }
