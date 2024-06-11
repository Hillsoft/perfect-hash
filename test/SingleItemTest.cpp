#include <functional>
#include <iostream>

#include "lib/PerfectHash.hpp"

class SingleIntHashDefinition {
 public:
  using TKey = int;
  constexpr static std::size_t factor = 1;
  constexpr static std::size_t bits = 1;

  static std::size_t baseHash(TKey key) { return std::hash<int>{}(key); }

#if defined PERFECT_HASH_DEV | not defined NDEBUG
  static constexpr std::array<std::size_t, 1> keySet{1};
#endif
};

using SingleIntHash = perfecthash::PerfectHashBase<SingleIntHashDefinition>;

int main(int argc, char** argv) {
#ifdef PERFECT_HASH_DEV
  std::cout << "Is valid: " << SingleIntHash::validate() << std::endl;
#endif

  for (int i = 0; i < 10; i++) {
    std::cout << SingleIntHash{}(i) << std::endl;
  }
}
