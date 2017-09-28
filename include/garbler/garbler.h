#ifndef GARBLER_H
#define GARBLER_H

#include "circuit.h"
#include "gate.h"
#include "random.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

class Garbler {

private:
  std::vector<int> ranBits;
  std::vector<int> circuitInput;
  std::vector<int> decodebits;
  void garbleGate(Gate &gate);
  void garbleGates(std::vector<Gate> &gates, int start, int count);

public:
  // input is assumed to be ordered correctly to the iwlabels
  Garbler(const std::vector<int> &input);
  void garbleCircuit(Circuit &circuit, int maxthreads);
  std::vector<int> decodeOutput(std::vector<int> output);
  std::vector<int> getInput();
};

#endif
