#pragma once

#include <cstddef>

namespace perfecthash {

template <typename THashDefinition>
class PerfectHashBase {
 public:
  PerfectHashBase() {}

  std::size_t operator()(typename THashDefinition::TKey key) const {
    std::size_t baseHash = THashDefinition::baseHash(key);
    constexpr std::size_t bitmask = ~static_cast<std::size_t>(0) >>
        (8 * sizeof(std::size_t) - THashDefinition::bits);
    return (baseHash * THashDefinition::factor) & bitmask;
  }
};

} // namespace perfecthash
