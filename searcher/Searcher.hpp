#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace perfecthash {

template <std::size_t N>
void sortByBit(std::array<std::size_t, N>& hashes, std::size_t bit) {
  int zeros = 0;
  for (const auto& h : hashes) {
    if (((h >> bit) & 1) == 0) {
      zeros++;
    }
  }

  std::array<std::size_t, N> sortedHashes;
  int zeroPos = 0;
  int onePos = zeros;
  for (const auto& h : hashes) {
    if (((h >> bit) & 1) == 0) {
      sortedHashes[zeroPos] = h;
      zeroPos++;
    } else {
      sortedHashes[onePos] = h;
      onePos++;
    }
  }

  hashes.swap(sortedHashes);
}

template <std::size_t N>
bool uniqBits(const std::array<std::size_t, N>& hashes, std::size_t bits) {
  std::size_t bitmask =
      ~static_cast<std::size_t>(0) >> (8 * sizeof(std::size_t) - bits);
  std::size_t prev = hashes[0] & bitmask;
  for (int i = 1; i < N; i++) {
    std::size_t cur = hashes[i] & bitmask;
    if (prev == cur) {
      return false;
    }
    prev = cur;
  }
  return true;
}

template <typename THashDefinition>
class PerfectHashSearcher {
 public:
  PerfectHashSearcher() {}

  void search() {
    std::cout << "Searching..." << std::endl;

    while (true) {
      std::size_t factor = std::rand();
      std::size_t shift = std::rand() % 64;

      // Calculate hashes
      std::array<std::size_t, THashDefinition::keySet.size()> hashes;
      for (int i = 0; i < hashes.size(); i++) {
        hashes[i] =
            (THashDefinition::baseHash(THashDefinition::keySet[i]) * factor) >>
            shift;
      }

      std::size_t bit;
      for (bit = 0; bit < 64; bit++) {
        sortByBit(hashes, bit);
        if (uniqBits(hashes, bit + 1)) {
          std::cout << "Found result:\n factor: " << factor
                    << "\n shift: " << shift << "\n bits: " << bit + 1
                    << std::endl;
          std::cout << "Maximum hash value: "
                    << ((~static_cast<std::size_t>(0) >>
                         (8 * sizeof(std::size_t) - (bit + 1))) &
                        hashes.back())
                    << std::endl;
          return;
        }
      }
    }
  }
};

} // namespace perfecthash
