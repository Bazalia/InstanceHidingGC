#ifndef CIRCUIT_FILE_READER_H
#define CIRCUIT_FILE_READER_H

// #define NDEBUG

#include "circuit.h"
#include "gate.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

Circuit readTGCircuit(std::string filename);
Circuit readFPCircuit(std::string filename);
Circuit readBRISCircuit(std::string filename);

#endif
