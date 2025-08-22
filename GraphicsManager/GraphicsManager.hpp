#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <functional>
#include <string>

#include "../HopfieldSimulator/math/MathDimension.hpp"
#include <cstdint>
#include <complex>


struct FileDialogHelper {
  std::string dialogKey;
  std::string windowTitle;
  std::string allowedExtensions;
  int maxSelection;
  std::function<void(const std::string &filePath)> onSuccess;
  std::function<void(const std::string &filePath)> onError;
  std::function<void()> onDialogClose;

  FileDialogHelper(std::string key, std::string title, std::string extensions,
                   int max_selection);

  void open();
  void render();
      
};



struct SDL_Window;
struct ImGuiIO;
typedef void *SDL_GLContext;

class GraphicsManager {

private:

  SDL_Window *window_ = nullptr;
  SDL_GLContext glContext_ = nullptr;
  ImGuiIO *io_ = nullptr;
  bool isInitialized_ = false;
  void initialize();

public:
  GraphicsManager();

  ~GraphicsManager();

  bool beginFrame(std::vector<float *> kill = {});
  void endFrame();
  void config();

  void shutdown();
};

template <typename T = int8_t,typename M = double >

struct Comp {
   static constexpr std::array<T, static_cast<unsigned int>(std::pow(2,MD::getMathematicalDimension<T>()))> POINTS = MD::getMathematicalVertex<T>();

  void drawGrid(const std::vector<T> &data, int cols, int rows,
                const char *id_grid,
                const std::function<void(int)> &function = {},
                float size = 300.0f);

  void drawPlot(const std::vector<float> &array);
  void setElementsByFile(const std::string &filePath, int *a, int *b,
                       std::vector<M> *m);
};

template struct Comp<int8_t, double>;
template struct Comp<int,double>;
template struct Comp<std::complex<int8_t>,std::complex<double>>;


#endif // GRAPHICS_MANAGER_H