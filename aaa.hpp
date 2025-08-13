#ifndef AAA_H
#define AAA_H

#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/imgui.h"
#include <SDL.h>
#include <SDL_opengl.h> // Se usi OpenGL direttamente, altrimenti ImGui_ImplOpenGL3_Init gestisce le inclusioni base.
#include <stdexcept>
// Include ImGui e i suoi backend.
// Questi di solito non vengono inclusi in un file chiamato "aaa.h"
// a meno che 'aaa' non sia il tuo "GraphicsManager" centrale.

// La tua struct aaa
class aaa { // <-- QUI INIZIA LA DEFINIZIONE DELLA STRUCT
  // Membri della struct, inizializzati a nullptr (C++11 feature)
  ImGuiIO *io = nullptr;
  SDL_Window *window = nullptr;
  SDL_GLContext gl_context =
      nullptr; // Aggiungi anche il contesto OpenGL se lo gestisci qui

public:
  aaa(ImGuiIO *imgui_io_ptr, SDL_Window *window_ptr, SDL_GLContext gl_ctx_ptr)
      : io(imgui_io_ptr), window(window_ptr), gl_context(gl_ctx_ptr) {}

  // Costruttore di default (senza parametri)
  // Se definisci membri con inizializzazione diretta (es. 'io = nullptr;'),
  // puoi omettere questo costruttore se non hai logica aggiuntiva.
  // Il compilatore ne genererà uno che usa quelle inizializzazioni.
  // Se lo definisci tu, assicurati di inizializzare tutto esplicitamente.
  aaa() : io(nullptr), window(nullptr), gl_context(nullptr) {}

  // Metodo di inizializzazione
  void initialize() {
    // --- Inizializzazione SDL ---
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
        0) {
      throw std::invalid_argument(std::string("Errore SDL: ") + SDL_GetError());
    }

    // --- Impostazioni OpenGL ---
    const char *glsl_version = "#version 130"; // compatibile con OpenGL 3.0+
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // --- Crea FINESTRA SDL + CONTESTO OPENGL ---
    // Controlla se 'window' è già stato inizializzato
    if (window == nullptr) { // Crea la finestra solo se non già esistente
      window = SDL_CreateWindow("Rete di Hopfield", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 800, 600,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
      if (!window) {
        SDL_Quit();
        throw std::invalid_argument(std::string("Errore SDL_CreateWindow: ") +
                                    SDL_GetError());
      }
    }

    // Controlla se 'gl_context' è già stato inizializzato
    if (gl_context == nullptr) { // Crea il contesto solo se non già esistente
      gl_context = SDL_GL_CreateContext(window);
      if (!gl_context) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::invalid_argument(
            std::string("Errore SDL_GL_CreateContext: ") + SDL_GetError());
      }
    }

    SDL_GL_SetSwapInterval(1); // abilita VSync

    // --- Inizializza Dear ImGui ---
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // **CORREZIONE:** ImGui::GetIO() restituisce un ImGuiIO&.
    // Se 'io' è un puntatore (ImGuiIO*), devi prenderne l'indirizzo.
    io = &ImGui::GetIO(); // <-- IMPORTANTE: usa '&' per assegnare l'indirizzo

    ImGui::StyleColorsDark();

    // --- Inizializza backend ImGui ---
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void render() {
    ImGui::Render();
    glViewport(0, 0, (*io).DisplaySize.x, (int)(*io).DisplaySize.y);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  void clear() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

}; // <-- QUI FINISCE LA DEFINIZIONE DELLA STRUCT AAA

#endif // AAA_H