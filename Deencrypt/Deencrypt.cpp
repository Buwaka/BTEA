// Deencrypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>


#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

/*BTEA will encode or decode n words as a single block where n > 1

v is the n word data vector
k is the 4 word key
n is negative for decoding
if n is zero result is 1 and no coding or decoding takes place, otherwise the result is zero
assumes 32 bit 'long' and same endian coding and decoding*/



void btea(uint32_t *v, int n, uint32_t const key[4]) {
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	if (n > 1) {          /* Coding Part */
		rounds = 6 + 52 / n;
		sum = 0;
		z = v[n - 1];
		do {
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++) {
				y = v[p + 1];
				z = v[p] += MX;
			}
			y = v[0];
			z = v[n - 1] += MX;
		} while (--rounds);
	}
	else if (n < -1) {  /* Decoding Part */
		n = -n;
		rounds = 6 + 52 / n;
		sum = rounds * DELTA;
		y = v[0];
		do {
			e = (sum >> 2) & 3;
			for (p = n - 1; p > 0; p--) {
				z = v[p - 1];
				y = v[p] -= MX;
			}
			z = v[n - 1];
			y = v[0] -= MX;
			sum -= DELTA;
		} while (--rounds);
	}
}


void WriteFile(uint32_t* data, int length, std::string path)
{
	std::ofstream writer;
	writer.open(path);
	writer.write((char*)data, length);
	writer.close();
}

int ReadFile(uint32_t** data, std::string path)
{
	//read input
	std::ifstream reader;
	reader.open(path, std::ifstream::in);

	//get length
	reader.seekg(0, reader.end);
	int length = reader.tellg();
	reader.seekg(0, reader.beg);

	//make buffer
	//belangrijk om array te initializen, anders krijg je random garbage in uw data, wat decryptie onmogelijk maakt
	*data = new uint32_t[length]{};

	//read input
	reader.read((char*)*data, length);
	reader.close();

	return length;
}


int main(int argc, char **argv)
{
	//shift 1 forward since executable itself is an argument
	if (argc != 5)
	{
		std::cout << "Not enough arguments" << std::endl;
		return 0;
	}

	std::cout << "Action: " << argv[1] << std::endl;
	std::cout << "Input: " << argv[2] << std::endl;
	std::cout << "Password: " << argv[3] << std::endl;
	std::cout << "Output: " << argv[4] << std::endl;

	char encrypt[] = "Encrypt";
	bool Action = strncmp(argv[1], encrypt, 7) == 0;
	std::string input = argv[2];
	std::string key = argv[3];
	std::string output = argv[4];


	if(key.length() > 16)
	{
		std::cout << "Password is not 4 characters" << std::endl;
		return 0;
	}

	//belangrijk om array te initializen
	uint32_t keyData[4]{};
	memcpy(keyData, key.data(), 4);

	uint32_t* data = nullptr;
	int length = ReadFile(&data, input);

	if (Action)
	{
		//encrypt
		btea(data, length, keyData);
		length = length * sizeof(uint32_t);
	}
	else
	{
		//decrypt
		length = length / sizeof(uint32_t);
		btea(data, -length, keyData);
	}


	WriteFile(data, length, output);

	delete[] data;

	return 1;
}
