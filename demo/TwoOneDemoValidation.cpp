#define PERFECT_HASH_DEV 1

#include <iostream>

#include "demo/TwoOneDemo.hpp"
#include "lib/PerfectHash.hpp"

using TwoOnePerfectHash = perfecthash::PerfectHashBase<TwoOneHashDefinition>;

int main(int argc, char** argv) {
  if (TwoOnePerfectHash::validate()) {
    std::cout << "Valid" << std::endl;
  } else {
    std::cout << "Invalid" << std::endl;
  }
}