#include "GraphicsManager.hpp"

// Includi le implementazioni complete solo nel file .cpp
#include "ImGuiFileDialog/ImGuiFileDialog.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/imgui.h"

#include <SDL.h>
#include <SDL_opengl.h>

GraphicsManager::GraphicsManager() { initialize(); }

GraphicsManager::~GraphicsManager() { shutdown(); }

void GraphicsManager::initialize() {
  if (isInitialized_) {
    return;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    throw std::runtime_error(std::string("Errore SDL: ") + SDL_GetError());
  }

  const char *glsl_version = "#version 130"; // OpenGL 3.0+
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // --- Creazione Finestra SDL e Contesto OpenGL ---
  window_ = SDL_CreateWindow("Rete di Hopfield", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, 1280, 720,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (!window_) {
    throw std::runtime_error(std::string("Errore SDL_CreateWindow: ") +
                             SDL_GetError());
  }

  glContext_ = SDL_GL_CreateContext(window_);
  if (!glContext_) {
    throw std::runtime_error(std::string("Errore SDL_GL_CreateContext: ") +
                             SDL_GetError());
  }
  SDL_GL_MakeCurrent(window_, glContext_);
  SDL_GL_SetSwapInterval(1); // Abilita VSync

  // --- Inizializzazione Dear ImGui ---
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  io_ = &ImGui::GetIO();
  io_->ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Abilita controllo da tastiera
  io_->ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Abilita controllo da gamepad

  ImGui::StyleColorsDark();

  // --- Inizializzazione Backend ImGui ---
  ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
  ImGui_ImplOpenGL3_Init(glsl_version);

  isInitialized_ = true;
}
// ... (costruttore, distruttore, initialize, shutdown rimangono uguali)

bool GraphicsManager::beginFrame(std::vector<float *> kill) {
  if (!isInitialized_) {
    return false; // Non può iniziare un frame se non inizializzato
  }

  // Gestione degli eventi SDL
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT) {
      return false; // Segnala al main loop di terminare
    }
    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE &&
        event.window.windowID == SDL_GetWindowID(window_)) {
      for (auto &k : kill) {
        *k = -1.0f;
      }

      return false; // Segnala al main loop di terminare
    }
  }

  // Avvia un nuovo frame di ImGui
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  return true; // Continua il frame
}

void GraphicsManager::endFrame() {
  if (!isInitialized_) {
    return;
  }

  // Rendering di ImGui
  ImGui::Render();

  // Pulizia e rendering su schermo
  glViewport(0, 0, (int)io_->DisplaySize.x, (int)io_->DisplaySize.y);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  SDL_GL_SwapWindow(window_);
}

void GraphicsManager::shutdown() {
  if (!isInitialized_) {
    return;
  }

  // De-inizializza ImGui in ordine inverso
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // De-inizializza SDL
  SDL_GL_DeleteContext(glContext_);
  SDL_DestroyWindow(window_);
  SDL_Quit();

  // Resetta i puntatori per sicurezza
  window_ = nullptr;
  glContext_ = nullptr;
  io_ = nullptr;
  isInitialized_ = false;
}

void GraphicsManager::config() {

  const ImGuiViewport *viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoSavedSettings;
}