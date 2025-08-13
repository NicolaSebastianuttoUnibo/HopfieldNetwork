
#define STB_IMAGE_IMPLEMENTATION

#include "TrainingPattern.hpp"

#include "../stb/stb_image.h"

#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>



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
         regrid(numColumns, numRows);
 

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


  const float threshold = 128.0f;

   
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


      long long total_luminance = 0;
      int pixel_count = 0;

      //contiamo pixel per pixel di tutte le celle
      for (int y = start_y; y < end_y; ++y) {
        for (int x = start_x; x < end_x; ++x) {
          unsigned char *pixel = imageData_.get() + (y * imgWidth_ + x) * imgChannels_;
          
          int luminance = 0;
          if (imgChannels_ >= 3) { // Se l'immagine ha colori (RGB o RGBA)
            luminance = (pixel[0] + pixel[1] + pixel[2]) / 3;
          } else { // Se l'immagine è già in scala di grigi
            luminance = pixel[0];
          }

          total_luminance += luminance;
          pixel_count++;
        }
      }
      //calcoliamo la luminosità media
      float average_luminance = 0.0f;
      if (pixel_count > 0) {
        average_luminance = static_cast<float>(total_luminance) / pixel_count;
      }

      if (average_luminance > threshold) {
        pattern_.push_back(+1); // Chiaro
      } else {
        pattern_.push_back(-1); // Scuro
      }
    }
  }

}
