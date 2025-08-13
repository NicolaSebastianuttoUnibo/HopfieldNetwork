
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
    int w = 8;
    int h = 8;
    int w_slider = 8;
    int h_slider = 8;
    float noise = 0.1f;
    size_t index = 0;
    bool running = true;
    bool is_operation_in_progress = false;
    bool showWindow=true;
    std::future<void> training_future;


    float statusTrain=0.0f;
    float statusEvolve=0.0f;


    while (running) {
      running = graphics.beginFrame();

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
        if (ImGui::SliderInt("Larghezza", &w_slider, 2, 32)) {
        }
        if (ImGui::SliderInt("Altezza", &h_slider, 2, 32)) {
        }

        ImGui::BeginDisabled(is_operation_in_progress);
        {
          if (ImGui::Button("Applica Grid")) {
            w = w_slider;
            h = h_slider;
            hs.regrid(w, h);
          }
        }
        ImGui::EndDisabled();

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
                  CSP::CoherenceSetPattern<int8_t> cps(filePath, w, h);
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

         
            ImGui::BeginDisabled(is_operation_in_progress); // Questo ora disabiliterà il bottone mentre il training è in corso
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

          

          ImGui::Separator();
        
        bool imremoving = false;
        if (hs.size() > 0) {

          ImGui::Text("Stai guardando il %ld° pattern", index + 1);

          if (ImGui::Button("<")) {
            if (index > 0) {
              index--;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button(">")) {
            if (index < hs.size() - 1) {
              index++;
            }
          }

          ImGui::Separator();

          ImGui::BeginGroup();
          
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
        }
        if (hs.size() > 0 && !imremoving) {
          const auto &current_pattern_container = hs.getPatterns()[index];
          ImGui::Text("Pattern pixelato originale:");
          const std::vector<int8_t> &training_data =
              current_pattern_container.getTrainingPatternVector();
          comp.drawGrid(training_data, w, h, "training_pattern");
          ImGui::EndGroup();

          ImGui::SameLine();

          ImGui::BeginGroup();
          if (ImGui::Button("Corrompi")) {
            hs.corruptPattern(index, noise);
          }
          ImGui::Text("Pattern pixelato corrotto:");
          const std::vector<int8_t> &noisy_data =
              current_pattern_container.getNoisyPatternVector();

          comp.drawGrid(noisy_data, w, h, "noisy_pattern",
                        [&](int hovered_index) {
                          hs.flipPixelOnPattern(index, hovered_index);
                        });
          ImGui::EndGroup();

          ImGui::SameLine();

          ImGui::BeginGroup();

          ImGui::BeginDisabled(is_operation_in_progress);
          {
            if (ImGui::Button("Evolvi")) {
              // hs.resolvePattern(index);
              training_future = std::async(std::launch::async, [&]() {
            // Questa è una lambda che verrà eseguita nel nuovo thread
            hs.resolvePattern(index,  &statusEvolve);
        });
            }
          }
          ImGui::EndDisabled();

ImGui::SameLine();
ImGui::Text("Caricamento: %.1f/100.0", 100.0f * statusEvolve);

          ImGui::Text("Pattern pixelato evoluzione:");
          const std::vector<int8_t> &evolving_data =
              current_pattern_container.getEvolvingPatternVector();

          comp.drawGrid(evolving_data, w, h, "evolving_pattern");
ImGui::EndGroup();

ImGui::SameLine();
          ImGui::BeginGroup();

const std::vector<float>& energy= current_pattern_container.getEnergy();
          
 if (energy.empty()) {
        ImGui::Text("Nessun dato di energia disponibile.");
    }

    // --- Parametri per ImGui::PlotLines ---
    const char* graph_label = "Andamento Energia"; // Etichetta che apparirà sul grafico
    const float* values = energy.data();           // Puntatore ai dati (necessario per ImGui)
    int values_count = static_cast<int>(energy.size()); // Numero di elementi nel vettore

    // Puoi calcolare min/max per impostare un range automatico del grafico
    // (Opzionale, se non specifichi min_value/max_value, ImGui li calcola automaticamente)
    float min_val ;
    float max_val ;
    if (values_count > 0) {
        min_val = *std::min_element(energy.begin(), energy.end());
        max_val = *std::max_element(energy.begin(), energy.end());
        // Aggiungi un piccolo buffer per la visualizzazione
        float buffer = (max_val - min_val) * 0.1f;
        min_val -= buffer;
        max_val += buffer;
    }



    // Disegna il grafico a linee
    ImGui::PlotLines(
        graph_label,      // Etichetta del grafico
        values,           // Puntatore ai dati
        values_count,     // Numero di elementi
        0,                // Offset (inizia dal primo elemento)
        NULL,             // Overlay text (testo aggiuntivo sul grafico, qui nessuno)
        min_val,          // Valore minimo del range Y (ImGui lo calcola se 0.0f)
        max_val,          // Valore massimo del range Y (ImGui lo calcola se 0.0f)
        ImVec2(300, 300)      // Dimensione del grafico (larghezza 0 = riempi, altezza 80 pixel)
    );

    



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