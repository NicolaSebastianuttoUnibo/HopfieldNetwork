#include "../ImGuiFileDialog/ImGuiFileDialog.h"
#include "GraphicsManager.hpp"
#include <filesystem>
#include <fstream>
#include <map>

FileDialogHelper::FileDialogHelper(std::string key, std::string title,
                                   std::string extensions, int max_selection)
    : dialogKey(std::move(key)), windowTitle(std::move(title)),
      allowedExtensions(std::move(extensions)), maxSelection(max_selection) {}

void FileDialogHelper::open() {
  IGFD::FileDialogConfig config;
  config.path = ".";
  config.countSelectionMax = maxSelection;
  ImGuiFileDialog::Instance()->OpenDialog(dialogKey, windowTitle,
                                          allowedExtensions.c_str(), config);
}

void FileDialogHelper::render() {
  if (ImGuiFileDialog::Instance()->IsOpened(dialogKey)) {
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
  }

  if (ImGuiFileDialog::Instance()->Display(dialogKey)) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::map<std::string, std::string> selections =
          ImGuiFileDialog::Instance()->GetSelection();
      for (const auto &[fileName, filePath] : selections) {
        try {
          if (onSuccess) {
            onSuccess(filePath);
          }
        } catch (const std::exception &e) {
          throw std::logic_error("Error on loading " + filePath + ": " +
                                 std::string(e.what()));
        }
      }
    }

    ImGuiFileDialog::Instance()->Close();

    if (onDialogClose) {
      onDialogClose();
    }
  }
}