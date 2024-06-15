#include "demo/TwoOneDemo.hpp"
#include "searcher/Searcher.hpp"

int main(int argc, char** argv) {
  perfecthash::PerfectHashSearcher<TwoOneHashDefinition>{}.search(10);
  return 0;
}
