#include "garbler.h"

Garbler::Garbler(const std::vector<int> &input) { circuitInput = input; }

std::vector<int> Garbler::decodeOutput(std::vector<int> output)
{
    std::vector<int> result(output.size());

    assert(output.size() == decodebits.size());

    for (int i = 0; i < output.size(); i++)
    {
        result[i] = output[i] ^ decodebits[i];
    }

    return result;
}

std::vector<int> Garbler::getInput() { return circuitInput; }

void Garbler::garbleCircuit(Circuit &C, int maxthreads)
{

    // Generate random bits for garbling
    ranBits = genNRandomBits(C.getWireCount());

    // Garble Gates

    // Pre process for multi-threads
    int ngates = C.gates.size();
    int nthreads = (ngates > maxthreads) ? maxthreads : ngates;
    int gatesPerThread = ngates / nthreads;
    int leftOverGates = ngates % nthreads;
    assert(nthreads >= 1);
    std::vector<std::thread> threads(nthreads - 1);

    // Start threads to garble the gates
    for (int i = 0; i < nthreads - 1; i++)
    {
        threads[i] = std::thread(&Garbler::garbleGates, this, std::ref(C.gates), i * gatesPerThread, gatesPerThread);
    }
    garbleGates(C.gates, (nthreads - 1) * gatesPerThread, gatesPerThread + leftOverGates);
    // Trying hardcoding 1 thread
    // garbleGates(C.gates, 0, C.gates.size());

    // Prepare output wire garbling bits for output decoding
    std::vector<int> owlabels = C.getOWLabels();
    decodebits.resize(owlabels.size());
    for (int i = 0; i < owlabels.size(); i++)
    {
        assert(i + C.getWireCount() - owlabels.size() < ranBits.size());
        decodebits[i] = ranBits[i + C.getWireCount() - owlabels.size()];
    }

    // Garble the input of the circuit
    std::vector<int> iwlabels = C.getIWLabels();
    for (int i = 0; i < iwlabels.size(); i++)
    {
        circuitInput[i] = circuitInput[i] ^ ranBits[iwlabels[i]];
    }

    for (int i = 0; i < nthreads - 1; i++)
    {
        threads[i].join();
    }
}

void Garbler::garbleGate(Gate &g)
{
    // Get input wire labels
    int liw = g.getLIW();
    int riw = g.getRIW();

    assert(liw < ranBits.size() && riw < ranBits.size());

    // Waste time
    // int a = 10234;
    // for (int i=0; i<10000000; i++) {
    //     a = a % 10000007;
    //     a++;
    // }
    // a = 10234;
    // for (int i=0; i<10000000; i++) {
    //     a = a % 10000007;
    //     a++;
    // }

    // Get output wire labels
    std::vector<int> outWires = g.getOutLabels();

    char r0 = (char)ranBits[liw];
    char r1 = (char)ranBits[riw];
    std::vector<char> r2s(outWires.size());

    for (int i = 0; i < outWires.size(); i++)
        r2s[i] = (char)ranBits[outWires[i]];

    g.garble(r0, r1, r2s);
}

void Garbler::garbleGates(std::vector<Gate> &gates, int start, int count)
{
    int end = count + start;
    for (int i = start; i < end; i++)
    {
        garbleGate(std::ref(gates[i]));
    }
}