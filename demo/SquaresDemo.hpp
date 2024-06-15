#include <array>
#include <cstdint>
#include <functional>

class SquareHashDefinition {
 public:
  using TKey = int;

  // Some sample values we got from a search
  constexpr static std::size_t factor = 7260173331976099771;
  constexpr static std::size_t shift = 58;
  constexpr static std::size_t bits = 5;
  constexpr static std::size_t maxValue = 27;

  static std::size_t baseHash(TKey key) { return std::hash<int>{}(key); }

  static constexpr std::array<int, 20> keySet{1,   4,   9,   16,  25,  36,  49,
                                              64,  81,  100, 121, 144, 169, 196,
                                              225, 256, 289, 324, 361, 400};
};
