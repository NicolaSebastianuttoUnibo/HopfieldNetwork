
#include "GraphicsManager.hpp"
#include "HopfieldSimulator/CoherenceSetPattern.hpp"
#include "HopfieldSimulator/HopfieldNetwork.hpp"
#include "HopfieldSimulator/HopfieldSimulator.hpp"
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <future>
#include <chrono>   



int main(int, char **) {
  try {
    GraphicsManager graphics;
    Components<int8_t> comp;

    HS::HopfieldSimulator<int8_t, double> hs;
    int pixel = 8;
    int pixel_slider = 8;
    float noise = 0.1f;
    size_t index = 0;
    bool running = true;
    bool is_operation_in_progress = false;
    bool showWindow=true;
    std::future<void> training_future;


    float statusTrain=0.0f;
    float statusEvolve=0.0f;
std::vector<float*> kill={&statusTrain, &statusEvolve};

    while (running) {
      running = graphics.beginFrame(kill);





      
      if (running) {
if (is_operation_in_progress) {
    if (training_future.valid() && training_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        training_future.get();
        is_operation_in_progress = false;
        statusTrain = 1.0f;
        statusEvolve= 1.0f;

    }
}




        graphics.config();
        ImGui::SetNextWindowCollapsed(!showWindow, ImGuiCond_Always); 

        ImGui::Begin("Rete di Hopfield");

        ImGui::Text("Controllo rete di Hopfield");
        ImGui::Text("Parametri:");
        if (ImGui::SliderInt("Pixel Per Row", &pixel_slider, 2,128)) {
         
          if(hs.size()<9){ pixel=pixel_slider;    hs.regrid(pixel, pixel);}
        }
     
if(hs.size()>8){
        ImGui::BeginDisabled(is_operation_in_progress);
        {
          if (ImGui::Button("Applica Grid")) {
          pixel=pixel_slider;
            hs.regrid(pixel, pixel);
          }
        }
        ImGui::EndDisabled();
      }

        ImGui::SliderFloat("Noise", &noise, 0.0f,
                           1.0f);
        ImGui::Separator();


        ImGui::BeginDisabled(is_operation_in_progress);
        {
          if (ImGui::Button("Open Images")) {
            IGFD::FileDialogConfig config;
            showWindow=false;
            config.path = ".";
            config.countSelectionMax = 25;
            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseFileDlgKey_Multiple", "Choose Image(s)",
                ".png,.jpg,.jpeg,.bmp,.gif", config);
          }

      
          
 if (ImGuiFileDialog::Instance()->IsOpened("ChooseFileDlgKey_Multiple")) {
          const ImGuiViewport *viewport = ImGui::GetMainViewport();
          ImGui::SetNextWindowPos(viewport->WorkPos);
          ImGui::SetNextWindowSize(viewport->WorkSize);
        }





        
          if (ImGuiFileDialog::Instance()->Display(
                  "ChooseFileDlgKey_Multiple")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
              std::map<std::string, std::string> selections =
                  ImGuiFileDialog::Instance()->GetSelection();
              for (const auto &[fileName, filePath] : selections) {
                try {
                  CSP::CoherenceSetPattern<int8_t> cps(filePath, pixel, pixel);
                  hs.push_back(cps);
                } catch (const std::exception &e) {
                      throw std::logic_error("Errore nel carcicare "+filePath+": " + std::string(e.what()));

                  
                  
                }
              }
            }
            ImGuiFileDialog::Instance()->Close();
            showWindow=true;
          }
        }
          ImGui::EndDisabled();

          ImGui::Separator();

         
            ImGui::BeginDisabled(is_operation_in_progress); 
{
    if (ImGui::Button("Train Hopfield Network")) {
        is_operation_in_progress = true; // Imposta lo stato a "in corso"
        statusTrain = 0.0f;                   // Resetta la barra di progresso

        // Avvia trainNetwork in un thread separato usando std::async
        training_future = std::async(std::launch::async, [&]() {
            // Questa è una lambda che verrà eseguita nel nuovo thread
            hs.trainNetwork(&statusTrain);
        });
    }
}
ImGui::EndDisabled();
           ImGui::SameLine();

ImGui::Text("Caricamento: %.1f/100.0", 100.0f * statusTrain);
           ImGui::SameLine();

           if (ImGui::Button("Stop!")) {
         statusTrain=-1;
          }

          ImGui::Separator();
        
        bool imremoving = false;
        if (hs.size() > 0) {

          ImGui::Text("Stai guardando il %ld° pattern", index + 1);

          if (ImGui::Button("<")) {
            if (index > 0) {
              hs.clear(index);

              index--;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button(">")) {
            if (index < hs.size() - 1) {
              hs.clear(index);
              index++;
            }
          }

          ImGui::Separator();

          ImGui::BeginGroup();
         ImGui::BeginDisabled(is_operation_in_progress); 
          if (ImGui::Button("Elimina")) {
            hs.removePattern(index);
            if (hs.size() == 0) {
              index = -1;
            } else if (index > 0) {
              index--;
            }
            index = 0;
            ImGui::EndGroup();
            imremoving = true;
          }
ImGui::EndDisabled();

        }
        if (hs.size() > 0 && !imremoving) {
          const auto &current_pattern_container = hs.getPatterns()[index];
          ImGui::Text("Pattern pixelato originale:");
          const std::vector<int8_t> &training_data =
              current_pattern_container.getTrainingPatternVector();
          comp.drawGrid(training_data, pixel, pixel, "training_pattern");
          ImGui::EndGroup();

          ImGui::SameLine();
ImGui::BeginDisabled(is_operation_in_progress);
          ImGui::BeginGroup();
          if (ImGui::Button("Corrompi")) {
            hs.corruptPattern(index, noise);
          }
ImGui::EndDisabled();

          ImGui::Text("Pattern pixelato corrotto:");
          const std::vector<int8_t> &noisy_data =
              current_pattern_container.getNoisyPatternVector();

          comp.drawGrid(noisy_data, pixel, pixel, "noisy_pattern",
                        [&](int hovered_index) {
                          hs.flipPixelOnPattern(index, hovered_index);
                        });
          ImGui::EndGroup();

          ImGui::SameLine();

          ImGui::BeginGroup();

          ImGui::BeginDisabled(is_operation_in_progress);
          {
            if (ImGui::Button("Evolvi")) {
statusEvolve = 0.0f;               // hs.resolvePattern(index);
              training_future = std::async(std::launch::async, [&]() {
            // Questa è una lambda che verrà eseguita nel nuovo thread
            hs.resolvePattern(index,  &statusEvolve);
        });
            }
          }
          ImGui::EndDisabled();

ImGui::SameLine();
ImGui::Text("Caricamento: %.1f/100.0", 100.0f * statusEvolve);
  ImGui::SameLine();

           if (ImGui::Button("Stop")) {
         statusEvolve=-1;
          }
          ImGui::Text("Pattern pixelato evoluzione:");
          const std::vector<int8_t> &evolving_data =
              current_pattern_container.getEvolvingPatternVector();

          comp.drawGrid(evolving_data, pixel, pixel, "evolving_pattern");
ImGui::EndGroup();

ImGui::SameLine();
          ImGui::BeginGroup();

const std::vector<float>& energy= current_pattern_container.getEnergy();
    

// Se non ci sono dati, mostra un messaggio
if (energy.size() == 0) {
    ImGui::Text("Nessun dato di energia disponibile.");
} else {
  ImGui::Text("Andamento dell'energia");
    // --- Parametri per ImGui::PlotLines ---
    const char* graph_label = "label";
    // Il puntatore è lo stesso, ma l'offset farà iniziare ImGui da un punto diverso
    const float* values = energy.data(); 

    // Calcola min/max SOLO sulla finestra di dati visibile. Questo è un enorme guadagno!
    float min_val = 0.0f;
    float max_val = 0.0f;
    // Prendi un iteratore che punta all'inizio dei dati che ci interessano
    auto start_it = energy.begin();
    auto end_it = energy.end();

    if (start_it != end_it) {
        min_val = *std::min_element(start_it, end_it);
        max_val = *std::max_element(start_it, end_it);

        float buffer = (max_val - min_val) * 0.1f;
        min_val -= buffer;
        max_val += buffer;
    }

    // Disegna il grafico a linee
    ImGui::PlotLines(
        graph_label,
        values,
        energy.size(),       // Numero totale di elementi nel vettore originale
        0,      // Offset di partenza! ImGui inizierà da qui.
        NULL,
        min_val,
        max_val,
        ImVec2(300, 300)      // Larghezza 0 = riempi, altezza 150 pixel
    );
}


ImGui::EndGroup();

          
        }

        ImGui::End();
      }


      
      graphics.endFrame();

    }

  } catch (const std::runtime_error &e) {
    throw std::logic_error("Errore critico: " + std::string(e.what()));

    return 1;
  }

  return 0;
}