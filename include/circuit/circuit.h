#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "gate.h"
#include <iostream>
#include <string>
#include <vector>

class Circuit {
private:
  std::vector<int> iwlabels;
  std::vector<int> owlabels;
  int wirecount;

public:
  std::vector<Gate> gates;

  Circuit(std::vector<Gate> &gates, int wirecount,
          std::vector<int> &inputWireLabels,
          std::vector<int> &outputWireLabels);
  
  Circuit();

  int getWireCount();

  std::vector<int> &getIWLabels();
  std::vector<int> &getOWLabels();

  /***
    Description:
      If multiple wires are coming out from a gate then they should be labeled
      differently. This is a requirement in this garbling scheme and this
      function basically takes a circuit and adds wire labels if multiple wires
      came out of gates.
    Assumes:
      The circuit's gates only have one output wire label
  ****/
  void addGateOutputLabels();

  // Converts the circuit to a string for display purposes
  std::string toString();
};

#endif
