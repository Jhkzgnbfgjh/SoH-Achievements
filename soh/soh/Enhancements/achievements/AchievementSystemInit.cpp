#include "AchievementSystem.h"
#include "AchievementWindow.h"
#include "AchievementSystemInit.h" // declara InitAchievementsSystem
#include "soh/Enhancements/game-interactor/GameInteractor.h"

extern "C" PlayState* gPlayState;

void InitAchievementsSystem() {
    AchievementSystem::Instance().Init();
    AchievementWindow::Register();

    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (gPlayState != nullptr) {
            AchievementSystem::Instance().Update(gPlayState);
        }
    });

    // NOVO: troca o "save ativo" quando o jogador carrega um arquivo
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnLoadGame>([](int32_t fileNum) {
        AchievementSystem::Instance().SetSaveFile(fileNum);
    });
}