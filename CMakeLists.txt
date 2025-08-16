#cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug


cmake_minimum_required(VERSION 3.16) 

project(main VERSION 0.1.0) 

 option(USE_INCLUSION_MODEL "Use the inclusion model for templates instead of explicit instantiation" OFF)


# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto 

include(CTest) 

 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

 

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato  

set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_CXX_STANDARD_REQUIRED ON) 

set(CMAKE_CXX_EXTENSIONS OFF) 

 

# abilita warning 

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra") 

 

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode 

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer") 

string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer") 

 

 

# Aggiunge la directory di inclusione per Eigen
# Assicurati che la cartella 'eigen' (quella che contiene la sottocartella 'Eigen')
# sia nella stessa directory del tuo CMakeLists.txt
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen)


# Aggiunge il target eseguibile. Specifica il nome dell'eseguibile e i file sorgente. 

add_executable(main main.cpp GraphicsManager.cpp Components.cpp HopfieldSimulator/TrainingPattern.cpp HopfieldSimulator/NoisyPattern.cpp  HopfieldSimulator/EvolvingPattern.cpp  HopfieldSimulator/CoherenceSetPattern.cpp HopfieldSimulator/HopfieldNetwork.cpp HopfieldSimulator/HopfieldSimulator.cpp 

 ImGuiFileDialog/ImGuiFileDialog.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui_demo.cpp imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl3.cpp
) 

 

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(main ${SDL2_LIBRARIES} GL)

 
add_executable(all.t HopfieldNetwork.test.cpp  HopfieldSimulator/HopfieldNetwork.cpp )
# Anche i test potrebbero aver bisogno di Eigen se HopfieldNetwork.cpp lo usa
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/eigen) è già globale e copre questo
target_link_libraries(all.t)
add_test(NAME all.t COMMAND all.t)