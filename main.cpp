#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/imgui.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include "Pattern.h"
#include "TantiPattern.h"
#include <iostream>
#include <stdio.h>
#include <string>

void DrawHopfieldGridAsTable(TantiPattern::TantiPattern &tp,
                             std::vector<int> &data, int &cols, int &rows,
                             float size, bool clickable, const char *unique_id,
                             int index) {
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
  ImVec2 canvas_sz = ImVec2(size, size);
  ImVec2 canvas_p1 =
      ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

  draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

  ImGui::InvisibleButton(unique_id, canvas_sz);
  // is_hovered è la variabile che ci dice se il mouse è sopra il nostro
  // InvisibleButton (l'intera griglia)
  bool is_grid_hovered = ImGui::IsItemHovered();
  // bool is_active = ImGui::IsItemActive(); // Rimosso perché inutilizzato

  const float cell_size_x = canvas_sz.x / cols;
  const float cell_size_y = canvas_sz.y / rows;

  const ImU32 COLOR_BLACK = IM_COL32(50, 50, 50, 255);
  const ImU32 COLOR_WHITE = IM_COL32(255, 255, 255, 255);
  const ImU32 COLOR_HOVER = IM_COL32(100, 100, 255, 150);

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      int index = row * cols + col;
      if (static_cast<std::vector<int>::size_type>(index) >= data.size())
        continue;

      float x0 = canvas_p0.x + col * cell_size_x;
      float y0 = canvas_p0.y + row * cell_size_y;
      float x1 = x0 + cell_size_x;
      float y1 = y0 + cell_size_y;

      ImU32 cell_color = (data[index] == 1) ? COLOR_WHITE : COLOR_BLACK;
      draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), cell_color);
    }
  }

  if (clickable && is_grid_hovered) {
    ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetMousePos().x - canvas_p0.x,
                                        ImGui::GetMousePos().y - canvas_p0.y);
    int hovered_col = static_cast<int>(mouse_pos_in_canvas.x / cell_size_x);
    int hovered_row = static_cast<int>(mouse_pos_in_canvas.y / cell_size_y);

    if (hovered_col >= 0 && hovered_col < cols && hovered_row >= 0 &&
        hovered_row < rows) {
      int hovered_index = hovered_row * cols + hovered_col;
      if (static_cast<std::vector<int>::size_type>(hovered_index) <
          data.size()) {

        float hx0 = canvas_p0.x + hovered_col * cell_size_x;
        float hy0 = canvas_p0.y + hovered_row * cell_size_y;
        float hx1 = hx0 + cell_size_x;
        float hy1 = hy0 + cell_size_y;
        draw_list->AddRectFilled(ImVec2(hx0, hy0), ImVec2(hx1, hy1),
                                 COLOR_HOVER);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
          tp.corrompiPixel(index, hovered_index);
        }
      }
    }
  }
}

int main(int, char **) {

  // initializza SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Errore SDL: %s\n", SDL_GetError());
    return -1;
  }
  // OpenGL setiings
  const char *glsl_version = "#version 130"; // compatibile con OpenGL 3.0+
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // CREA  FINESTRA SDL + CONTESTO OPENGL
  SDL_Window *window = SDL_CreateWindow(
      "Rete di Hopfield", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
      600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1); // abilita VSync

  // inizializza Dear  IMGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  // inizializza backend ImGui
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////

  // variabili DEMO
  TantiPattern::TantiPattern allpatterns;

  int w = 2;
  int h = 2;
  bool evolvable = false;
  // --- DATI PER LA GRIGLIA ---
  const int grid_cols = 3;
  const int grid_rows = 3;
  int hopfield_state[grid_cols * grid_rows] = {1, 0, 0, 1, 0, 1, 0, 1, 0};

  // Loop principale:
  bool done = false;

  while (!done) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT) {
        done = true;
      }
    }
    /// inizio frame ImGUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    /// finestra GUI
    ImGui::Begin("Controllo rete di Hopfield");
    ImGui::Text("Parametri:");
    if (ImGui::SliderInt("Larghezza", &w, 2, 128)) {
      evolvable = false;
      allpatterns.applicapixelatura(w, h);
    }
    if (ImGui::SliderInt("Altezza", &h, 2, 128)) {
      evolvable = false;

      allpatterns.applicapixelatura(w, h);
    }
    ImGui::Separator();
    if (ImGui::Button("Open File Dialog")) {
      evolvable = false;
      IGFD::FileDialogConfig config;
      config.path = ".";
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                              ".png, .jpg, .jpeg, .bmp, .gif",
                                              config);
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
      evolvable = false;

      if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
        std::string filePathName =
            ImGuiFileDialog::Instance()->GetFilePathName();
        std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
        Pattern::Pattern pattern(filePathName, w, h);
        allpatterns.push_back(pattern);
      }
      // close
      ImGuiFileDialog::Instance()->Close();
    }
    ImGui::Separator();

    if (ImGui::Button("Save Images")) {
      allpatterns.creaMatrice();
      evolvable = true;
    }

    // --- INIZIO CODICE GRIGLIA ---
    ImGui::Separator(); // Aggiunge una linea di separazione per pulizia

    ImGui::End();

    for (int i = 0; i < allpatterns.size(); i++) {

      // All'interno del tuo ciclo, per ogni pattern 'i'
      ImGui::Begin(("Immagine numero " + std::to_string(i + 1)).c_str());

      ImGui::Text("La griglia della immagine e': %dx%d", w, h);
      ImGui::Separator();

      ImGui::BeginGroup();
      if (ImGui::Button("Elimina")) {
        allpatterns.removePattern(i);
        ImGui::EndGroup();
        ImGui::End();
        break;
      }

      ImGui::Text("Pattern pixelato originale:");
      DrawHopfieldGridAsTable(
          allpatterns, allpatterns.getPatterns()[i].getPatternQ(), w, h, 300.0f,
          false, ("grid_originale" + std::to_string(i)).c_str(), i);
      ImGui::EndGroup();

      ImGui::SameLine();

      ImGui::BeginGroup();
      ImGui::Text("Pattern pixelato corrotto:");
      DrawHopfieldGridAsTable(
          allpatterns, allpatterns.getPatterns()[i].getPatternC(), w, h, 300.0f,
          true, ("grid_corrotta" + std::to_string(i)).c_str(), i);
      if (ImGui::Button("Corrompi")) {
        allpatterns.corrompi(i);
      }

      ImGui::EndGroup();

      ImGui::BeginGroup();
      ImGui::Text("Pattern pixelato evoluzione:");
      DrawHopfieldGridAsTable(
          allpatterns, allpatterns.getPatterns()[i].getPatternD(), w, h, 300.0f,
          false, ("grid_evoluzione" + std::to_string(i)).c_str(), i);
      if (ImGui::Button("Evoluzione") && evolvable) {
        allpatterns.disicorrompi(i);
      }

      ImGui::EndGroup();

      ImGui::End();
    }

    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////////

  // CleanUP
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}