#include "GraphicsManager.hpp"
#include <filesystem>
#include <fstream>
#include <map>

#include "../imgui/imgui.h"
template <typename T> auto POINTS = MD::getMathematicalVertex<T>();

template <typename T, typename M>
void Comp<T, M>::drawGrid(const std::vector<T> &data, int cols, int rows,
                          const char *id_grid,
                          const std::function<void(int)> &function,
                          float size) {
  ImGui::PushID(id_grid);

  bool clickable = static_cast<bool>(function);

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
  ImVec2 canvas_sz = ImVec2(size, size);
  ImVec2 canvas_p1 =
      ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

  draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

  ImGui::InvisibleButton("##invisible_button", canvas_sz);

  bool is_grid_hovered = ImGui::IsItemHovered();

  const float cell_size_x = canvas_sz.x / cols;
  const float cell_size_y = canvas_sz.y / rows;

  const ImU32 COLOR_HOVER = IM_COL32(100, 100, 255, 150);

  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      int index = row * cols + col;
      if (static_cast<std::vector<int>::size_type>(index) >= data.size()) {
        continue;
      }
      float x0 = canvas_p0.x + col * cell_size_x;
      float y0 = canvas_p0.y + row * cell_size_y;
      float x1 = x0 + cell_size_x;
      float y1 = y0 + cell_size_y;

      auto it = std::find(POINTS<T>.begin(), POINTS<T>.end(), data[index]);
      if (it == POINTS<T>.end()) {
        throw std::invalid_argument(
            "Current pixel value is not found in POINTS set.");
      }
      int color =
          50 + 200 * std::distance(POINTS<T>.begin(), it) / POINTS<T>.size();
      const ImU32 COLOR_BLACK = IM_COL32(color, color, color, 255);
      ImU32 cell_color = COLOR_BLACK;
      draw_list->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), cell_color);
    }
  }

  /// clickable
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
          function(hovered_index);
        }
      }
    }
  }

  ImGui::PopID();
}
template <typename T, typename M>
void Comp<T, M>::drawPlot(const std::vector<float> &array) {

  if (array.size() == 0) {
    ImGui::Text("Nessun dato di energia disponibile.");
  } else {
    ImGui::Text("Andamento dell'energia");
    const char *graph_label = "##label";

    const float *values = array.data();

    float min_val = 0.0f;
    float max_val = 0.0f;
    auto start_it = array.begin();
    auto end_it = array.end();

    if (start_it != end_it) {
      min_val = *std::min_element(start_it, end_it);
      max_val = *std::max_element(start_it, end_it);

      float buffer = (max_val - min_val) * 0.1f;
      min_val -= buffer;
      max_val += buffer;
    }

    ImGui::PlotLines(graph_label, values, array.size(), 0, NULL, min_val,
                     max_val, ImVec2(300, 300));
  }
}

template <typename T, typename M>
void Comp<T, M>::setElementsByFile(const std::string &filePath, int *a, int *b,
                                   std::vector<M> *m) {

  std::filesystem::path filePath2(filePath);

  if (!std::filesystem::exists(filePath)) {
    throw std::runtime_error("File training: " + filePath2.string() +
                             " not found");
  }
  std::ifstream inFile(filePath);
  if (!inFile.is_open()) {
    throw std::runtime_error("Cannot open file for reading: " +
                             filePath2.string());
  }

  int numRows, numColumns;

  if (!(inFile >> numRows >> numColumns)) {
    throw std::runtime_error(
        "Invalid file format: unable to read dimensions from " +
        filePath2.string());
  }

  std::vector<M> matrix;
  M element;

  while (inFile >> element) {
    matrix.push_back(element);
  }
  if (!inFile.eof() && inFile.fail()) {

    throw std::runtime_error(
        "Error: The file contains invalid data or is in an incorrect format.");
  }

  inFile.close();
  *a = numRows;
  *b = numColumns;
  *m = matrix;
}