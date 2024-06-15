#include "demo/SquaresDemo.hpp"
#include "searcher/Searcher.hpp"

int main(int argc, char** argv) {
  perfecthash::PerfectHashSearcher<SquareHashDefinition>{}.search(10);
  return 0;
}
