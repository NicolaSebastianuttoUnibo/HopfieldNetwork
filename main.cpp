
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
#include <functional> 
#include <filesystem> 
#include <fstream> 

template <typename matrix_type>
void  setElementsByFile(const std::string& filePath, int* a, int* b, std::vector<matrix_type>* m){


 std::filesystem::path filePath2(filePath);

        if (!std::filesystem::exists(filePath)) {
            throw std::runtime_error("File di training non trovato: " + filePath2.string());
        }
        std::ifstream inFile(filePath);
        if (!inFile.is_open()) {
            throw std::runtime_error("Impossibile aprire il file per la lettura: " + filePath2.string());
        }

        int numRows, numColumns;
        
        if (!(inFile >> numRows >> numColumns)) {
            throw std::runtime_error("Formato file non valido: impossibile leggere le dimensioni da " + filePath2.string());
        }
        if(numRows!=numColumns){
        throw std::runtime_error("Le dimensioni della matrice non valida");

        }

        std::vector<double> matrix;
        double element;
        
        while (inFile >> element) {
            matrix.push_back(element);
        }

        inFile.close();
 *a=numRows;
 *b=numColumns;
 *m=matrix;
}


int main(int, char **) {
  try {
    GraphicsManager graphics;
    Grid<int8_t> comp;

    HS::HopfieldSimulator<int8_t, double> hs;
    int pixel = 8;
    int pixel_slider = 8;
    float noise = 0.1f;
    size_t index = 0;
    bool running = true;
    bool is_operation_in_progress = false;
    bool trained=false;
    bool showWindow=true;
    std::future<void> training_future;
static char pathBuffer[256] = "trainings/nome_rete";
    float statusTrain=0.0f;
    float statusEvolve=0.0f;
std::vector<float*> kill={&statusTrain, &statusEvolve};

FileDialogHelper fdh{"firstDialog","Open Image(s)",".png,.jpg,.jpeg,.bmp,.gif",25};
fdh.onSuccess = [&](const std::string& filePath) {
        CSP::CoherenceSetPattern<int8_t> cps(filePath, pixel, pixel);
        hs.push_back(cps);
};
fdh.onDialogClose = [&]{showWindow=true;};

FileDialogHelper fdh2{"seccondDialog","Open Training",".training", 1};
fdh2.onSuccess = [&](const std::string& filePath) {
   
     
        // std::filesystem::path filePath2(filePath);

        // if (!std::filesystem::exists(filePath)) {
        //     throw std::runtime_error("File di training non trovato: " + filePath2.string());
        // }
        // std::ifstream inFile(filePath);
        // if (!inFile.is_open()) {
        //     throw std::runtime_error("Impossibile aprire il file per la lettura: " + filePath2.string());
        // }

        // int a, b;
        
        // if (!(inFile >> a >> b)) {
        //     throw std::runtime_error("Formato file non valido: impossibile leggere le dimensioni da " + filePath2.string());
        // }
        // if(a!=b){
        // throw std::runtime_error("Le dimensioni della matrice non valida");

        // }

        // std::vector<double> m;
        // double element;
        
        // while (inFile >> element) {
        //     m.push_back(element);
        // }

        // inFile.close();


        int numRows=0;
        int numColumns=0;
        std::vector<double> matrix;
        setElementsByFile(filePath,&numRows,&numColumns,&matrix);
        pixel=numRows;
        hs.setTraining(numRows,numColumns,matrix);
        trained=true;

};
fdh2.onDialogClose = [&]{showWindow=true;};

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
                ImGui::BeginDisabled(is_operation_in_progress);
        if (ImGui::SliderInt("Pixel Per Row", &pixel_slider, 2,64)) {
         
          if(hs.size()<9){ pixel=pixel_slider;trained=false;    hs.regrid(pixel, pixel);}
        }
        ImGui::EndDisabled();
     
if(hs.size()>8){
        ImGui::BeginDisabled(is_operation_in_progress);
        {
          if (ImGui::Button("Applica Grid")) {
          pixel=pixel_slider;
          trained=false;
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
            showWindow=false;
            fdh.open();
          }
           fdh.render();
        }
          ImGui::EndDisabled();

          ImGui::Separator();

         
            ImGui::BeginDisabled(is_operation_in_progress); 
{
    if (ImGui::Button("Train Hopfield Network")) {
      trained=true;
        is_operation_in_progress = true; // Imposta lo stato a "in corso"
        statusTrain = 0.0f;                   // Resetta la barra di progresso

        // Avvia trainNetwork in un thread separato usando std::async
        training_future = std::async(std::launch::async, [&]() {
            // Questa è una lambda che verrà eseguita nel nuovo thread
            hs.trainNetworkWithPseudoinverse(&statusTrain);
        });
    }
}
ImGui::EndDisabled();

           ImGui::SameLine();
             
      
////////////////////////////////////////////////////////////////////////

    if (ImGui::Button("Save Trained Network")) {
        ImGui::OpenPopup("Save Dialog");
    }
    
    if (ImGui::BeginPopupModal("Save Dialog", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        
        ImGui::Text("Salva la rete Hopfield addestrata.\nSpecifica il percorso di salvataggio (senza estensione).\n");
        ImGui::Separator();

        ImGui::InputText("Percorso di salvataggio", pathBuffer, sizeof(pathBuffer));
        ImGui::Spacing();

    
if (ImGui::Button("Save", ImVec2(120, 0))) {
hs.saveFileTraining(pathBuffer);
    ImGui::CloseCurrentPopup();
}

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ////////////////////////////////////////////////////////////////////////////
           ImGui::SameLine();
        ImGui::BeginDisabled(is_operation_in_progress);
        {
          if (ImGui::Button("Open Training")) {
            showWindow=false;
            fdh2.open();
          }
           fdh2.render();
        }
          ImGui::EndDisabled();
          //////////
           ImGui::SameLine();
     if (ImGui::Button("Stop!")) {
         statusTrain=-1;
          }
           ImGui::SameLine();
ImGui::Text("Caricamento: %.1f/100.0", 100.0f * statusTrain);

      

          ImGui::Separator();
        
        if (hs.size() > 0) {

          ImGui::Text("Stai guardando il %ld° pattern", index + 1);

          if (ImGui::Button("<")) {
            if (index > 0) {
              hs.clear(index);

              index--;
            }
             ImGui::End();
            graphics.endFrame();

            continue;
          }
          ImGui::SameLine();
          if (ImGui::Button(">")) {
            if (index < hs.size() - 1) {
              hs.clear(index);
              index++;
            }
            ImGui::End();
            graphics.endFrame();

            continue;
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
            ImGui::EndDisabled();
              ImGui::End();
            graphics.endFrame();

            continue;
          }
ImGui::EndDisabled();

        
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

          ImGui::BeginDisabled(!trained||is_operation_in_progress);
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