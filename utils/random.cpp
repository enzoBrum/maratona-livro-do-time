const int minRand = 1;
const int maxRand = 1e12;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> distribution(minRand, maxRand);

int someRand = distribution(gen);

// shuffling a vector
random_device rd;
mt19937 gen(rd());
shuffle(all(vec), gen);
