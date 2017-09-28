#ifndef BINARY_H
#define BINARY_H 

#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <cassert>
#include <cstdint>

std::vector<int> decToBin(int dec, int binlen, bool direction);

std::vector<int> hexToBin(std::string hex, int binlen);

int64_t binToDec(std::vector<int> bin, bool direction);

std::string binToHex(std::vector<int> bin);

#endif /* ifndef BINARY_H */
