#include "HopfieldSimulator.hpp"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// private function
// regrid_impl()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::regrid_impl(
    size_t numColumns, size_t numRows) {
  for (auto &element : patterns_) {
    element->regrid(numColumns, numRows);
  }
}

// public function
// getPatterns()

template <typename neurons_type, typename matrix_type>
const std::vector<std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>> &
HS::HopfieldSimulator<neurons_type, matrix_type>::getPatterns() const noexcept {

  return patterns_;
};

// clear()

template <typename neurons_type, typename matrix_type>

void HS::HopfieldSimulator<neurons_type, matrix_type>::clear(const int index) {

  patterns_[index]->clearEnergy();
}

// corruptPattern()
template <typename neurons_type, typename matrix_type>

void HS::HopfieldSimulator<neurons_type, matrix_type>::corruptPattern(
    const size_t index, const float noise) {
  std::lock_guard<std::mutex> lock(mtx_);
  patterns_[index]->reCorrupt(noise);
}

// emplace_pattern()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::emplace_pattern(
    const std::string &path, const int a, const int b) {

  if (patterns_.size() > 0) {
    if (!patterns_[0]->hasTheseDimensions(a, b)) {
      throw std::runtime_error(
          "Dimensions are incompatible; you have to regrid!");
    }
  }

  patterns_.emplace_back(
      std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(path, a, b));
}

template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::emplace_pattern(
    const float noise, const int a, const int b) {
  if (patterns_.size() > 0) {
    if (!patterns_[0]->hasTheseDimensions(a, b)) {
      throw std::runtime_error(
          "Dimensions are incompatible; you have to regrid!");
    }
  }

  patterns_.emplace_back(
      std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(noise, a, b));
}

// cyclePixelStateOnPattern()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::cyclePixelStateOnPattern(
    const size_t index, const size_t pos) {
  std::lock_guard<std::mutex> lock(mtx_);

  patterns_[index]->flipNoisyPixel(pos);
}
// generatePattern()
template <typename neurons_type, typename matrix_type>

void HS::HopfieldSimulator<neurons_type, matrix_type>::generatePattern(
    const float noise, const std::size_t numColumns,
    const std::size_t numRows) {
  patterns_.emplace_back(
      std::make_unique<CSP::CoherenceSetPattern<neurons_type>>(
          noise, numColumns, numRows));
}

// regrid()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::regrid(size_t numColumns,
                                                              size_t numRows) {

  std::lock_guard<std::mutex> lock(mtx_);
  regrid_impl(numColumns, numRows);
}

// removePattern()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::removePattern(
    const size_t index) {
  std::lock_guard<std::mutex> lock(mtx_);

  if (index < patterns_.size()) {
    patterns_.erase(patterns_.begin() + index);
  } else {
    throw std::runtime_error("The pattern you want to delete doesn't exist");
  }
}

// resolvePattern()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::resolvePattern(
    const int index, std::atomic<float> *status) {
  std::lock_guard<std::mutex> lock(mtx_);

  if (patterns_.size() == 0) {
    return;
  }
  patterns_[index]->resolveEvolvingPattern(hn_, status);
}

// saveFileTraining()

template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::saveFileTraining(
    const std::string &str_buffer) {
  if (patterns_.size() == 0) {
    return;
  }

  const size_t numColumns = patterns_[0]->getCol();
  const size_t numRows = patterns_[0]->getRow();

  const std::vector<matrix_type> &matrix = hn_.getTraining();

  try {

    std::filesystem::path filePath(str_buffer);
    filePath.replace_extension(".training");
    std::filesystem::path directoryPath = filePath.parent_path();

    if (!directoryPath.empty() && !std::filesystem::exists(directoryPath)) {
      if (!std::filesystem::create_directories(directoryPath)) {
        throw std::logic_error("Problems with the creation of the folder: " +
                               directoryPath.string());
      }
    }

    std::ofstream outFile(filePath);

    if (!outFile.is_open()) {
      throw std::logic_error("Cannot open file for writing: " +
                             filePath.string());
    }

    outFile << numRows << " " << numColumns << "\n";

    if (!matrix.empty()) {

      for (const auto &element : matrix) {
        outFile << element << " ";
      }
    }

    outFile.close();

  } catch (const std::filesystem::filesystem_error &e) {
    throw std::logic_error("Error of the filesystem: " + std::string(e.what()));
  } catch (const std::exception &e) {
    throw;
  }
}

// setTraining()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::setTraining(
    const int numColumns, const int numRows, std::vector<matrix_type> &matrix) {
  std::lock_guard<std::mutex> lock(mtx_);

  const int dim = std::sqrt(matrix.size());
  if (dim * dim != matrix.size()) {
    throw std::logic_error("Dimensions are incompatible");
  }
  hn_.setTraining(matrix);
  regrid_impl(numColumns, numRows);
}

// trainNetworkHebb()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::trainNetworkHebb(
    std::atomic<float> *status) {
  std::lock_guard<std::mutex> lock(mtx_);

  auto function =
      [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>
             &csp_ptr) { return csp_ptr->getTrainingPatternVector(); };

  hn_.trainNetworkWithHebb(patterns_, function, status);
}

// trainNetworkWithPseudoinverse()
template <typename neurons_type, typename matrix_type>
void HS::HopfieldSimulator<neurons_type, matrix_type>::
    trainNetworkWithPseudoinverse(std::atomic<float> *status) {
  std::lock_guard<std::mutex> lock(mtx_);

  auto function =
      [](const std::unique_ptr<CSP::CoherenceSetPattern<neurons_type>>
             &csp_ptr) { return csp_ptr->getTrainingPatternVector(); };

  hn_.trainNetworkWithPseudoinverse(patterns_, function, status);
}

// size()
template <typename neurons_type, typename matrix_type>
size_t HS::HopfieldSimulator<neurons_type, matrix_type>::size() const noexcept {
  return patterns_.size();
}

// checkDimension()

template <typename neurons_type, typename matrix_type>
bool HS::HopfieldSimulator<neurons_type,
                           matrix_type>::checkDimension() noexcept {
  if (patterns_.size() == 0) {
    return false;
  }
  const size_t cols = patterns_[0]->getCol();
  const size_t rows = patterns_[0]->getRow();
  return rows * cols * rows * cols == hn_.getTraining().size();
}