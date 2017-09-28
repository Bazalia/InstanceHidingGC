#ifndef GATE_H
#define GATE_H

#include "ttable.h"
#include <string>
#include <vector>

enum operation
{
    AND,
    OR,
    NAND,
    NOR,
    XOR
};

class Gate
{
  private:
    // Only allows two input wire gates so left input and right input wire
    int liw, riw;

    // There can be arbitrary fan out
    std::vector<int> outWires;

    std::vector<TTable> tables;

  public:
    // Creates a Gate with given operation and input and output wire labels
    Gate(operation functionality, int liw, int riw,
         const std::vector<int> &outputWires);

    // Creates a Gate from given 4 row truth table
    Gate(char ttable, int liw, int riw, const std::vector<int> &outputWires);

    // Returns the Left Input Wire label
    int getLIW();

    // Returns the Right Input Wire label
    int getRIW();

    // Sets the left input wire label of this gate
    void setLIW(int nliw);

    // Sets the right input wire label of this gate
    void setRIW(int nriw);

    /*
    Description:
        Garble all the underlying truth tables of this gate using the random bits
        r0, r1 and the r2 for each table. Garbling mechanism is introduced in 
        https://dl.acm.org/citation.cfm?id=3011100 "An efficient instance 
        hiding scheme"
    */
    void garble(char r0, char r1, std::vector<char> r2s);

    // Returns the output wire labels
    std::vector<int> getOutLabels();

    /*
    Description:
        Adds a new output wire label to the gate. Additionaly adds an extra table
        to accommodate for the new output label, this table is a copy of the first
        table (i.e. the table of the first output wire)
    */
    void addOutLabel(int owl);

    // Returns the truth table values from left input and right input
    // li and ri are either 0 or 1
    std::vector<int> evaluate(int li, int ri);

    // Displays a gates input and output labels in addition to its truth table
    std::string toString();
};

#endif
