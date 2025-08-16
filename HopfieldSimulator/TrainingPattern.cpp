
#define STB_IMAGE_IMPLEMENTATION

#include "TrainingPattern.hpp"

#include "../stb/stb_image.h"

#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <cstdint>
#include <cmath>



void TP::StbiImageDeleter::operator()(unsigned char* data) const {
    if (data) {
        stbi_image_free(data);
    }
}

//costruttore
template <typename T> 
TP::TrainingPattern<T>::TrainingPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows) : path_{path} {
  
   unsigned char* rawData = stbi_load(
        path.c_str(), &imgWidth_, &imgHeight_, &imgChannels_, 0
    );
if (rawData == nullptr) {
        throw std::runtime_error("Failed to load image from: " + path);
    }

   imageData_.reset(rawData);
       calculateIntegralImage();

         regrid(numColumns, numRows);
 

}

template <typename T>
void TP::TrainingPattern<T>::calculateIntegralImage() {
    const std::size_t integral_width = imgWidth_ + 1;
    const std::size_t integral_height = imgHeight_ + 1;
    integralImage_.assign(integral_width * integral_height, 0);

    for (int y = 0; y < imgHeight_; ++y) {
        for (int x = 0; x < imgWidth_; ++x) {
            unsigned char* pixel = imageData_.get() + (y * imgWidth_ + x) * imgChannels_;
            
            int luminance = 0;
            if (imgChannels_ >= 3) { 
                luminance = (pixel[0] + pixel[1] + pixel[2]) / 3;
            } else { 
                luminance = pixel[0];
            }

            
            integralImage_[(y + 1) * integral_width + (x + 1)] = 
                luminance + 
                integralImage_[y * integral_width + (x + 1)] + 
                integralImage_[(y + 1) * integral_width + x] - 
                integralImage_[y * integral_width + x];
        }
    }
}



template <typename T> 
const std::vector<T> &TP::TrainingPattern<T>::getPattern() const{
  return pattern_;
}


template <typename T> 
void TP::TrainingPattern<T>::regrid(const std::size_t numColumns, const std::size_t numRows) {

    if (numColumns == 0 || numRows == 0) {
        throw std::invalid_argument("Number of columns and rows must be positive.");
    }
    if (!imageData_) {
        throw std::runtime_error("Image data is not loaded, cannot regrid.");
    }



  
  const float maxLum = 256.0f;
  const float threshold = 128.0f;
  
    const std::size_t integral_width = imgWidth_ + 1;

   
    pattern_.clear();
    pattern_.reserve(numColumns * numRows);

  const float cell_width = static_cast<float>(imgWidth_) / numColumns;
  const float cell_height = static_cast<float>(imgHeight_) / numRows;

  for (std::size_t r = 0; r < numRows; ++r) {
    for (std::size_t c = 0; c < numColumns; ++c) {
      const int start_x = static_cast<int>(c * cell_width);
      const int start_y = static_cast<int>(r * cell_height);
       int end_x = static_cast<int>((c + 1) * cell_width);
       int end_y = static_cast<int>((r + 1) * cell_height);

      //per evitare che nel conteggio si esca dall'immagine
      end_x = std::min(end_x, imgWidth_);
      end_y = std::min(end_y, imgHeight_);

            long long C1 = integralImage_[start_y * integral_width + start_x];
            long long C2 = integralImage_[start_y * integral_width + end_x];   
            long long C3 = integralImage_[end_y * integral_width + start_x];        
            long long C4 = integralImage_[end_y * integral_width + end_x];   
            long long total_luminance = C4 - C2 - C3 + C1;
            int pixel_count = (end_x - start_x) * (end_y - start_y);

    

      float average_luminance = 0.0f;
      if (pixel_count > 0) {
        average_luminance = static_cast<float>(total_luminance) / pixel_count;
      }

      const int N=POINTS.size();
for(int i=0;i<N;i++){
  if(average_luminance>=i*maxLum/N&&average_luminance<(i+1)*maxLum/N){
    pattern_.push_back(POINTS[i]);
  }
}
      // if (average_luminance > threshold) {
      //   pattern_.push_back(+1); // Chiaro
      // } else {
      //   pattern_.push_back(-1); // Scuro
      // }
    }
  }

}
