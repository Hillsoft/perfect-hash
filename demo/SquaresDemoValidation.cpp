#define PERFECT_HASH_DEV 1

#include <iostream>

#include "demo/SquaresDemo.hpp"
#include "lib/PerfectHash.hpp"

using SquarePerfectHash = perfecthash::PerfectHashBase<SquareHashDefinition>;

int main(int argc, char** argv) {
  if (SquarePerfectHash::validate()) {
    std::cout << "Valid" << std::endl;
    return 0;
  } else {
    std::cout << "Invalid" << std::endl;
    return 1;
  }
}