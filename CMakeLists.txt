#cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake_minimum_required(VERSION 3.16)
project(HopfieldProject VERSION 0.1.0)

option(USE_INCLUSION_MODEL "Use the inclusion model for templates instead of explicit instantiation" OFF) # Mantengo la tua opzione, ma non la userò direttamente in questo script.

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

# Variabili per raggruppare i file sorgente
set(SIMULATOR_CORE_SOURCES
    HopfieldSimulator/ImageTrainingPattern.cpp
    HopfieldSimulator/RandomTrainingPattern.cpp
    HopfieldSimulator/NoisyPattern.cpp
    HopfieldSimulator/EvolvingPattern.cpp
    HopfieldSimulator/CoherenceSetPattern.cpp
    HopfieldSimulator/HopfieldNetwork.cpp
    HopfieldSimulator/HopfieldSimulator.cpp
)

set(GRAPHICS_AND_UI_SOURCES
    GraphicsManager/GraphicsManager.cpp
    GraphicsManager/Components.cpp
    GraphicsManager/FileDialogHelper.cpp
    ImGuiFileDialog/ImGuiFileDialog.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# Libreria statica per la logica del simulatore
add_library(hopfield_simulator STATIC ${SIMULATOR_CORE_SOURCES})
target_include_directories(hopfield_simulator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/eigen
)

# Trova i pacchetti necessari
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED) 

# Eseguibile per i tipi reali
add_executable(hopfield_real main.cpp ${GRAPHICS_AND_UI_SOURCES})
target_link_libraries(hopfield_real PRIVATE
    hopfield_simulator
    SDL2::SDL2
    OpenGL::GL
)
target_include_directories(hopfield_real PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

# Eseguibile per i tipi complessi
add_executable(hopfield_complex main.cpp ${GRAPHICS_AND_UI_SOURCES})
target_compile_definitions(hopfield_complex PRIVATE USE_COMPLEX_TYPES)
target_link_libraries(hopfield_complex PRIVATE
    hopfield_simulator
    SDL2::SDL2
    OpenGL::GL
)
target_include_directories(hopfield_complex PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
)

# Test
enable_testing()
include(CTest)

set(TEST_SPECIFIC_SOURCES
    tests/all.test.cpp
    tests/MathDimension.test.cpp
    tests/Cast.test.cpp
    tests/RandomUtils.test.cpp
    tests/TrainingPattern.test.cpp
    tests/NoisyPattern.test.cpp
    tests/EvolvingPattern.test.cpp
    tests/CoherenceSetPattern.test.cpp
    tests/HopfieldNetwork.test.cpp
    tests/HopfieldSimulator.test.cpp
)

add_executable(all.t ${TEST_SPECIFIC_SOURCES})
target_link_libraries(all.t PRIVATE hopfield_simulator)
add_test(NAME all.t COMMAND all.t)