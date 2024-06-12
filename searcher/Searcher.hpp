#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>

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

struct PerfectHashResult {
 public:
  PerfectHashResult() {}
  PerfectHashResult(
      std::size_t factor,
      std::size_t shift,
      std::size_t bits,
      std::size_t maxValue)
      : factor_(factor), shift_(shift), bits_(bits), maxValue_(maxValue) {}

  std::size_t factor_ = 0;
  std::size_t shift_ = 0;
  std::size_t bits_ = 0;
  std::size_t maxValue_ = 0;
};

template <typename THashDefinition>
class PerfectHashSearcher {
 public:
  PerfectHashSearcher() {}

  void search() {
    std::cout << "Searching..." << std::endl;
    inProgress_.store(true, std::memory_order_relaxed);
    bestMaxValue_.store(
        std::numeric_limits<std::size_t>::max(), std::memory_order_relaxed);

    {
      std::jthread worker{[thisPtr = this]() { thisPtr->searchImpl(); }};

      std::cin.get();
      inProgress_.store(false, std::memory_order_relaxed);
    }

    printBestResult();
  }

 protected:
  void searchImpl() {
    while (inProgress_.load(std::memory_order_relaxed)) {
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
          std::size_t maxValue = (~static_cast<std::size_t>(0) >>
                                  (8 * sizeof(std::size_t) - (bit + 1))) &
              hashes.back();
          if (maxValue < bestMaxValue_.load(std::memory_order_relaxed)) {
            std::unique_lock resultLock{resultMutex_};
            if (maxValue < bestMaxValue_.load(std::memory_order_relaxed)) {
              bestMaxValue_.store(maxValue, std::memory_order_relaxed);
              result_ = PerfectHashResult{factor, shift, bit + 1, maxValue};
            }
          }
        }
      }
    }
  }

  void printBestResult() const {
    std::unique_lock resultLock{resultMutex_};
    std::cout << "Best result found\n"
              << " Factor   : " << result_.factor_ << "\n"
              << " Shift    : " << result_.shift_ << "\n"
              << " Bits     : " << result_.bits_ << "\n"
              << " Max value: " << result_.maxValue_ << std::endl;
  }

  std::atomic<bool> inProgress_;
  std::atomic<size_t> bestMaxValue_;

  mutable std::mutex resultMutex_;
  PerfectHashResult result_;
};

} // namespace perfecthash
