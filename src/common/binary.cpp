#include "binary.h"

// Given a decimal number returns its binary representation in a vector
// of length binlen. The binary number will be from right to left or left to
// right depending on if direction is true or false respectively (Assumes dec is
// positive)
std::vector<int> decToBin(int dec, int binlen, bool direction) {

  std::vector<int> res(binlen, 0);
  int i = binlen - 1;

  while (i >= 0) {
    if (direction) {
      res[i] = dec % 2;
    } else {
      res[binlen - i - 1] = dec % 2;
    }
    dec /= 2;
    i--;
  }

  return res;
}

/***
  Description:
    converts a binary number to a decimal one (note this can overflow)
  Inputs:
    bin - is a positive binary number made of 0s and 1s
    direction - true is normal direction false is flipped
  Outputs:
    int that is the representation of the binary number (could be overflown)
 ***/
int64_t binToDec(std::vector<int> bin, bool direction) {
  int64_t res = 0;
  for (int i = 0; i < bin.size(); i++) {
    if (direction) {
      res += bin[bin.size() - i - 1] * pow(2, i);
    } else {
      res += bin[i] * pow(2, i);
    }
  }
  return res;
}

/***
  Description:
    converts a hex string to a binary vector. assumes trailing zeroes
    from the left of hex if |hex| != binlen/4
  Inputs:
    hex is the hex string which must be correct. |hex| <= binlen/4
    binlen is the length of the resulting binary number
  Outputs:
    int vector of length binlen of the binary representation of hex
 ***/
std::vector<int> hexToBin(std::string hex, int binlen) {
  assert(hex.size() <= binlen / 4);
  std::vector<int> bin(binlen, 0);
  for(int i = hex.size() - 1; i >= 0; i--) {
    switch(hex[hex.size() - i - 1]) 
    {
      case '0':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '1':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '2':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '3':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '4':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '5':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '6':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '7':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 0;
        break;
      case '8':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 1;
        break;
      case '9':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'A':
      case 'a':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'B':
      case 'b':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 0;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'C':
      case 'c':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'D':
      case 'd':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 0;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'E':
      case 'e':
        bin[binlen - 4*i - 1] = 0;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 1;
        break;
      case 'F':
      case 'f':
        bin[binlen - 4*i - 1] = 1;
        bin[binlen - 4*i - 2] = 1;
        bin[binlen - 4*i - 3] = 1;
        bin[binlen - 4*i - 4] = 1;
        break;
      default:
        std::cout << "In correct hex value " << hex[i] << std::endl;
        std::exit(EXIT_FAILURE);
    }
  }
  return bin;
}

/***
  Description:
    converts a binary number to a hex
 ***/
std::string binToHex(std::vector<int> bin) {
  assert(bin.size() % 4 == 0);
  std::string hex;
  for (int i = 0; i < bin.size() - 3; i += 4){
    if (bin[i] == 0 && bin[i+1] == 0 && bin[i+2] == 0 && bin[i+3] == 0) {
      hex += "0";
    } else if (bin[i] == 0 && bin[i+1] == 0 && bin[i+2] == 0 && bin[i+3] == 1) {
      hex += "1";
    } else if (bin[i] == 0 && bin[i+1] == 0 && bin[i+2] == 1 && bin[i+3] == 0) {
      hex += "2";
    } else if (bin[i] == 0 && bin[i+1] == 0 && bin[i+2] == 1 && bin[i+3] == 1) {
      hex += "3";
    } else if (bin[i] == 0 && bin[i+1] == 1 && bin[i+2] == 0 && bin[i+3] == 0) {
      hex += "4";
    } else if (bin[i] == 0 && bin[i+1] == 1 && bin[i+2] == 0 && bin[i+3] == 1) {
      hex += "5";
    } else if (bin[i] == 0 && bin[i+1] == 1 && bin[i+2] == 1 && bin[i+3] == 0) {
      hex += "6";
    } else if (bin[i] == 0 && bin[i+1] == 1 && bin[i+2] == 1 && bin[i+3] == 1) {
      hex += "7";
    } else if (bin[i] == 1 && bin[i+1] == 0 && bin[i+2] == 0 && bin[i+3] == 0) {
      hex += "8";
    } else if (bin[i] == 1 && bin[i+1] == 0 && bin[i+2] == 0 && bin[i+3] == 1) {
      hex += "9";
    } else if (bin[i] == 1 && bin[i+1] == 0 && bin[i+2] == 1 && bin[i+3] == 0) {
      hex += "A";
    } else if (bin[i] == 1 && bin[i+1] == 0 && bin[i+2] == 1 && bin[i+3] == 1) {
      hex += "B";
    } else if (bin[i] == 1 && bin[i+1] == 1 && bin[i+2] == 0 && bin[i+3] == 0) {
      hex += "C";
    } else if (bin[i] == 1 && bin[i+1] == 1 && bin[i+2] == 0 && bin[i+3] == 1) {
      hex += "D";
    } else if (bin[i] == 1 && bin[i+1] == 1 && bin[i+2] == 1 && bin[i+3] == 0) {
      hex += "E";
    } else {
      hex += "F";
    }
  }
  return hex;
}