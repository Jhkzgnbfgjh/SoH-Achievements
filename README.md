> [!IMPORTANT]  
> **Este é um fork focado no desenvolvimento de um Sistema de Conquistas.**
> 
> O objetivo deste fork é adicionar um sistema local de conquistas (estilo RetroAchievements) integrado nativamente à engine do Ship of Harkinian, baseado na versão **9.3.1 (Dewey Bravo)**.
> 
> 🏆 **39 conquistas** iniciais cobrindo história, sidequests, minigames e colecionáveis.
> 💾 Salvamento automático por arquivo de save em JSON.
> 🔔 Popups estilizados com ícones e sons originais do jogo.
> 
> Para a documentação técnica de como o sistema funciona e quais arquivos foram modificados, leia o [README do módulo](soh/soh/Enhancements/achievements/README.md).
> 
> ---
> *Abaixo segue a documentação original do Ship of Harkinian.*

<br>

> [!IMPORTANT]  
> **Este é um fork focado no desenvolvimento de um Sistema de Conquistas.**
> 
> O objetivo deste fork é adicionar um sistema local de conquistas (estilo RetroAchievements) integrado nativamente à engine do Ship of Harkinian, baseado na versão **9.3.1 (Dewey Bravo)**.
> 
> 🏆 **39 conquistas** iniciais cobrindo história, sidequests, minigames e colecionáveis.
> 💾 Salvamento automático por arquivo de save em JSON.
> 🔔 Popups estilizados com ícones e sons originais do jogo.
> 
> Para a documentação técnica de como o sistema funciona e quais arquivos foram modificados, leia o [README do módulo](soh/soh/Enhancements/achievements/README.md).
> 
> ---
> *Abaixo segue a documentação original do Ship of Harkinian.*

<br>

Crie este arquivo dentro da pasta de conquistas:
markdown

# Achievements System

Sistema local de conquistas estilo RetroAchievements para o Ship of Harkinian.

## O que tem?
- **39 conquistas** divididas em 5 categorias (História, Sidequest, Minigame, Coleta, Extras).
- **Persistência por save file** (`achievements_save_<N>.json`), com migração automática do formato antigo `.txt`.
- **Janela nativa** em `Settings -> Achievements` com filtros, barra de progresso e ícones.
- **Popups customizados** com ícone do item, cor por categoria, duração de 6s e som de "Item Get" do OoT.
- **Toggle global** (liga/desliga) e **botão de reset** por save.

## ⚠️ Arquivos do core modificados

Para integrar este sistema, duas alterações foram feitas em arquivos upstream:

### 1. `soh/soh/SohGui/SohGui.cpp`
Adiciona no topo:
```cpp
#include "soh/Enhancements/achievements/AchievementSystemInit.h"

E ao final de SetupGuiElements():
cpp

InitAchievementsSystem();

2. soh/soh/Notification/Notification.cpp

No método Draw(), o ícone da notificação agora escala com a cvar Notifications.Size:
cpp

float iconScale = CVarGetFloat(CVAR_SETTING("Notifications.Size"), 1.8f);
float iconSize  = 32.0f * iconScale;
ImGui::Image(tex, ImVec2(iconSize, iconSize));

⚠️ Ao atualizar do upstream (Shipwright)

Este código foi baseado na tag 9.3.1 (Dewey Bravo).
Ao rodar git pull upstream develop, os arquivos SohGui.cpp e Notification.cpp podem conflitar. Resolva os conflitos reaplicando as duas modificações acima. Os arquivos dentro desta pasta (achievements/) são novos e não conflitarão.
Estrutura de Arquivos

    Achievement.h — Definição do modelo de dados.

    AchievementData.cpp — Lista com as 39 conquistas e suas lógicas de checagem.

    AchievementSystem.h/.cpp — Core do sistema (leitura/escrita JSON, loop de checagem).

    AchievementSystemInit.h/.cpp — Ponto de entrada (hooks de frame e load game).

    AchievementWindow.h/.cpp — Interface gráfica (ImGui).

text


---

### 2. Arquivo: `README.md` (na raiz do projeto)

Abra o `README.md` original do Shipwright (que já existe na raiz) e **adicione este bloco no topo**, deixando o texto original do Ship of Harkinian logo abaixo:

```markdown
> [!IMPORTANT]  
> **Este é um fork focado no desenvolvimento de um Sistema de Conquistas.**
> 
> O objetivo deste fork é adicionar um sistema local de conquistas (estilo RetroAchievements) integrado nativamente à engine do Ship of Harkinian, baseado na versão **9.3.1 (Dewey Bravo)**.
> 
> 🏆 **39 conquistas** iniciais cobrindo história, sidequests, minigames e colecionáveis.
> 💾 Salvamento automático por arquivo de save em JSON.
> 🔔 Popups estilizados com ícones e sons originais do jogo.
> 
> Para a documentação técnica de como o sistema funciona e quais arquivos foram modificados, leia o [README do módulo](soh/soh/Enhancements/achievements/README.md).
> 
> ---
> *Abaixo segue a documentação original do Ship of Harkinian.*

