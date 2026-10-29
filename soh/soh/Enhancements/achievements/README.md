# Achievements System

Sistema local de conquistas estilo RetroAchievements para o Ship of Harkinian.

## O que tem?
- **49 conquistas** divididas em 5 categorias (História, Sidequest, Minigame, Coleta, Extras).
- **Persistência por save file** (`achievements_save_<N>.json`), com migração automática do formato antigo `.txt`.
- **Janela nativa** em `Settings -> Achievements` com filtros, barra de progresso e ícones.
- **Popups customizados** com ícone do item, cor por categoria, duração de 6s e som de "Item Get" do OoT.
- **Toggle global** (liga/desliga) e **botão de reset** por save.

## ⚠️ Arquivos do core modificados

Para integrar este sistema, duas alterações foram feitas em arquivos upstream:

### 1. `soh/soh/SohGui/SohGui.cpp`
Adiciona no topo:
    #include "soh/Enhancements/achievements/AchievementSystemInit.h"

E ao final de `SetupGuiElements()`:
    InitAchievementsSystem();

### 2. `soh/soh/Notification/Notification.cpp`
No método `Draw()`, o ícone da notificação agora escala com a cvar `Notifications.Size`:
    float iconScale = CVarGetFloat(CVAR_SETTING("Notifications.Size"), 1.8f);
    float iconSize  = 32.0f * iconScale;
    ImGui::Image(tex, ImVec2(iconSize, iconSize));

## ⚠️ Ao atualizar do upstream (Shipwright)

Este código foi baseado na tag **9.3.1 (Dewey Bravo)**.
Ao rodar `git pull upstream develop`, os arquivos `SohGui.cpp` e `Notification.cpp` podem conflitar. Resolva os conflitos reaplicando as duas modificações acima. Os arquivos dentro desta pasta (`achievements/`) são novos e não conflitarão.

## Estrutura de Arquivos
- `Achievement.h` — Definição do modelo de dados.
- `AchievementData.cpp` — Lista com as 39 conquistas e suas lógicas de checagem.
- `AchievementSystem.h/.cpp` — Core do sistema (leitura/escrita JSON, loop de checagem).
- `AchievementSystemInit.h/.cpp` — Ponto de entrada (hooks de frame e load game).
- `AchievementWindow.h/.cpp` — Interface gráfica (ImGui).
