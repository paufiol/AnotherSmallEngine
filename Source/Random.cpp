#include "Random.h"
#include <random>
#include "Dependencies/PCG/pcg_random.hpp"
#include "Dependencies/PCG/pcg_extras.hpp"

Random::Random(uint32 num) : num(num)
{
}
uint32 Random::GenerateRandomIntRange(int first, int last)
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);

	std::uniform_int_distribution<uint32> uniform_dist1(first, last);
	int num = num = uniform_dist1(rng);

	return num;;
}

uint32 Random::GenerateRandomInt()
{
	pcg_extras::seed_seq_from<std::random_device> seed;
	pcg32 rng(seed);

	std::uniform_int_distribution<uint32> uniform_dist1(0, 429496);
	int num = num = uniform_dist1(rng);

	return num;
}