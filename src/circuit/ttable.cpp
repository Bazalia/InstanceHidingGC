#include "ttable.h"

TTable::TTable()
{
    setTable(0);
}

TTable::TTable(char table)
{
    setTable(table);
}

void TTable::setTable(char table)
{
    assert(table >= 0 && table <= 15);
    t = table;
}

char TTable::lookup(char l, char r)
{
    assert(l == 0 || l == 1);
    assert(r == 0 || r == 1);
    return (t >> l + l + r) & 1;
}

char TTable::getTable()
{
    assert(t >= 0 && t <= 15);
    return t;
}

void TTable::garble(char r0, char r1, char r2)
{
    assert(r0 == 0 || r0 == 1);
    assert(r1 == 0 || r1 == 1);
    assert(r2 == 0 || r2 == 1);

    // new table
    char nt = 0;

    // expand 1 bit to 4 bits because we need to xor with 4 bits of truth values
    if (r2 == 1)
        r2 = 15;

    nt = nt | lookup(0 ^ r0, 0 ^ r1);
    nt = nt | (lookup(0 ^ r0, 1 ^ r1) << 1);
    nt = nt | (lookup(1 ^ r0, 0 ^ r1) << 2);
    nt = nt | (lookup(1 ^ r0, 1 ^ r1) << 3);

    t = nt ^ r2;
    assert(t >=0 && t <= 15);
}

std::string TTable::toString()
{
    std::string res;
    char table = t;
    for (int i = 0; i < 4; i++)
    {
        (table % 2) ? res += '1' : res += '0';
        table = table >> 1;
    }
    return res;
}