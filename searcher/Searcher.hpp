#pragma once

#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>

namespace perfecthash {

void sortByBit(
    const std::vector<std::size_t>& hashesIn,
    std::vector<std::size_t>& hashesOut,
    std::size_t bit) {
  int zeros = 0;
  for (const auto& h : hashesIn) {
    if (((h >> bit) & 1) == 0) {
      zeros++;
    }
  }

  assert(hashesOut.size() == hashesIn.size());
  int zeroPos = 0;
  int onePos = zeros;
  for (const auto& h : hashesIn) {
    if (((h >> bit) & 1) == 0) {
      hashesOut[zeroPos] = h;
      zeroPos++;
    } else {
      hashesOut[onePos] = h;
      onePos++;
    }
  }
}

bool uniqBits(const std::vector<std::size_t>& hashes, std::size_t bits) {
  std::size_t bitmask =
      ~static_cast<std::size_t>(0) >> (8 * sizeof(std::size_t) - bits);
  std::size_t prev = hashes[0] & bitmask;
  for (int i = 1; i < hashes.size(); i++) {
    std::size_t cur = hashes[i] & bitmask;
    if (prev == cur) {
      return false;
    }
    prev = cur;
  }
  return true;
}

template <typename T>
bool hasDuplicates(std::vector<T> vec) {
  std::sort(vec.begin(), vec.end());
  for (int i = 0; i < vec.size() - 1; i++) {
    if (vec[i] == vec[i + 1]) {
      return true;
    }
  }
  return false;
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
  static void search(std::size_t numThreads) {
    PerfectHashSearcher searcher{};
    searcher.baseHashes_.clear();
    searcher.baseHashes_.reserve(THashDefinition::keySet.size());
    for (const auto& key : THashDefinition::keySet) {
      searcher.baseHashes_.emplace_back(THashDefinition::baseHash(key));
    }
    if (hasDuplicates(searcher.baseHashes_)) {
      std::cout << "There are hash collisions in the base hash\nTerminating"
                << std::endl;
      return;
    }

    std::cout << "Theoretical best max value is "
              << THashDefinition::keySet.size() << std::endl;
    std::cout << "Searching..." << std::endl;
    searcher.inProgress_.store(true, std::memory_order_relaxed);
    searcher.bestMaxValue_.store(
        std::numeric_limits<std::size_t>::max(), std::memory_order_relaxed);
    searcher.testedHashes_.store(0, std::memory_order_relaxed);
    searcher.start_ = std::chrono::steady_clock::now();

    {
      std::jthread printer{[&searcher]() { searcher.resultPrintImpl(); }};
      std::vector<std::jthread> workerThreads;
      workerThreads.reserve(numThreads);
      for (int i = 0; i < numThreads; i++) {
        workerThreads.emplace_back([&searcher]() { searcher.searchImpl(); });
      }

      std::cin.get();
      searcher.inProgress_.store(false, std::memory_order_relaxed);
    }

    searcher.printBestResult();
  }

 protected:
  PerfectHashSearcher() {}

  void searchImpl() {
    size_t testedHashes = 0;
    std::minstd_rand randomEngine{std::random_device{}()};
    std::vector<std::size_t> hashes;
    std::vector<std::size_t> hashSortBuffer;
    hashes.resize(THashDefinition::keySet.size());
    hashSortBuffer.resize(THashDefinition::keySet.size());

    while (inProgress_.load(std::memory_order_relaxed)) {
      std::size_t factor =
          std::uniform_int_distribution<std::minstd_rand::result_type>{
              0, std::numeric_limits<std::minstd_rand::result_type>::max()}(
              randomEngine);
      std::size_t shift =
          std::uniform_int_distribution<std::minstd_rand::result_type>{
              0, 63}(randomEngine);

      // Calculate hashes
      for (int i = 0; i < hashes.size(); i++) {
        hashes[i] = (baseHashes_[i] * factor) >> shift;
      }

      std::size_t bestMaxValueCache =
          bestMaxValue_.load(std::memory_order_relaxed);
      std::size_t bit;
      for (bit = 0; bit < 64 && (1ull << bit) < bestMaxValueCache; bit++) {
        sortByBit(hashes, hashSortBuffer, bit);
        hashes.swap(hashSortBuffer);
        if ((1ull << bit) >= hashes.size() && uniqBits(hashes, bit + 1)) {
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

      testedHashes++;
      if (factor % 100 == 0) {
        testedHashes_.fetch_add(testedHashes, std::memory_order_relaxed);
        testedHashes = 0;
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
      for (int i = 0; i < 8; i++) {
        std::cout << "\033[1A\033[K";
      }
    }
    std::cout << "\033[1A\033[K";
  }

  void printBestResult() const {
    std::unique_lock resultLock{resultMutex_};

    std::size_t testedHashes = testedHashes_.load(std::memory_order_relaxed);
    std::size_t elapsedMillis =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start_)
            .count();
    std::size_t testRate =
        elapsedMillis > 0 ? (1000 * testedHashes / elapsedMillis) : 0;

    std::cout << "Tested hashes: " << testedHashes << "\n"
              << "Test rate    : " << testRate << "/s\n";

    std::cout << "Best result found\n"
              << " Factor   : " << result_.factor_ << "\n"
              << " Shift    : " << result_.shift_ << "\n"
              << " Bits     : " << result_.bits_ << "\n"
              << " Max value: " << result_.maxValue_ << std::endl;
  }

  std::vector<size_t> baseHashes_;
  std::chrono::steady_clock::time_point start_;

  std::atomic<bool> inProgress_;
  std::atomic<size_t> bestMaxValue_;

  mutable std::mutex resultMutex_;
  PerfectHashResult result_;

  std::atomic<size_t> testedHashes_;
};

} // namespace perfecthash
