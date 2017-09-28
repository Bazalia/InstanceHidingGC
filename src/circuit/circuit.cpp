#include "circuit.h"

Circuit::Circuit(std::vector<Gate> &inpGates, int wcount,
                 std::vector<int> &inputWireLabels,
                 std::vector<int> &outputWireLabels)
{
    gates = inpGates;
    iwlabels = inputWireLabels;
    owlabels = outputWireLabels;
    wirecount = wcount;
}

Circuit::Circuit(){}

int Circuit::getWireCount() { return wirecount; }

std::vector<int> &Circuit::getIWLabels() { return iwlabels; }

std::vector<int> &Circuit::getOWLabels() { return owlabels; }

/***
  Description:
    If multiple wires are coming out from a gate then they should be labeled
    differently. This is a requirement in this garbling scheme and this
    function basically takes a circuit and adds wire labels if multiple wires
    came out of gates.
  Assumes:
    The circuit's gates only have one output wire label
****/
void Circuit::addGateOutputLabels()
{
    // Index is a wire label. Result is the index of the gate that outputs that
    // wire label or -1 if none exist
    std::vector<int> outLab2Gate(wirecount, -1);

    // Index is the gate index and the value is 0 or 1 depending on whether the
    // output label of the respective gate has already inputed to another gate or
    // not
    std::vector<int> doesGateFanout(gates.size(), 0);

    for (int g = 0; g < gates.size(); g++)
    {
        int liw = gates[g].getLIW();
        int parent = outLab2Gate[liw];

        // If the left wire came from another gate
        if (parent != -1)
        {
            // If that gate already used its output wire label for another gate
            if (doesGateFanout[parent] == 1)
            {
                // Add another wire to the circuit
                wirecount++;

                // Replace this gate's input wire with the new wire
                gates[g].setLIW(wirecount - 1);

                // Add the new wire to the list of output wires of the parent gate
                gates[parent].addOutLabel(wirecount - 1);
            }
            else
            {
                // Use the parent's output wire for this gate so the next gate that has
                // the same parent gate will have to create a new wire label
                doesGateFanout[parent] = 1;
            }
        }

        int riw = gates[g].getRIW();
        parent = outLab2Gate[riw];

        // If the right wire came from another gate
        if (parent != -1)
        {
            // If that gate already used its output wire label for another gate
            if (doesGateFanout[parent] == 1)
            {
                // Add another wire to the circuit
                wirecount++;

                // Replace this gate's input wire with the new wire
                gates[g].setRIW(wirecount - 1);

                // Add the new wire to the list of output wires of the parent gate
                gates[parent].addOutLabel(wirecount - 1);
            }
            else
            {
                // Use the parent's output wire for this gate so the next gate that has
                // the same parent gate will have to create a new wire label
                doesGateFanout[parent] = 1;
            }
        }

        // This gate's output wire label points to this gate
        outLab2Gate[gates[g].getOutLabels()[0]] = g;
    }

    // Loop over output wires and check if there is some gate fanout there
    for (int w = 0; w < owlabels.size(); w++)
    {

        // Find where this wire is coming from
        int parent = outLab2Gate[owlabels[w]];

        // Check if it is coming from a gate
        if (parent != -1)
        {
            // If that gate already used its output wire label elsewhere
            if (doesGateFanout[parent] == 1)
            {
                // Add another wire to the circuit
                wirecount++;

                // Replace this output wire with the new wire
                owlabels[w] = wirecount - 1;

                // Add the new wire to the list of output wires of the parent gate
                gates[parent].addOutLabel(wirecount - 1);
            }
            else
            {
                // Use the parent's output wire for this gate so the next gate that has
                // the same parent gate will have to create a new wire label
                doesGateFanout[parent] = 1;
            }
        }
    }
}

// Converts the circuit to a string for display purposes
std::string Circuit::toString()
{
    // The circuit
    std::string c;

    // Just appends the gates together
    for (int i = 0; i < gates.size(); i++)
    {
        c += gates[i].toString();
    }

    return c;
}
