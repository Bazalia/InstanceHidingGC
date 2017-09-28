#include "random.h"

// Dummy function that generates alternating 0s and 1s
std::vector<int> genNRandomBits(int n)
{
    std::vector<int> res(n);

    for (int i = 0; i < n; i++)
    {
        res[i] = i % 2;
    }

    return res;
}
