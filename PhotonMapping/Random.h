#pragma once
#include <random>
#include <memory>

class Random
{
public:
	static float getRussianRouletteValue();
	static float getValueFromMinus1To1();
	static int getValueFrom0To255();

private:
	static std::mt19937 gen;
	static std::uniform_real_distribution<float> russianRouletteDistribution;
	static std::uniform_int_distribution<int> zeroTo255Distribution;
	static std::uniform_real_distribution<float> minus1To1Distribution;
};

