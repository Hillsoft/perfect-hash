#include <array>
#include <cstdint>
#include <functional>

std::vector<uint64_t> getTwoOneKeySet() {
  std::vector<uint64_t> result;
  for (int i = 0; i < 64; i++) {
    for (int j = i + 1; j < 64; j++) {
      result.emplace_back(
          (1ull << static_cast<uint64_t>(i)) +
          (1ull << static_cast<uint64_t>(j)));
    }
  }
  return result;
}

class TwoOneHashDefinition {
 public:
  using TKey = uint64_t;

  // Some sample values we got from a search
  constexpr static std::size_t factor = 7504714790840675698;
  constexpr static std::size_t shift = 47;
  constexpr static std::size_t bits = 17;
  constexpr static std::size_t maxValue = 130641;

  static std::size_t baseHash(TKey key) { return std::hash<uint64_t>{}(key); }

  static std::vector<uint64_t> keySet;
};

std::vector<uint64_t> TwoOneHashDefinition::keySet = getTwoOneKeySet();
