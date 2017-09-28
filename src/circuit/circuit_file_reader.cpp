#include "circuit_file_reader.h"

// template <typename Out>
// void split(const std::string &s, char delim, Out result)
// {
//     std::stringstream ss;
//     ss.str(s);
//     std::string item;
//     while (std::getline(ss, item, delim))
//     {
//         *(result++) = item;
//     }
// }

// // splits a string by some delimiter
// std::vector<std::string> split(const std::string &s, char delim)
// {
//     std::vector<std::string> elems;
//     split(s, delim, std::back_inserter(elems));
//     return elems;
// }

// Reads circuits created in University of Bristol UK
// http://www.cs.bris.ac.uk/Research/CryptographySecurity/MPC/
// Requires given file to be of correct format indicated in link above
Circuit readBRISCircuit(std::string filename)
{
    // Open file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::exit(EXIT_FAILURE);
    }

    // First line has number of gates followed by number of wires
    int numGates, numWires = 0;
    file >> numGates;
    file >> numWires;

    assert(numGates != 0 && numWires != 0);

    // Second line has number of first input bits, number of second input
    // bits and number of output bits
    int inpSize0, inpSize1, outSize;
    file >> inpSize0;
    file >> inpSize1;
    file >> outSize;

    assert(outSize != 0 && (inpSize0 != 0 || inpSize1 != 0));

    // To remove 'not' gates that percede other gates we use an int array such
    // that array[i] = x means a 'not' gate with output wire label i and has an
    // input wire label x (x = -1 if there is no 'not' gate that outputs to wire
    // label i). Since BRIS circuits are in topological order when we see an AND
    // or a XOR gate we can query this array to check if it is perceded by a 'not'
    // gate in which we have to alter the current gate's table to merge the two
    // gates.
    std::vector<int> notGates(numWires, -1);

    // Gates that account for the circuit
    std::vector<Gate> gates;

    // Read the gates. Each gate has the following format:
    // - Number input wires
    // - Number output wires
    // - List of input wires
    // - List of output wires
    // - Gate operation (XOR, AND or INV).
    // So for example 2 1 3 4 5 XOR corresponds to w5 = XOR(w3, w4)
    int numInpWires;
    while (file >> numInpWires)
    {
        // If gates can have more than 2 input wires then modification need to be
        // made
        assert(numInpWires == 2 || numInpWires == 1);

        int numOutWires;
        file >> numOutWires;
        assert(numOutWires >= 1);
        std::vector<int> outWires(numOutWires);

        // If it's a binary gate (XOR or AND only)
        if (numInpWires == 2)
        {
            int liw, riw;
            file >> liw;
            file >> riw;
            // Make sure liw and riw are valid input wire labels
            assert(liw >= 0 && liw < (numWires - outSize));
            assert(riw >= 0 && riw < (numWires - outSize));

            for (int i = 0; i < numOutWires; i++)
            {
                file >> outWires[i];
                // Make sure outWires are valid wire labels
                assert(outWires[i] < numWires && outWires[i] >= (inpSize0 + inpSize1));
            }

            std::string type;
            std::getline(file, type);
            if (type == " AND")
            {
                if (notGates[liw] == -1 && notGates[riw] == -1)
                {
                    // If this AND gate is not preceded directly by any 'not'
                    gates.push_back(Gate(AND, liw, riw, outWires));
                }
                else if (notGates[riw] == -1)
                {
                    // If the left input wire comes from a 'not' then table should be 0010
                    gates.push_back(Gate(2, notGates[liw], riw, outWires));
                }
                else if (notGates[liw] == -1)
                {
                    // If the right input wire comes from a 'not' then table should be 0100
                    gates.push_back(Gate(4, liw, notGates[riw], outWires));
                }
                else
                {
                    // If both wires come from 'not' gates then should be 0001
                    gates.push_back(Gate(1, notGates[liw], notGates[riw], outWires));
                }
            }
            else if (type == " XOR")
            {
                if (notGates[liw] == -1 && notGates[riw] == -1)
                {
                    // If this XOR gate is not preceded directly by any 'not'
                    gates.push_back(Gate(XOR, liw, riw, outWires));
                }
                else if (notGates[riw] == -1)
                {
                    // Then table should be 1001
                    gates.push_back(Gate(9, notGates[liw], riw, outWires));
                }
                else if (notGates[liw] == -1)
                {
                    gates.push_back(Gate(9, liw, notGates[riw], outWires));
                }
                else
                {
                    // If both wires come from 'not' gates then 0110
                    gates.push_back(Gate(XOR, notGates[liw], notGates[riw], outWires));
                }
            }
            else
            {
                std::cout << "Unsupported gate type '" << type << "'" << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
        else
        { // If it's a not gate (inverter)

            int iw; // Input wire label
            file >> iw;
            // Make sure that input wire label is valid
            assert(iw >= 0 && iw < (numWires - outSize));

            // If this not is preceded by a not then halt because this case has not
            // been taken care of yet
            if (notGates[iw] != -1)
            {
                std::cout << "There is a not preceded by a not... stopping execution"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            }

            int ow; // Output wire label
            file >> ow;
            // Make sure that output wire label is valid
            assert(ow < numWires && ow >= (inpSize0 + inpSize1));

            // Move to next line
            std::string line;
            std::getline(file, line);
            // Make sure that this is indeed a 'not' gate
            assert(line == " INV");

            // If this 'not' gate is an output gate
            if (ow < numWires && ow >= (numWires - outSize))
            {
                // Replace the not with an NAND\NOR gate whose left input and right
                // inputs are the inputs of this not gate and whose output wire is the
                // output of this not gate
                gates.push_back(Gate(NAND, iw, iw, std::vector<int>(1, ow)));
            }
            else
            {
                // Add not gate to array of not gates
                notGates[ow] = iw;
            }
        }
    }

    std::vector<int> iwlabels(inpSize0 + inpSize1);
    for (int i = 0; i < (inpSize0 + inpSize1); i++)
    {
        iwlabels[i] = i;
    }
    std::vector<int> owlabels(outSize);
    for (int i = 0; i < outSize; i++)
    {
        owlabels[i] = numWires - outSize + i;
    }
    Circuit C(gates, numWires, iwlabels, owlabels);
    C.addGateOutputLabels();
    return C;
}
