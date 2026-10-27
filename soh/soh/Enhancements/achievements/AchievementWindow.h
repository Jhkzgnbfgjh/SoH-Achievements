#pragma once
#include <libultraship/libultraship.h>
#include <memory>

class AchievementWindow {
  public:
    static void Register();
};

class AchievementGuiWindow : public Ship::GuiWindow {
  public:
    using GuiWindow::GuiWindow;
    void DrawElement() override;
    void InitElement() override {
    }
    void UpdateElement() override {
    }
};