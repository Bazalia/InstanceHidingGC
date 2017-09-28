#include "evaluator.h"

Evaluator::Evaluator() {}

std::vector<int> Evaluator::evaluate(Circuit &C, std::vector<int> input)
{
    std::vector<int> iwlabels = C.getIWLabels();
    std::vector<int> owlabels = C.getOWLabels();

    // prepare wires of the circuit and add input data to them
    // -1 means that this wire was yet to be evaluated so it can't be used
    std::vector<int> wires(C.getWireCount(), -1);
    for (int i = 0; i < iwlabels.size(); i++)
    {
        wires[iwlabels[i]] = input[i];
        // std::cout << "Wire " << iwlabels[i] << " = " << wires[iwlabels[i]]
        //           << std::endl;
    }

    // evaluate the gates sequentially assuming they are topologically sorted
    for (int i = 0; i < C.gates.size(); i++)
    {
        // get left wire bit
        int li = wires[C.gates[i].getLIW()];
        // std::cout << "li (" << C.gates[i].getLIW() << ") = " << li << std::endl;

        // get right wire bit
        int ri = wires[C.gates[i].getRIW()];
        // std::cout << "ri (" << C.gates[i].getRIW() << ") = " << ri << std::endl;

        // Make sure these wires have been evaluated
        assert(li != -1 && ri != -1);

        // evaluate the gate with these bits and write output to wire
        // each output wire will have to be evaluated independently
        std::vector<int> outwires = C.gates[i].getOutLabels();
        std::vector<int> result = C.gates[i].evaluate(li, ri);
        assert(outwires.size() == result.size());
        // std::cout << C.gates[i].toString();
        for (int j = 0; j < outwires.size(); j++)
        {
            wires[outwires[j]] = result[j];
            // std::cout << "Wire " << outwires[j] << " = " << wires[outwires[j]]
            //           << std::endl
            //           << std::endl;
        }
    }

    // return the values of the output wires
    std::vector<int> result(owlabels.size());
    // std::cout << "First owlabel = " << owlabels[0] << std::endl;
    // std::cout << "Result from evaluator = ";
    for (int i = 0; i < result.size(); i++)
    {
        // std::cout << wires[owlabels[i]];
        result[i] = wires[owlabels[i]];
    }
    // std::cout << std::endl;

    return result;
}