<br>

![Ship of Harkinian](docs/shiptitle.darkmode.png#gh-dark-mode-only)
![Ship of Harkinian](docs/shiptitle.lightmode.png#gh-light-mode-only)

* [Website](https://www.shipofharkinian.com)
* [Discord](https://discord.com/invite/harbourmasters)

If you're having any trouble after reading through this `README`, feel free to ask for help in the Support text channels. Please keep in mind that we do not condone piracy.

# Quick Start

The Ship does not include any copyrighted assets.  You are required to provide a supported copy of the game.

### 1. Verify your ROM dump
You can verify you have dumped a supported copy of the game by using the compatibility checker at https://ship.equipment/. If you'd prefer to manually validate your ROM dump, you can cross-reference its `sha1` hash with the hashes [here](docs/supportedHashes.json).

### 2. Download The Ship of Harkinian from [Releases](https://github.com/HarbourMasters/Shipwright/releases)

### 3. Launch the Game!
#### Windows
* Extract the zip
* Launch `soh.exe`

#### Linux
* Execute `soh.appimage`.  You may have to `chmod +x` the appimage via terminal.
* When prompted, select your supported copy of the game.
* Saves, settings and `oot.o2r` are stored in `~/.local/share/soh/`. If the folder you launch from already has `shipofharkinian.json`, `oot.o2r` or `oot-mq.o2r`, that folder is used instead. Set `SHIP_HOME` to choose another folder.

#### macOS
* Run `soh.app`. When prompted, select your supported copy of the game.
* You should see a notification saying `Processing OTR`, then, once the process is complete, you should get a notification saying `OTR Successfully Generated`, then the game should start.

### 4. Play!

Congratulations, you are now sailing with the Ship of Harkinian! Have fun!

# Configuration

### Default keyboard configuration
| N64 | A | B | Z | Start | Analog stick | C buttons | D-Pad |
| - | - | - | - | - | - | - | - |
| Keyboard | X | C | Z | Space | WASD | Arrow keys | TFGH |

### Other shortcuts
| Keys | Action |
| - | - |
| ESC | Toggle menu |
| F2 | Toggle capture mouse input |
| F5 | Save state |
| F6 | Change state |
| F7 | Load state |
| F9 | Toggle Text-to-Speech |
| F11 | Fullscreen |
| Tab | Toggle Alternate assets |
| Ctrl+R | Reset |

# Project Overview
Ship of Harkinian (SOH) is built atop a custom library dubbed libultraship (LUS). Back in the N64 days, there was an SDK distributed to developers named libultra; LUS is designed to mimic the functionality of libultra on modern hardware. In addition, we are dependent on the source code provided by the OOT decompilation project.

In order for the game to function, you will require a **legally acquired** ROM for Ocarina of Time. Click [here](https://ship.equipment/) to check the compatibility of your specific rom. Any copyrighted assets are extracted from the ROM and reformatted as a .o2r archive file which the code uses.

### Graphics Backends
Currently, there are three rendering APIs supported: DirectX11 (Windows), OpenGL (all platforms), and Metal (MacOS). You can change which API to use in the `Settings` menu of the menubar, which requires a restart.  If you're having an issue with crashing, you can change the API in the `shipofharkinian.json` file by finding the line `gfxbackend:""` and changing the value to `sdl` for OpenGL. DirectX 11 is the default on Windows.

# Custom Assets

Custom assets are packed in `.otr` archive files. To use custom assets, place them in the `mods` folder.

If you're interested in creating and/or packing your own custom asset `.otr` files, check out the following tools:
* [**retro - OTR generator**](https://github.com/HarbourMasters64/retro)
* [**fast64 - Blender plugin**](https://github.com/HarbourMasters/fast64)

# Development
### Building

If you want to manually compile SoH, please consult the [building instructions](docs/BUILDING.md).

### Playtesting
If you want to playtest a continuous integration build, you can find them at the links below. Keep in mind that these are for playtesting only, and you will likely encounter bugs and possibly crashes. 

* [Windows](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-windows.zip)
* [macOS](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-mac.zip)
* [Linux X86_64](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-linux-x86_64.zip)
* [Linux ARM](https://nightly.link/HarbourMasters/Shipwright/workflows/generate-builds/develop/soh-linux-aarch64.zip)

### Further Reading
More detailed documentation can be found in the 'docs' directory, including the aforementioned [building instructions](docs/BUILDING.md).

* [Credits](docs/CREDITS.md)
* [Custom Music](docs/CUSTOM_MUSIC.md)
* [Formatting](docs/FORMATTING.md)
* [Controller Mapping](docs/GAME_CONTROLLER_DB.md)
* [Modding](docs/MODDING.md)
* [Versioning](docs/VERSIONING.md)

<a href="https://github.com/Kenix3/libultraship/">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="./docs/poweredbylus.darkmode.png">
    <img alt="Powered by libultraship" src="./docs/poweredbylus.lightmode.png">
  </picture>
</a>
