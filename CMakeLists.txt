 

#cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug 

 

cmake_minimum_required(VERSION 3.16) 

project(main VERSION 0.1.0) 

 

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto 

include(CTest) 

 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

 

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato  

set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_CXX_STANDARD_REQUIRED ON) 

set(CMAKE_CXX_EXTENSIONS ON) 

 

# abilita warning 

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra") 

 

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode 

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer") 

string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer") 

 

 

 

# Aggiunge il target eseguibile. Specifica il nome dell'eseguibile e i file sorgente. 

add_executable(main main.cpp HopfieldAlgorithm.cpp quadro.cpp corrotto.cpp Pattern.cpp TantiPattern.cpp

 ImGuiFileDialog/ImGuiFileDialog.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui_demo.cpp imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_opengl3.cpp
) 

 




find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(main ${SDL2_LIBRARIES} GL)

 


 

 

 

 

 