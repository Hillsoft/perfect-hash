#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

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

  void search(std::size_t numThreads) {
    std::cout << "Theoretical best max value is "
              << THashDefinition::keySet.size() - 1 << std::endl;
    std::cout << "Searching..." << std::endl;
    inProgress_.store(true, std::memory_order_relaxed);
    bestMaxValue_.store(
        std::numeric_limits<std::size_t>::max(), std::memory_order_relaxed);

    {
      std::vector<std::jthread> workerThreads;
      workerThreads.reserve(numThreads);
      for (int i = 0; i < numThreads; i++) {
        workerThreads.emplace_back(
            [thisPtr = this]() { thisPtr->searchImpl(); });
      }
      std::jthread printer{[thisPtr = this]() { thisPtr->resultPrintImpl(); }};

      std::cin.get();
      inProgress_.store(false, std::memory_order_relaxed);
    }

    printBestResult();
  }

 protected:
  void searchImpl() {
    std::minstd_rand randomEngine{std::random_device{}()};
    while (inProgress_.load(std::memory_order_relaxed)) {
      std::size_t factor =
          std::uniform_int_distribution<std::minstd_rand::result_type>{
              0, std::numeric_limits<std::minstd_rand::result_type>::max()}(
              randomEngine);
      std::size_t shift =
          std::uniform_int_distribution<std::minstd_rand::result_type>{
              0, 63}(randomEngine);

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

  void resultPrintImpl() const {
    while (inProgress_.load(std::memory_order_relaxed)) {
      printBestResult();
      std::cout << "Press enter to stop search..." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      // Clear lines
      std::cout << "\r\033[K";
      for (int i = 0; i < 6; i++) {
        std::cout << "\033[1A\033[K";
      }
    }
    std::cout << "\033[1A\033[K";
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
