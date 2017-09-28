#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "circuit.h"
#include <cassert>
#include <iostream>
#include <vector>

class Evaluator {

public:
  Evaluator();

  std::vector<int> evaluate(Circuit &C, std::vector<int> input);
};

#endif
