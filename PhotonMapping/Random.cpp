#include "Random.h"

std::mt19937 Random::gen = std::mt19937(std::random_device()());
std::uniform_int_distribution<int> Random::zeroTo255Distribution(0, 255);
std::uniform_real_distribution<float> Random::russianRouletteDistribution(0.f, 1.f);
std::uniform_real_distribution<float> Random::minus1To1Distribution(-1.f,1.f);

float Random::getValueFromMinus1To1()
{
	return minus1To1Distribution(gen);
}

float Random::getRussianRouletteValue()
{
	return russianRouletteDistribution(gen);
}

int Random::getValueFrom0To255()
{
	return zeroTo255Distribution(gen);
}

