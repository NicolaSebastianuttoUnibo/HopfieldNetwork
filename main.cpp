#include "HopfieldSimulator/HopfieldNetwork.hpp"
#include "GraphicsManager.hpp"
#include "HopfieldSimulator/HopfieldSimulator.hpp"
#include "HopfieldSimulator/CoherenceSetPattern.hpp"
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include <iostream>
#include <stdio.h>
#include <string>

void DrawHopfieldGridAsTable(HS::HopfieldSimulator<int8_t, double> &hopSim,
                             const std::vector<int8_t> &data, // <-- AGGIUNGI CONST QUI!
                             int cols, int rows,
                             float size, bool clickable, const char *unique_id,
                             int myindex) {
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
          hopSim.flipPixelOnPattern(myindex, hovered_index);
        }
      }
    }
  }

}


int main(int, char**) {
    try {
        GraphicsManager graphics; 
        
        HS::HopfieldSimulator<int8_t,double> hs;
        int w=8;
        int h=8;
         int w_slider=8;
        int h_slider=8;
        float noise=0.1f;
        size_t index=0;
        bool running = true;
        bool loadingfile = false;
        bool is_operation_in_progress = false;



        while (running) {
            running = graphics.beginFrame();

            if (running) {

 const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoResize;     // La tua richiesta principale
        window_flags |= ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoSavedSettings;




                ImGui::Begin("Rete di Hopfield");
                ImGui::Text("Controllo rete di Hopfield");
                ImGui::Text("Parametri:");
                if (ImGui::SliderInt("Larghezza", &w_slider, 2, 64)) { }
                if (ImGui::SliderInt("Altezza", &h_slider, 2, 64)) { }


                 ImGui::BeginDisabled(is_operation_in_progress);
                {
if (ImGui::Button("Applica Grid")) {w=w_slider;h=h_slider; hs.regrid(w, h);}
                }
 ImGui::EndDisabled();



                ImGui::SliderFloat("Noise", &noise, 0.0f, 1.0f); // Aggiunto .0f per coerenza

                ImGui::Separator();

              // All'interno del tuo loop di rendering di ImGui
                 ImGui::BeginDisabled(is_operation_in_progress);
                 {
if (ImGui::Button("Open Images")) {
    IGFD::FileDialogConfig config;
    config.path = ".";
    
    // 1. ABILITA LA SELEZIONE MULTIPLA

    config.countSelectionMax = 25; 

    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey_Multiple",
                                            "Choose Image(s)",
                                            ".png,.jpg,.jpeg,.bmp,.gif",
                                            config);
}

// Controlla il dialogo per la selezione multipla
if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey_Multiple")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
        
    //  loadingfile=true;
        std::map<std::string, std::string> selections = ImGuiFileDialog::Instance()->GetSelection();

      
        for (const auto& [fileName, filePath] : selections) {

            try {
                CSP::CoherenceSetPattern<int8_t> cps(filePath, w, h);
                hs.push_back(cps);
                // loadingfile=false;
            } catch (const std::exception& e) {
                 std::cerr << "Error loading " << filePath << ": " << e.what() << std::endl;
            }
        }
    }
    


    // Chiudi il dialogo
    ImGuiFileDialog::Instance()->Close();
}
                ImGui::Separator();
                                 ImGui::BeginDisabled(is_operation_in_progress);
                                 {
                if (ImGui::Button("Train Hopfield Network")) {
                    hs.trainNetwork();
                } 
              }
                ImGui::EndDisabled();

                ImGui::Separator(); 
  }
 ImGui::EndDisabled();

            
               
               bool imremoving=false;
                if(hs.size()>0&&!loadingfile) {

                  

                    ImGui::Text("Stai guardando il %ld° pattern", index+1);
                    if (ImGui::Button("<")) {
                    
                    
                if(index>0){index--;}
                }
                    ImGui::SameLine();
              if (ImGui::Button(">")) {
                      if(index<hs.size()-1){index++;}
                }
                    ImGui::Separator();


                               


                    ImGui::BeginGroup();


                    if (ImGui::Button("Elimina")) {
                        hs.removePattern(index);
                        if(hs.size()==0){index=-1;}
                        else if(index>0){index--;}
                        index=0;
                        ImGui::EndGroup();
                        imremoving=true;
                       
                    }

                     
                  }

                                if(hs.size()>0&&!imremoving) {

                  const auto& current_pattern_container = hs.getPatterns()[index];

                    ImGui::Text("Pattern pixelato originale:");
                    const std::vector<int8_t>& training_data = current_pattern_container.getTrainingPatternVector();

                    DrawHopfieldGridAsTable(hs, training_data, w, h, 300.0f, false, "grid_originale", index);
                  
                  
                    ImGui::EndGroup();


                    ImGui::SameLine();

                    ImGui::BeginGroup();
                    if (ImGui::Button("Corrompi")) {
                        hs.corruptPattern(index, noise);
                    }
                    ImGui::Text("Pattern pixelato corrotto:");
                    const std::vector<int8_t>& noisy_data = current_pattern_container.getNoisyPatternVector();

                    DrawHopfieldGridAsTable(hs, noisy_data, w, h, 300.0f, true, "grid_corrotto", index);
                    ImGui::EndGroup();
                    
                    ImGui::SameLine();

                    ImGui::BeginGroup();
                     ImGui::BeginDisabled(is_operation_in_progress);
{
                     if (ImGui::Button("Evolvi")) {
                        hs.resolvePattern(index);
                    }
                  }
                   ImGui::EndDisabled();

                    ImGui::Text("Pattern pixelato evoluzione:");
                    const std::vector<int8_t>& evolving_data = current_pattern_container.getEvolvingPatternVector();

                    DrawHopfieldGridAsTable(hs, evolving_data, w, h, 300.0f, false, "grid_evoluzione", index);
                    ImGui::EndGroup();
                    ImGui::Separator();
              
              
                    }

                   ImGui::End(); 

            }

            graphics.endFrame();
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Errore critico: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}