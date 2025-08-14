#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/imgui.h"
#include <functional>
#include <stdexcept>
#include <string>
#include "ImGuiFileDialog/ImGuiFileDialog.h"

struct FileDialogHelper {
    std::string dialogKey;      
    std::string windowTitle;       
    std::string allowedExtensions;
    int maxSelection;             

     std::function<void(const std::string& filePath)> onSuccess;
    std::function<void(const std::string& filePath)> onError;
     std::function<void()> onDialogClose;

    FileDialogHelper(
        std::string key, 
        std::string title, 
        std::string extensions, 
        int max_selection)
        : dialogKey(std::move(key)),
          windowTitle(std::move(title)),
          allowedExtensions(std::move(extensions)),
          maxSelection(max_selection) {}


    void open() {
        IGFD::FileDialogConfig config;
        config.path = "."; 
        config.countSelectionMax = maxSelection;
        ImGuiFileDialog::Instance()->OpenDialog(dialogKey, windowTitle, allowedExtensions.c_str(), config);
    }

    void render() {
        if (ImGuiFileDialog::Instance()->IsOpened(dialogKey)) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
        }

        if (ImGuiFileDialog::Instance()->Display(dialogKey)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::map<std::string, std::string> selections = ImGuiFileDialog::Instance()->GetSelection();
                for (const auto& [fileName, filePath] : selections) {
                    try {
                         if (onSuccess) {
                             onSuccess(filePath);
                         }
                    } catch (const std::exception& e) {
                        throw std::logic_error("Errore nel carcicare "+filePath+": " + std::string(e.what()));
                    }
                }
            }

            ImGuiFileDialog::Instance()->Close();

            if (onDialogClose) {
                onDialogClose();
            }
        }
    }
};



struct SDL_Window;
struct ImGuiIO;
typedef void *SDL_GLContext;

class GraphicsManager {

private:
  // Membri privati per incapsulare lo stato.
  // Il prefisso 'm_' è una convenzione comune per i membri di una classe.
  SDL_Window *window_ = nullptr;
  SDL_GLContext glContext_ = nullptr;
  ImGuiIO *io_ = nullptr;
  bool isInitialized_ = false;
  void initialize();

public:
  GraphicsManager();

  ~GraphicsManager();

  bool beginFrame(std::vector<float*> kill={});
  void endFrame();
  void config();

  void shutdown();
};

template <typename T = int8_t>

struct Grid {

  void drawGrid(const std::vector<T> &data, int cols, int rows,
                const char *id_grid,
                const std::function<void(int)> &function = {},
                float size = 300.0f);



    };

template struct Grid<int8_t>;
template struct Grid<int>;

#endif // GRAPHICS_MANAGER_H