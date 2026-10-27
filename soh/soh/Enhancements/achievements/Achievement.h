#pragma once
#include <string>
#include <functional>
#include <cstdint>
#include <vector>

struct PlayState;

enum class AchievementCategory { Main, Sidequest, Minigame, Collection, Misc };

struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    AchievementCategory category = AchievementCategory::Misc;
    bool hidden = false;
    std::string icon = ""; // ← NOVO: nome da textura, vazio = usa ícone da categoria
    std::function<bool(PlayState*)> check;
    bool unlocked = false;
    uint64_t unlockedAt = 0;

    std::string GetDisplayName() const {
        return (hidden && !unlocked) ? "???" : name;
    }
    std::string GetDisplayDescription() const {
        return (hidden && !unlocked) ? "Conquista secreta." : description;
    }
};