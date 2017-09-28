This code is a basic implementation of the instance hiding protocol introduced in https://dl.acm.org/citation.cfm?id=3011100

The code does not implement the full protocol namely it does not send or receive the circuit through the network and does not try to generate strong randomness. It is only a proof of concept code that can be used to benchmark.

To compile:
    mkdir build
    cd build
    cmake ..
    make

To run a simple addition:
    cd build
    bash ../scripts/add.sh 5000 7000

You can also run other circuits available in the input folder with the run binary as such:
    ./run -f ../input/somecircuit -i binaryinput -n "optional number of threads to garble in parallel"

You can also make your own circuits and try them out as long as they take the same form of circuits in http://www.cs.bris.ac.uk/Research/CryptographySecurity/MPC/

Acknowledgement:
This work was made possible with by the NPRP award  X-063-1–014 from the Qatar National Research Fund (a member of The Qatar Foundation).