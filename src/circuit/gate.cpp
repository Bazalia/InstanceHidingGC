#include "gate.h"

Gate::Gate(operation functionality, int lilabel, int rilabel,
           const std::vector<int> &outputWires)
{
    // Two input wires
    liw = lilabel;
    riw = rilabel;

    // Output wires
    outWires = outputWires;

    // Table
    switch (functionality)
    {
    case AND:
        tables = std::vector<TTable>(outputWires.size(), TTable(8));
        break;

    case OR:
        tables = std::vector<TTable>(outputWires.size(), TTable(14));
        break;

    case NAND:
        tables = std::vector<TTable>(outputWires.size(), TTable(7));
        break;

    case NOR:
        tables = std::vector<TTable>(outputWires.size(), TTable(1));
        break;

    case XOR:
        tables = std::vector<TTable>(outputWires.size(), TTable(6));
        break;
    }
}

Gate::Gate(char ttable, int lilabel, int rilabel,
           const std::vector<int> &outputWires)
{
    // Two input wires
    liw = lilabel;
    riw = rilabel;

    // Output wires
    outWires = outputWires;

    tables = std::vector<TTable>(outputWires.size(), TTable(ttable));
}

// Returns the Left Input Wire label
int Gate::getLIW() { return liw; }

// Returns the Right Input Wire label
int Gate::getRIW() { return riw; }

// Returns the output wire labels
std::vector<int> Gate::getOutLabels() { return outWires; }

// Returns the truth table value from left input and right input
std::vector<int> Gate::evaluate(int li, int ri)
{
    assert(tables.size() == outWires.size());
    std::vector<int> results(tables.size());
    for (int i = 0; i < tables.size(); i++)
    {
        results[i] = (int)tables[i].lookup((char)li, (char)ri);
    }
    return results;
}

// Sets the left input wire label of this gate
void Gate::setLIW(int nliw) { liw = nliw; }

// Sets the right input wire label of this gate
void Gate::setRIW(int nriw) { riw = nriw; }

void Gate::garble(char r0, char r1, std::vector<char> r2s)
{
    assert(tables.size() == outWires.size());
    assert(r2s.size() == tables.size());
    assert(r0 == 0 || r0 == 1);
    assert(r1 == 0 || r1 == 1);

    for (int i = 0; i < tables.size(); i++)
    {
        tables[i].garble(r0, r1, r2s[i]);
    }
}

// Adds a new output wire label to the gate
void Gate::addOutLabel(int owl)
{
    outWires.push_back(owl);
    tables.push_back(TTable(tables[0].getTable()));
    assert(tables.size() == outWires.size());
}

std::string Gate::toString()
{
    // Add input labels
    std::string res =
        "liw = " + std::to_string(liw) + ", riw = " + std::to_string(riw);

    // Add output labels
    res += "\now = ";
    for (int i = 0; i < outWires.size(); i++)
    {
        res += std::to_string(outWires[i]) + " ";
    }
    res += "\n";

    // Add truth table
    res += "ttables = \n";
    for (int i = 0; i < tables.size(); i++)
    {
        res += "\t" + tables[i].toString() + "\n";
    }

    return res;
}
