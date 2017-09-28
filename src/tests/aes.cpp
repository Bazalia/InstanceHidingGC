#include "binary.h"
#include "circuit.h"
#include "circuit_file_reader.h"
#include "evaluator.h"
#include "garbler.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " key"
                  << " msg" << std::endl;
        return 0;
    }

    // Reading and preparing input

    std::string k(argv[2]);
    std::string m(argv[1]);

    std::vector<int> input = hexToBin(k, 128);
    std::vector<int> mbin = hexToBin(m, 128);

    input.insert(input.end(), mbin.begin(), mbin.end());

    // std::vector<int> input;
    // for (char &b : k)
    // {
    //     int bit = b - '0';
    //     assert(bit >= 0 && bit <=9);
    //     input.push_back(bit);
    // }
    // for (char &b : m)
    // {
    //     int bit = b - '0';
    //     assert(bit >= 0 && bit <=9);
    //     input.push_back(bit);
    // }

    // Reading and preparing circuit

    auto t1 = std::chrono::high_resolution_clock::now();
    Circuit C = readBRISCircuit("../input/AES-non-expanded.txt");
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Circuit reading time (microseconds) = "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count()
              << std::endl;

    // Garbling circuit

    t1 = std::chrono::high_resolution_clock::now();
    Garbler garbler(input);
    garbler.garbleCircuit(C, 1);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Circuit garbling time (microseconds) = "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count()
              << std::endl;

    // Evaluating

    t1 = std::chrono::high_resolution_clock::now();
    Evaluator eva;
    std::vector<int> gres = eva.evaluate(C, garbler.getInput());
    std::vector<int> res = garbler.decodeOutput(gres);
    t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Evaluation time (microseconds) = "
              << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                     .count()
              << std::endl;

    // Check result and print it

    std::string result = binToHex(res);
    std::cout << "Result = " << result << std::endl;

    return 0;
}
