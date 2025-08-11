#define STB_IMAGE_IMPLEMENTATION

#include "TrainingPattern.hpp"

#include "../stb/stb_image.h"
#include <numeric>
#include <stdexcept>

#include <string>
#include <vector>
#include <cstdint>

//costruttore
TP::TrainingPattern::TrainingPattern(const std::string &path, const std::size_t numColumns, const std::size_t numRows) : path_{path} {
  regrid(numColumns, numRows);
}

const std::vector<int8_t> &TP::TrainingPattern::getPattern() const{
  return pattern_;
}



void TP::TrainingPattern::regrid(const std::size_t numColumns, const std::size_t numRows) {

  const float threshold = 128.0f;

  pattern_.clear();

  int img_width, img_height, img_channels;

  unsigned char *image_data = stbi_load(
    path_.c_str(), &img_width, &img_height, &img_channels, 0);
  
      if (image_data == nullptr) {
    throw std::runtime_error(
        ("Errore: Impossibile caricare l'immagine: " + path_)
            .c_str());
  }
  pattern_.reserve(numColumns * numRows);
  float cell_width = static_cast<float>(img_width) / numColumns;
  float cell_height = static_cast<float>(img_height) / numRows;

  for (std::size_t r = 0; r < numRows; ++r) {
    for (std::size_t c = 0; c < numColumns; ++c) {
      int start_x = static_cast<int>(c * cell_width);
      int start_y = static_cast<int>(r * cell_height);
      int end_x = static_cast<int>((c + 1) * cell_width);
      int end_y = static_cast<int>((r + 1) * cell_height);

      //per evitare che nel conteggio si esca dall'immagine
      end_x = std::min(end_x, img_width);
      end_y = std::min(end_y, img_height);


      long long total_luminance = 0;
      int pixel_count = 0;

      //contiamo pixel per pixel di tutte le celle
      for (int y = start_y; y < end_y; ++y) {
        for (int x = start_x; x < end_x; ++x) {
          unsigned char *pixel = image_data + (y * img_width + x) * img_channels;
          
          int luminance = 0;
          if (img_channels >= 3) { // Se l'immagine ha colori (RGB o RGBA)
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
  stbi_image_free(image_data);
}
