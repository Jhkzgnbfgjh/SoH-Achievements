#include "AchievementWindow.h"
#include "AchievementSystem.h"

#include <imgui.h>
#include <ship/Context.h>
#include "soh/Notification/Notification.h"
#include "soh/SohGui/ImGuiUtils.h"
#include <fast/Fast3dGui.h>       // pra GetTextureByName
#include <ctime>

#include "soh/cvar_prefixes.h"
#include "libultraship/bridge/consolevariablebridge.h"
#include "soh/SohGui/UIWidgets.hpp"
#include "soh/SohGui/SohMenu.h"
#include "soh/SohGui/SohGui.hpp"

#include <memory>

// mSohMenu vive DENTRO do namespace SohGui (declarado em ResolutionEditor.cpp, etc.)
namespace SohGui {
extern std::shared_ptr<SohMenu> mSohMenu;
}

static void RegisterAchievementsWidgets() {
    // 1) Cria a sidebar "Achievements" na seção "Settings" (com 1 coluna)
    //    Guard evita duplicar se o init rodar mais de uma vez
    static bool sidebarAdded = false;
    if (!sidebarAdded) {
        SohGui::mSohMenu->AddSidebarEntry("Settings", "Achievements", 1);
        sidebarAdded = true;
    }

    // 2) Agora sim adiciona o widget
    WidgetPath path = { "Settings", "Achievements", SECTION_COLUMN_1 };
    SohGui::mSohMenu->AddWidget(path, "Abrir Janela de Conquistas", WIDGET_WINDOW_BUTTON)
        .CVar(CVAR_WINDOW("Achievements"))
        .RaceDisable(false)
        .WindowName("Conquistas")
        .HideInSearch(false)
        .Options(UIWidgets::WindowButtonOptions()
                     .Tooltip("Veja o progresso das suas conquistas e gerencie-as.")
                     .EmbedWindow(false));
}

static RegisterMenuInitFunc menuInitFunc(RegisterAchievementsWidgets);

void AchievementWindow::Register() {
    Ship::Context::GetRawInstance()->GetWindow()->GetGui()->AddGuiWindow(
        std::make_shared<AchievementGuiWindow>(CVAR_WINDOW("Achievements"), "Conquistas", ImVec2(480, 600)));
}

void AchievementGuiWindow::DrawElement() {
    auto& sys = AchievementSystem::Instance();

    // --- Toggle global + save ---
    bool enabled = CVarGetInteger("gSettings.Achievements.Enabled", 1) != 0;
    if (ImGui::Checkbox("Habilitado", &enabled)) {
        CVarSetInteger("gSettings.Achievements.Enabled", enabled ? 1 : 0);
    }
    ImGui::SameLine();
    ImGui::TextDisabled("(save %d)", sys.GetCurrentFileNum());

    ImGui::Separator();

    // --- Contador total ---
    ImGui::Text("Total: %zu / %zu conquistas", sys.GetUnlockedCount(), sys.GetTotalCount());
    ImGui::ProgressBar(sys.GetTotalCount() > 0 ? (float)sys.GetUnlockedCount() / (float)sys.GetTotalCount() : 0.0f,
                       ImVec2(-1, 0), "");

    // --- Filtros de categoria (persistem entre frames) ---
    static bool showMain = true;
    static bool showSidequest = true;
    static bool showMinigame = true;
    static bool showCollection = true;
    static bool showMisc = true;

    ImGui::Text("Filtrar:");
    ImGui::SameLine();
    ImGui::Checkbox("História", &showMain);
    ImGui::SameLine();
    ImGui::Checkbox("Sidequest", &showSidequest);
    ImGui::SameLine();
    ImGui::Checkbox("Minigame", &showMinigame);
    ImGui::SameLine();
    ImGui::Checkbox("Coleta", &showCollection);
    ImGui::SameLine();
    ImGui::Checkbox("Extras", &showMisc);

    ImGui::Separator();

    // --- Helper: renderiza uma categoria ---
    auto renderCategory = [&](const char* label, AchievementCategory cat, bool show) {
        if (!show)
            return;

        int unlockedInCat = 0, totalInCat = 0;
        for (auto& a : sys.GetAll()) {
            if (a.category != cat)
                continue;
            totalInCat++;
            if (a.unlocked)
                unlockedInCat++;
        }
        if (totalInCat == 0)
            return;

        char header[128];
        snprintf(header, sizeof(header), "%s (%d/%d)", label, unlockedInCat, totalInCat);
        if (!ImGui::CollapsingHeader(header, ImGuiTreeNodeFlags_DefaultOpen))
            return;

        ImGui::Indent();
            for (auto& a : sys.GetAll()) {
                if (a.category != cat)
                    continue;

                if (a.hidden && !a.unlocked) {
                    ImGui::TextDisabled("???");
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("Conquista secreta.");
                    }
                    continue;
                }

                // ── Ícone (via API pública) ──
                const char* texName = GetAchievementIcon(a);
                if (texName) {
                    auto tex = std::dynamic_pointer_cast<Fast::Fast3dGui>(
                                   Ship::Context::GetRawInstance()->GetWindow()->GetGui())
                                   ->GetTextureByName(texName);
                    if (tex) {
                        ImGui::Image(tex, ImVec2(24, 24));
                        ImGui::SameLine();
                    }
                }

                ImVec4 color = a.unlocked ? ImVec4(1.0f, 0.84f, 0.0f, 1.0f)
                                          : ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                ImGui::TextColored(color, "%s %s", a.unlocked ? "[X]" : "[ ]", a.name.c_str());
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("%s", a.description.c_str());
                    if (a.unlocked && a.unlockedAt > 0) {
                        time_t t = (time_t)a.unlockedAt;
                        struct tm* lt = localtime(&t);
                        ImGui::SetTooltip("%s\n\nDesbloqueado em: %02d/%02d/%04d %02d:%02d",
                            a.description.c_str(),
                            lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900,
                            lt->tm_hour, lt->tm_min);
                    }
                }
            }
        ImGui::Unindent();
    };

    renderCategory("História Principal", AchievementCategory::Main, showMain);
    renderCategory("Sidequests", AchievementCategory::Sidequest, showSidequest);
    renderCategory("Minigames", AchievementCategory::Minigame, showMinigame);
    renderCategory("Coleta", AchievementCategory::Collection, showCollection);
    renderCategory("Extras", AchievementCategory::Misc, showMisc);

    ImGui::Separator();
    if (ImGui::Button("Resetar conquistas deste save")) {
        sys.ResetAll();
    }
}