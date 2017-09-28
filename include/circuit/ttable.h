#ifndef TTABLE_H
#define TTABLE_H

#include <string>
#include <algorithm>
#include <cassert>

/*
    A typical truth table of L op R looks like this

                L   |   R   |   L op R
                0   |   0   |   0 op 0
                0   |   1   |   0 op 1
                1   |   0   |   1 op 0
                1   |   1   |   1 op 1
    
    If we represent the possible values of L and R in binary as 1100 and 1010
    respectively then L = 12 in decimal and R = 10 in decimal.

    Using the same logic we say that the truth table of an XOR 0110 is 6 in decimal
    an AND is 8 and so on.

    Hence any truth table can be represented by a decimal from 0 to 15 which can
    fit on 4 bits but since the smallest premitive in C\C++ is char we use that to
    represent a table instead.
*/

class TTable
{
    //  The table itself only needs 4 bits but char is the smallest size I know
    char t;

  public:
    /*
    Description:
        Constructs an empty Truth Table. Must be initialized through setTable before
        using the other methods.
    Output:
        Empty truth table object that shouldn't be used before setTable.
    */
    TTable();

    /*
    Description:
        Constructs a Truth Table using the given binary representation of char table
    Input:
        table - a char representing the truth values of the table. Must be between 0
                and 15 in decimal (e.g. AND 1000 is 8).
    Output:
        a valid truth table object with the given table values
    */
    TTable(char table);

    /*
    Description:
        Sets the value of the truth table overwriting any existing table.
    Input:
        table - a char representing the truth values of the table. Must be between 0
                and 15 in decimal (e.g. AND 1000 is 8).
    Output:
        a valid truth table with the updated table
    */
    void setTable(char table);

    /*
    Description:
        Looks up the truth table value where with inputs l and r
    Input:
        l - must be a 0 or a 1 representing the truth value of the left input to the table
        r - must be a 0 or a 1 representing the truth value of the right input to the table
    Output:
        The truth table value given the left input l and right input r
    */
    char lookup(char l, char r);

    /*
    Description:
        Returns a char representation of the table. This method is meant to be used to
        create other tables that have the same truth values. That is to allow for copying a table
        or serialization and deserialization
    Output:
        Output is a char representing the truth table (e.g. AND that is 1000 is 8)
    */
    char getTable();

    /*
    Description:
        Garbles the table using the three random bits r0, r1 and r2 based on the protocol
        introduced in https://dl.acm.org/citation.cfm?id=3011100 "An efficient instance 
        hiding scheme"
    Input:
        r0 - a random bit 0 or 1 generated to xor into the left input to the truth table
        r1 - a random bit 0 or 1 generated to xor into the right input to the truth table
        r2 - a random bit 0 or 1 generated to xor into the output value of the truth table
    Output:
        None but modifies the truth table in the object
    */
    void garble(char r0, char r1, char r2);

    /*
    Description:
        Returns the string representation of the truth table
    Output:
        The truth table output is such that the significant bit is on the right 
        (e.g. an AND truth table would be 0001)
    */
    std::string toString();
};

#endif