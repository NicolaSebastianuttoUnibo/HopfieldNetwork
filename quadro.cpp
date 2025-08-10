#define STB_IMAGE_IMPLEMENTATION

#include "quadro.h"
#include "stb/stb_image.h"
#include <iostream>
#include <numeric>
#include <stdexcept> // Per std::runtime_error
#include <string>
#include <vector>
Quadro::Quadro::Quadro(std::string &str, int &a, int &b) : path_{str} {

  Quadro::Quadro::updatePattern(a, b);
}

std::vector<int> &Quadro::Quadro::getPattern() {
  return Quadro::Quadro::pattern_;
}

void Quadro::Quadro::updatePattern(int &a, int &b) {
  /// questa funzione prende il path della immagine e la converte in "pixel"
  float threshold = 128.0f;
  Quadro::Quadro::pattern_.clear();
  int img_width, img_height, img_channels;
  unsigned char *image_data = stbi_load(
      Quadro::Quadro::path_.c_str(), &img_width, &img_height, &img_channels, 0);
  if (image_data == nullptr) {
    throw std::runtime_error(
        ("Errore: Impossibile caricare l'immagine: " + Quadro::Quadro::path_)
            .c_str());
  }
  Quadro::Quadro::pattern_.reserve(a * b);
  float cell_width = static_cast<float>(img_width) / a;
  float cell_height = static_cast<float>(img_height) / b;

  for (int r = 0; r < b; ++r) {
    for (int c = 0; c < a; ++c) {
      int start_x = static_cast<int>(c * cell_width);
      int start_y = static_cast<int>(r * cell_height);
      int end_x = static_cast<int>((c + 1) * cell_width);
      int end_y = static_cast<int>((r + 1) * cell_height);

      end_x = std::min(end_x, img_width);
      end_y = std::min(end_y, img_height);

      long long total_luminance = 0;
      int pixel_count = 0;

      for (int y = start_y; y < end_y; ++y) {
        for (int x = start_x; x < end_x; ++x) {
          unsigned char *pixel =
              image_data + (y * img_width + x) * img_channels;
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

      float average_luminance = 0.0f;
      if (pixel_count > 0) {
        average_luminance = static_cast<float>(total_luminance) / pixel_count;
      }

      if (average_luminance > threshold) {
        Quadro::Quadro::pattern_.push_back(1); // Chiaro
      } else {
        Quadro::Quadro::pattern_.push_back(
            0); // Scuro (o -1 se la tua rete di Hopfield usa -1)
      }
    }
  }
  stbi_image_free(image_data);
}
