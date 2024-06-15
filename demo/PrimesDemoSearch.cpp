#include "demo/PrimesDemo.hpp"
#include "searcher/Searcher.hpp"

int main(int argc, char** argv) {
  perfecthash::PerfectHashSearcher<PrimeHashDefinition>::search(10);
  return 0;
}
