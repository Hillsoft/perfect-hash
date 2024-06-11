#include <array>
#include <cstdint>
#include <functional>

#include "searcher/Searcher.hpp"

class PrimeHashDefinition {
 public:
  using TKey = int;

  static std::size_t baseHash(TKey key) { return std::hash<int>{}(key); }

  static constexpr std::array<int, 10> keySet{
      2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
};

int main(int argc, char** argv) {
  perfecthash::PerfectHashSearcher<PrimeHashDefinition>{}.search();
  return 0;
}
