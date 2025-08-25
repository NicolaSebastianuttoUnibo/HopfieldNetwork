
#define STB_IMAGE_IMPLEMENTATION

#include "../stb/stb_image.h"
#include "ITrainingPattern.hpp"

#include <algorithm>
#include <stdexcept>

template <typename T> static auto POINTS = MD::getMathematicalVertex<T>();

// StbiImageDeleter
void TP::StbiImageDeleter::operator()(unsigned char *data) const {
  if (data) {
    stbi_image_free(data);
  }
}

// private function of ImageTrainingPattern
// calculateIntegralImage()
template <typename T>
void TP::ImageTrainingPattern<T>::calculateIntegralImage() {
  const std::size_t integral_width = imgWidth_ + 1;
  const std::size_t integral_height = imgHeight_ + 1;
  imgIntegral_.assign(integral_width * integral_height, 0);

  for (int y = 0; y < imgHeight_; ++y) {
    for (int x = 0; x < imgWidth_; ++x) {
      unsigned char *pixel =
          imgData_.get() + (y * imgWidth_ + x) * imgChannels_;

      int luminance = 0;
      if (imgChannels_ >= 3) {
        luminance = (pixel[0] + pixel[1] + pixel[2]) / 3;
      } else {
        luminance = pixel[0];
      }

      imgIntegral_[(y + 1) * integral_width + (x + 1)] =
          luminance + imgIntegral_[y * integral_width + (x + 1)] +
          imgIntegral_[(y + 1) * integral_width + x] -
          imgIntegral_[y * integral_width + x];
    }
  }
}

// public function of TrainingPattern

// constructor
template <typename T>
TP::ImageTrainingPattern<T>::ImageTrainingPattern(const std::string &path,
                                                  const std::size_t numColumns,
                                                  const std::size_t numRows) {
  unsigned char *rawData =
      stbi_load(path.c_str(), &imgWidth_, &imgHeight_, &imgChannels_, 0);
  if (rawData == nullptr) {
    throw std::runtime_error("Failed to load image from: " + path);
  }

  imgData_.reset(rawData);
  calculateIntegralImage();

  regrid(numColumns, numRows);
}

// regrid()
template <typename T>
void TP::ImageTrainingPattern<T>::regrid(const std::size_t numColumns,
                                         const std::size_t numRows) {

  if (numColumns == 0 || numRows == 0) {
    throw std::invalid_argument("Number of columns and rows must be positive.");
  }
  if (!imgData_) {
    throw std::invalid_argument("Image Data not found");
  }

  const float maxLum = 256.0f;
  const std::size_t integral_width = imgWidth_ + 1;

  this->pattern_.clear();
  this->pattern_.reserve(numColumns * numRows);

  const float cell_width = static_cast<float>(imgWidth_) / numColumns;
  const float cell_height = static_cast<float>(imgHeight_) / numRows;

  for (std::size_t r = 0; r < numRows; ++r) {
    for (std::size_t c = 0; c < numColumns; ++c) {
      const int start_x = static_cast<int>(c * cell_width);
      const int start_y = static_cast<int>(r * cell_height);
      int end_x = static_cast<int>((c + 1) * cell_width);
      int end_y = static_cast<int>((r + 1) * cell_height);

      // to prevent going out of bounds of the image during counting
      end_x = std::min(end_x, imgWidth_);
      end_y = std::min(end_y, imgHeight_);

      long long C1 = imgIntegral_[start_y * integral_width + start_x];
      long long C2 = imgIntegral_[start_y * integral_width + end_x];
      long long C3 = imgIntegral_[end_y * integral_width + start_x];
      long long C4 = imgIntegral_[end_y * integral_width + end_x];
      long long total_luminance = C4 - C2 - C3 + C1;
      int pixel_count = (end_x - start_x) * (end_y - start_y);

      float average_luminance = 0.0f;
      if (pixel_count > 0) {
        average_luminance = static_cast<float>(total_luminance) / pixel_count;
      }

      size_t index =
          static_cast<size_t>(average_luminance / maxLum * POINTS<T>.size());
      if (index >= POINTS<T>.size()) {
        index = POINTS<T>.size() - 1;
      }
      this->pattern_.push_back(POINTS<T>[index]);
    }
  }
}
