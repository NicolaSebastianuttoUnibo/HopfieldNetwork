#ifndef GRAPHICS_MANAGER_H
#define GRAPHICS_MANAGER_H

#include <stdexcept>
#include <string>

struct SDL_Window;
struct ImGuiIO;
typedef void* SDL_GLContext;



class GraphicsManager {

    private:
    

    // Membri privati per incapsulare lo stato.
    // Il prefisso 'm_' è una convenzione comune per i membri di una classe.
    SDL_Window*   window_ = nullptr;
    SDL_GLContext glContext_ = nullptr;
    ImGuiIO*      io_ = nullptr;
    bool          isInitialized_ = false;
    void initialize();

public:
   
    GraphicsManager();

    ~GraphicsManager();

   bool beginFrame();
    void endFrame();

    void shutdown();
};

#endif // GRAPHICS_MANAGER_H