#pragma once
#include "Achievement.h"
#include <vector>
#include <string>

struct PlayState;

// AchievementSystem.h
const char* GetAchievementIcon(const Achievement& a);

class AchievementSystem {
  public:
    static AchievementSystem& Instance();
    void Init();
    void SetSaveFile(int32_t fileNum); // ← NOVO
    void Update(PlayState* play);
    void Save();
    void Load();
    void ResetAll();

    std::vector<Achievement>& GetAll() {
        return achievements;
    }
    size_t GetUnlockedCount() const;
    size_t GetTotalCount() const {
        return achievements.size();
    }
    int32_t GetCurrentFileNum() const {
        return currentFileNum;
    } // ← NOVO

  private:
    std::vector<Achievement> achievements;
    uint64_t lastCheckFrame = 0;
    bool dirty = false;
    int32_t currentFileNum = -1; // ← NOVO

    void Unlock(Achievement& a);
    std::string GetSavePath() const;
};

std::vector<Achievement> BuildAchievements();