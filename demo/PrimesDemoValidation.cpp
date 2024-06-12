#define PERFECT_HASH_DEV 1

#include <iostream>

#include "demo/PrimesDemo.hpp"
#include "lib/PerfectHash.hpp"

using PrimePerfectHash = perfecthash::PerfectHashBase<PrimeHashDefinition>;

int main(int argc, char** argv) {
  if (PrimePerfectHash::validate()) {
    std::cout << "Valid" << std::endl;
  } else {
    std::cout << "Invalid" << std::endl;
  }
}