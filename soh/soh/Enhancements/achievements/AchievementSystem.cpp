#include "AchievementSystem.h"
#include "soh/Notification/Notification.h"
#include "soh/SohGui/ImGuiUtils.h"
#include "libultraship/bridge/consolevariablebridge.h"
#include <nlohmann/json.hpp>
#include <fstream>

extern "C" {
#include "z64.h"
#include "macros.h"
#include "functions.h"
#include "variables.h"
#include "z64item.h"
}

#include <cstdio>
#include <ctime>

// ============================================================
// Ícones por categoria
// ============================================================

static const char* CategoryIcon(AchievementCategory cat) {
    switch (cat) {
        case AchievementCategory::Main:
            return GetTextureForItemId(ITEM_MEDALLION_FOREST);
        case AchievementCategory::Sidequest:
            return GetTextureForItemId(ITEM_OCARINA_TIME);
        case AchievementCategory::Minigame:
            return GetTextureForItemId(ITEM_BOW);
        case AchievementCategory::Collection:
            return GetTextureForItemId(ITEM_HEART_CONTAINER);
        case AchievementCategory::Misc:
        default:
            return GetTextureForItemId(ITEM_RUPEE_GOLD);
    }
}

const char* GetAchievementIcon(const Achievement& a) {
    if (!a.icon.empty())
        return a.icon.c_str();
    return CategoryIcon(a.category);
}

AchievementSystem& AchievementSystem::Instance() {
    static AchievementSystem instance;
    return instance;
}

// ... resto igual (Init, SetSaveFile, GetSavePath, Load, Save) ...

void AchievementSystem::Init() {
    achievements = BuildAchievements();
    // NÃO chama Load() aqui — espera SetSaveFile
}

void AchievementSystem::SetSaveFile(int32_t fileNum) {
    currentFileNum = fileNum;
    for (auto& a : achievements) {
        a.unlocked = false;
        a.unlockedAt = 0;
    }
    if (fileNum >= 0)
        Load();
}

std::string AchievementSystem::GetSavePath() const {
    if (currentFileNum < 0)
        return "";
    return "achievements_save_" + std::to_string(currentFileNum) + ".txt";
}

// Helper: deriva .json a partir do path do .txt
static std::string ToJsonPath(const std::string& txtPath) {
    auto dot = txtPath.rfind('.');
    if (dot == std::string::npos) return txtPath + ".json";
    return txtPath.substr(0, dot) + ".json";
}

void AchievementSystem::Save() {
    auto path = GetSavePath();
    if (path.empty()) return;

    nlohmann::json j;
    j["version"] = 1;
    j["achievements"] = nlohmann::json::array();

    for (auto& a : achievements) {
        if (!a.unlocked) continue;
        j["achievements"].push_back({
            { "id", a.id },
            { "unlockedAt", a.unlockedAt },
        });
    }

    std::ofstream f(ToJsonPath(path));
    if (!f) return;
    f << j.dump(2);
    dirty = false;
}

void AchievementSystem::Load() {
    auto path = GetSavePath();
    if (path.empty()) return;

    // Reset
    for (auto& a : achievements) {
        a.unlocked = false;
        a.unlockedAt = 0;
    }

    auto jsonPath = ToJsonPath(path);

    // 1) Tenta JSON
    std::ifstream jf(jsonPath);
    if (jf) {
        try {
            nlohmann::json j;
            jf >> j;
            if (j.contains("achievements") && j["achievements"].is_array()) {
                for (auto& entry : j["achievements"]) {
                    std::string id = entry.value("id", "");
                    uint64_t at = entry.value("unlockedAt", (uint64_t)0);
                    for (auto& a : achievements) {
                        if (a.id == id) {
                            a.unlocked = true;
                            a.unlockedAt = at;
                            break;
                        }
                    }
                }
                return;
            }
        } catch (...) {
            // JSON corrompido → cai pra migração
        }
    }

    // 2) Migração: lê .txt antigo e salva como .json
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return;

    char buf[256];
    while (fgets(buf, sizeof(buf), f)) {
        std::string line(buf);
        auto sep = line.find(':');
        if (sep == std::string::npos) continue;
        std::string id = line.substr(0, sep);
        bool unlocked = !line.empty() && line.substr(sep + 1, 1) == "1";
        if (!unlocked) continue;
        for (auto& a : achievements) {
            if (a.id == id) {
                a.unlocked = true;
                a.unlockedAt = (uint64_t)time(nullptr);  // sem timestamp no .txt
                break;
            }
        }
    }
    fclose(f);

    // Salva migração
    Save();
    // (opcional) std::remove(path.c_str());  // apaga o .txt antigo
}

void AchievementSystem::Unlock(Achievement& a) {
    if (a.unlocked) return;
    a.unlocked = true;
    a.unlockedAt = (uint64_t)time(nullptr);
    dirty = true;
    Save();

    ImVec4 catColor;
    const char* catName = "";
    switch (a.category) {
        case AchievementCategory::Main:
            catColor = ImVec4(1.00f, 0.84f, 0.00f, 1.0f);
            catName = "HISTORIA";
            break;
        case AchievementCategory::Sidequest:
            catColor = ImVec4(0.40f, 0.85f, 0.40f, 1.0f);
            catName = "SIDEQUEST";
            break;
        case AchievementCategory::Minigame:
            catColor = ImVec4(0.40f, 0.70f, 1.00f, 1.0f);
            catName = "MINIGAME";
            break;
        case AchievementCategory::Collection:
            catColor = ImVec4(0.80f, 0.50f, 1.00f, 1.0f);
            catName = "COLETA";
            break;
        case AchievementCategory::Misc:
        default:
            catColor = ImVec4(0.80f, 0.80f, 0.80f, 1.0f);
            catName = "EXTRA";
            break;
    }

    Notification::Emit({
        .itemIcon = GetAchievementIcon(a),
        //.iconSize = 1.8f,
        .prefix = "CONQUISTA DESBLOQUEADA",
        .prefixColor = ImVec4(1.0f, 0.84f, 0.0f, 1.0f),
        .message = a.name,
        .messageColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
        .suffix = std::string("[") + catName + "]",
        .suffixColor = catColor,
        .remainingTime = 6.0f,
        .mute = true,  // ← mute o som default do Notification
    });

    // Som de item get (uma linha, sem cast)
    Sfx_PlaySfxCentered(NA_SE_SY_GET_ITEM);
}

void AchievementSystem::Update(PlayState* play) {
    if (play == nullptr)
        return;
    if (play->gameplayFrames - lastCheckFrame < 15)
        return;
    lastCheckFrame = play->gameplayFrames;
    if (currentFileNum < 0)
        return;
    if (CVarGetInteger("gSettings.Achievements.Enabled", 1) == 0)
        return;

    // Guard extra: só checa em gameplay "estável"
    if (gSaveContext.gameMode != GAMEMODE_NORMAL)
        return;
    if (play->state.running == false)
        return;

    for (auto& a : achievements) {
        if (a.unlocked)
            continue;
        if (a.check && a.check(play))
            Unlock(a);
    }
}

size_t AchievementSystem::GetUnlockedCount() const {
    size_t n = 0;
    for (auto& a : achievements)
        if (a.unlocked)
            n++;
    return n;
}

void AchievementSystem::ResetAll() {
    for (auto& a : achievements) {
        a.unlocked = false;
        a.unlockedAt = 0;
    }
    Save();
}