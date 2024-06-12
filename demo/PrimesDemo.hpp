#include <array>
#include <cstdint>
#include <functional>

class PrimeHashDefinition {
 public:
  using TKey = int;

  // Some sample values we got from a search
  constexpr static std::size_t factor = 1866174325241492882;
  constexpr static std::size_t shift = 23;
  constexpr static std::size_t bits = 5;

  static std::size_t baseHash(TKey key) { return std::hash<int>{}(key); }

  static constexpr std::array<int, 20> keySet{2,  3,  5,  7,  11, 13, 17,
                                              19, 23, 29, 31, 37, 41, 43,
                                              47, 53, 59, 61, 67, 71};
};
