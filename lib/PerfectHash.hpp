#pragma once

#include <cassert>
#include <cstddef>

#if defined PERFECT_HASH_DEV | not defined NDEBUG
#include <algorithm>
#include <array>
#endif

namespace perfecthash {

template <typename THashDefinition>
class PerfectHashBase {
 public:
  constexpr PerfectHashBase() {}

  static std::size_t hash(typename THashDefinition::TKey key) {
#ifndef NDEBUG
    assert(
        ("key must be part of the allowed keySet",
         std::find(
             THashDefinition::keySet.begin(),
             THashDefinition::keySet.end(),
             key) != THashDefinition::keySet.end()));
#endif
    std::size_t baseHash = THashDefinition::baseHash(key);
    constexpr std::size_t bitmask = ~static_cast<std::size_t>(0) >>
        (8 * sizeof(std::size_t) - THashDefinition::bits);
    return (baseHash * THashDefinition::factor) & bitmask;
  }

  std::size_t operator()(typename THashDefinition::TKey key) const {
    return hash(key);
  }

#ifdef PERFECT_HASH_DEV
  static bool validate() {
    std::array<std::size_t, THashDefinition::keySet.size()> hashList;
    for (int i = 0; i < THashDefinition::keySet.size(); i++) {
      hashList[i] = hash(THashDefinition::keySet[i]);
    }

    std::sort(hashList.begin(), hashList.end());

    for (int i = 0; i < hashList.size() - 1; i++) {
      if (hashList[i] == hashList[i + 1]) {
        return false;
      }
    }

    return true;
  }
#endif
};

} // namespace perfecthash
