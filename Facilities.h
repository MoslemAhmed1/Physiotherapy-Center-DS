#pragma once
#include <random>
#include <iostream>

using namespace std;

// Helper Class for providing the random number generator for the scheduler
struct Facilities
{
	static int generateRandomNumber(int begin, int end)
	{
		random_device rd;      // Obtain a random seed
		mt19937 gen(rd());     // Initialize a random number generator

		uniform_int_distribution<int> dist(begin, end);
		int random_number = dist(gen); // Generate a random number

		return random_number;
	}
};


