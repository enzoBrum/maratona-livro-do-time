const long long minRand = 1;
const long long maxRand = 100;

default_random_engine generator;
uniform_int_distribution<long long> distribution(minRand, maxRand);

long long someRand = distribution(generator);
