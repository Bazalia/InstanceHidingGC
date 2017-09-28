#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <boost/program_options.hpp>
#include "circuit.h"
#include "circuit_file_reader.h"
#include "garbler.h"
#include "evaluator.h"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
    // Getting command line inputs

    // Initializing inputs
    std::string filename, input;
    int numthreads = 1;

    // Declare supported options
    po::options_description desc("Options");
    desc.add_options()("help,h", "produce help message")("file,f", po::value<std::string>(&filename), "path to the circuit file. Format found here http://www.cs.bris.ac.uk/Research/CryptographySecurity/MPC/")("input,i", po::value<std::string>(&input), "input to the circuit in binary")("numthreads,n", po::value<int>(&numthreads), "number of threads to spawn for garbling");
    
    // Parse the inputs to main
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    // Check all required inputs are available
    if (vm.count("help") || !vm.count("file") || !vm.count("input"))
    {
        std::cout << "Usage: " << argv[0] << " -f circuit_file -i input" << std::endl;
        std::cout << desc << std::endl;
        return 0;
    }

    std::vector<int> input_vector(input.size());
    for (int i = 0; i < input.size(); i++)
    {
        int b = input[i] - '0';
        if (b != 0 && b != 1)
        {
            std::cout << "Input must be in binary. Unkown value " << input[i] << std::endl;
            return 0;
        }
        input_vector[i] = b;
    }

    std::cout << "Read the following input ";
    for (int i = 0; i < input_vector.size(); i++)
    {
        std::cout << input_vector[i];
    }
    std::cout << std::endl;

    // Read the circuit
    Circuit C;

    C = readBRISCircuit(filename);

    // Garble the circuit
    auto t1 = std::chrono::high_resolution_clock::now();
    Garbler G(input_vector);
    G.garbleCircuit(C, numthreads);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Circuit garbling time (microseconds) = "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count()
              << std::endl;

    // Evaluate the circuit
    t1 = std::chrono::high_resolution_clock::now();
    Evaluator E;
    std::vector<int> gres = E.evaluate(C, G.getInput());
    std::vector<int> res = G.decodeOutput(gres);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Circuit evaluation time (microseconds) = "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count()
              << std::endl;

    // Print circuit size
    std::cout << "Circuit size = " << C.gates.size() << std::endl;

    // Print output
    for (int i = 0; i < res.size(); i++)
    {
        std::cout << res[i];
    }
    std::cout << std::endl;

    return 0;
}