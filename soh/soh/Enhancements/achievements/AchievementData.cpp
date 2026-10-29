#include "Achievement.h"
#include "soh/SohGui/ImGuiUtils.h"   // GetTextureForItemId

extern "C" {
#include "z64.h"
#include "macros.h"
#include "functions.h"
#include "variables.h"
#include "z64item.h"
}

// ============================================================
// Helpers de checagem
// ============================================================

static bool HasHearts(int32_t count) {
    return gSaveContext.healthCapacity >= count * 16;
}

static bool HasSkulltulas(int32_t count) {
    return gSaveContext.inventory.gsTokens >= count;
}

// AchievementsData.cpp
static std::string Icon(uint32_t itemId) {
    const char* tex = GetTextureForItemId(itemId);
    return tex ? std::string(tex) : std::string();
}

// ============================================================
// Definição das conquistas
// ============================================================

std::vector<Achievement> BuildAchievements() {
    std::vector<Achievement> list;

    // --------------------------------------------------------
    // MAIN — Masmorras e chefes
    // --------------------------------------------------------
    
    // Deku Tree → Esmeralda Kokiri
    list.push_back({
        .id = "deku_tree_cleared",
        .name = "O Grande Deku Cai",
        .description = "Conclua a Árvore Deku.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_KOKIRI_EMERALD),
        .check =
            [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_KOKIRI_EMERALD_DEKU_TREE_DEAD); },
    });

    list.push_back({
        .id = "dodongos_cavern_cleared",
        .name = "Pedras Que Falam",
        .description = "Conclua a Caverna dos Dodongos.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_GORON_RUBY),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_USED_DODONGOS_CAVERN_BLUE_WARP); },
    });

    list.push_back({
        .id = "silver_scale",
        .name = "Além do Raso",
        .description = "Obtenha a Silver Scale.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_SCALE_SILVER),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_SILVER_SCALE); },
    });
    
    list.push_back({
        .id = "rutos_letter",
        .name = "Uma Carta do Fundo",
        .description = "Obtenha a carta de Ruto.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_LETTER_RUTO),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_RUTOS_LETTER); },
    });

    list.push_back({
        .id = "jabu_jabu_cleared",
        .name = "A Barriga do Peixe",
        .description = "Conclua a Barriga de Jabu-Jabu.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_ZORA_SAPPHIRE),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_USED_JABU_JABUS_BELLY_BLUE_WARP); },
    });

    list.push_back({
        .id = "door_of_time_opened",
        .name = "O Portal do Tempo",
        .description = "Abra a Porta do Tempo no Templo do Tempo.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_OPENED_THE_DOOR_OF_TIME); },
    });

    list.push_back({
        .id = "adult_link",
        .name = "Sete Anos Depois",
        .description = "Puxe a Master Sword e acorde como adulto pela primeira vez.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_TIME_TRAVELED_TO_ADULT); },
    });

    list.push_back({
        .id = "master_sword",
        .name = "A Lâmina do Herói",
        .description = "Retire a Master Sword do pedestal.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_SWORD_MASTER),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_PULLED_MASTER_SWORD_FROM_PEDESTAL); },
    });

    list.push_back({
        .id = "played_sarias_song_for_mido",
        .name = "O Luto de Mido",
        .description = "Toque a Canção de Saria para Mido como adulto.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_SONG_SARIA),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_PLAYED_SARIAS_SONG_FOR_MIDO_AS_ADULT); },
    });

    list.push_back({
        .id = "forest_temple_cleared",
        .name = "Primeiro Selo",
        .description = "Conclua o Templo da Floresta.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_USED_FOREST_TEMPLE_BLUE_WARP); },
    });

    list.push_back({
        .id = "fire_temple_cleared",
        .name = "A Forja do Fogo",
        .description = "Conclua o Templo do Fogo.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_MEDALLION_FIRE),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_USED_FIRE_TEMPLE_BLUE_WARP); },
    });

    list.push_back({
        .id = "ice_cavern_cleared",
        .name = "O Gelo Eterno",
        .description = "Obtenha as Iron Boots na Caverna de Gelo.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_BOOTS_IRON),
        .check = [](PlayState* play) { return INV_CONTENT(ITEM_BOOTS_IRON) == ITEM_BOOTS_IRON; },
    });

    list.push_back({
        .id = "water_temple_cleared",
        .name = "As Profundezas",
        .description = "Conclua o Templo da Água.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_MEDALLION_WATER),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_USED_WATER_TEMPLE_BLUE_WARP); },
    });

    list.push_back({
        .id = "lake_hylia_restored",
        .name = "Águas Restauradas",
        .description = "Restaure o Lago Hylia derrotando o Templo da Água.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_RAISED_LAKE_HYLIA_WATER); },
    });

    list.push_back({
        .id = "well_drained",
        .name = "O Poço Sombrio",
        .description = "Drene o Poço de Kakariko.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_LENS),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_DRAINED_WELL_IN_KAKARIKO); },
    });

    list.push_back({
        .id = "bottom_of_the_well",
        .name = "O Fundo do Poço",
        .description = "Obtenha a Lens of Truth no Fundo do Poço.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_LENS),
        .check = [](PlayState* play) { return INV_CONTENT(ITEM_LENS) == ITEM_LENS; },
    });

    list.push_back({
        .id = "shadow_temple_cleared",
        .name = "O Templo das Sombras",
        .description = "Conclua o Templo das Sombras.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_MEDALLION_SHADOW),
        .check =
            [](PlayState* play) {
                return (gSaveContext.inventory.questItems & (1 << QUEST_MEDALLION_SHADOW)) != 0;
            },
    });

    list.push_back({
        .id = "carpenters_freed",
        .name = "Libertador de Gerudo",
        .description = "Liberte todos os carpinteiros da Fortaleza Gerudo.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_GERUDO_CARD),
        .check = [](PlayState* play) { return GET_EVENTCHKINF_CARPENTERS_FREE_ALL(); },
    });

    list.push_back({
        .id = "spirit_temple_cleared",
        .name = "O Templo do Espírito",
        .description = "Conclua o Templo do Espírito.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_MEDALLION_SPIRIT),
        .check =
            [](PlayState* play) {
                return (gSaveContext.inventory.questItems & (1 << QUEST_MEDALLION_SPIRIT)) != 0;
            },
    });

    list.push_back({
        .id = "all_medallions",
        .name = "As Seis Luzes",
        .description = "Reúna todos os seis Medalhões.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_ARROW_LIGHT),
        .check =
            [](PlayState* play) {
                return Flags_GetEventChkInf(EVENTCHKINF_RETURNED_TO_TEMPLE_OF_TIME_WITH_ALL_MEDALLIONS);
            },
    });

    list.push_back({
        .id = "entered_ganons_castle",
        .name = "O Covil do Mal",
        .description = "Descubra o Castelo de Ganon.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_KEY_BOSS),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_ENTERED_GANONS_CASTLE_EXTERIOR); },
    });

    list.push_back({
        .id = "all_adult_temples",
        .name = "A Ponte do Arco-Íris",
        .description = "Complete os cinco templos adultos e construa a Ponte do Arco-Íris.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_ARROW_LIGHT),
        .check =
            [](PlayState* play) {
                return Flags_GetEventChkInf(EVENTCHKINF_RAINBOW_BRIDGE_BUILT);
            },
    });

    list.push_back({
        .id = "ganons_castle_cleared",
        .name = "A Subida...",
        .description = "Complete todos os trials do Castelo de Ganon.",
        .category = AchievementCategory::Main,
        .hidden = false,
        .icon = Icon(ITEM_GAUNTLETS_GOLD),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_DISPELLED_GANONS_TOWER_BARRIER); },
    });

    // --------------------------------------------------------
    // SIDEQUEST — Missões opcionais
    // --------------------------------------------------------
    list.push_back({
        .id = "epona_obtained",
        .name = "Amiga de Infância",
        .description = "Obtenha Epona na Fazenda Lon Lon.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .icon = Icon(ITEM_SONG_EPONA),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_EPONA_OBTAINED); },
    });

    list.push_back({
        .id = "pocket_egg",
        .name = "Ovo no Bolso",
        .description = "Obtenha o Pocket Egg.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .icon = Icon(ITEM_WEIRD_EGG),
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_OBTAINED_POCKET_EGG); },
    });

    list.push_back({
        .id = "biggoron_sword",
        .name = "Aço de Verdade",
        .description = "Forje a Biggoron Sword.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .icon = Icon(ITEM_SWORD_BGS),
        .check = [](PlayState* play) {
            return (gSaveContext.inventory.equipment & (1 << EQUIP_INV_SWORD_BIGGORON)) != 0;
        },
    });

    list.push_back({
        .id = "gerudo_training_ground",
        .name = "O Ninja Gerudo",
        .description = "Obtenha as Ice Arrows no Gerudo Training Ground.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .icon = Icon(ITEM_ARROW_ICE),
        .check = [](PlayState* play) { return INV_CONTENT(ITEM_ARROW_ICE) == ITEM_ARROW_ICE; },
    });

    list.push_back({
        .id = "all_ocarina_songs",
        .name = "O Bardo de Hyrule",
        .description = "Aprenda todas as doze músicas do Ocarina.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .check =
            [](PlayState* play) {
                uint32_t mask = 0;
                for (int i = QUEST_SONG_MINUET; i <= QUEST_SONG_STORMS; i++) {
                    mask |= (1u << i);
                }
                return (gSaveContext.inventory.questItems & mask) == mask;
            },
    });



    // --------------------------------------------------------
    // MINIGAME — Minigames e desafios
    // --------------------------------------------------------
    list.push_back({
        .id = "won_cow_malons_race",
        .name = "Corrida na Fazenda",
        .description = "Vença a corrida de Malon e ganhe uma vaca.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_WON_COW_IN_MALONS_RACE); },
    });

    list.push_back({
        .id = "giant_fish",
        .name = "O Peixe Lendário",
        .description = "Pesque um peixe de 20 libras ou mais no Lago Hylia.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .icon = Icon(ITEM_FISHING_POLE),
        .check = [](PlayState* play) { return gSaveContext.highScores[HS_FISHING] >= 20; },
    });

    list.push_back({
        .id = "horseback_archery_1000",
        .name = "Arqueiro Montado",
        .description = "Faça 1000 pontos no Horseback Archery em Gerudo Fortress.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .icon = Icon(ITEM_BOW),
        .check = [](PlayState* play) { return gSaveContext.highScores[HS_HBA] >= 1000; },
    });

    list.push_back({
        .id = "malon_race_sub_60",
        .name = "Galope Perfeito",
        .description = "Vença a corrida de Malon em menos de 60 segundos.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .icon = Icon(ITEM_SONG_EPONA),
        .check =
            [](PlayState* play) {
                int32_t t = gSaveContext.highScores[HS_HORSE_RACE];
                return t > 0 && t < 60;
            },
    });

    list.push_back({
        .id = "dampe_race_sub_60",
        .name = "Além do Coveiro",
        .description = "Vença a corrida do Dampé em menos de 60 segundos.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .check =
            [](PlayState* play) {
                int32_t t = gSaveContext.highScores[HS_DAMPE_RACE];
                return t > 0 && t < 60;
            },
    });

    list.push_back({
        .id = "mask_of_truth",
        .name = "A Verdade Revelada",
        .description = "Obtenha a Máscara da Verdade.",
        .category = AchievementCategory::Sidequest,
        .hidden = false,
        .icon = Icon(ITEM_MASK_TRUTH),
        .check = [](PlayState* play) { return INV_CONTENT(ITEM_MASK_TRUTH) == ITEM_MASK_TRUTH; },
    });

    list.push_back({
        .id = "running_man",
        .name = "O Homem Mais Rápido",
        .description = "''Vença'' o Homem Corredor na Cidade de Hyrule em menos de 46 segundos.",
        .category = AchievementCategory::Minigame,
        .hidden = false,
        .icon = Icon(ITEM_BOOTS_HOVER), // Bota de Hover como ícone de velocidade
        .check =
            [](PlayState* play) {
                int32_t t = gSaveContext.highScores[HS_MARATHON];
                return t > 0 && t < 46;
            },
    });

    // --------------------------------------------------------
    // COLLECTION — Corações, Skulltulas, etc.
    // --------------------------------------------------------
    list.push_back({
        .id = "first_heart_container",
        .name = "Um Coração a Mais",
        .description = "Obtenha seu primeiro Heart Container.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .check = [](PlayState* play) { return HasHearts(4); },
    });

    list.push_back({
        .id = "ten_hearts",
        .name = "Coração de Leão",
        .description = "Alcance 10 corações de vida máxima.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_HEART_CONTAINER),
        .check = [](PlayState* play) { return HasHearts(10); },
    });

    list.push_back({
        .id = "twenty_hearts",
        .name = "Coração de Titã",
        .description = "Alcance 20 corações de vida máxima.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_HEART_CONTAINER),
        .check = [](PlayState* play) { return HasHearts(20); },
    });
    
    list.push_back({
        .id = "first_skulltula",
        .name = "Aranha de Ouro",
        .description = "Entregue sua primeira Skulltula de Ouro.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = "__OTR__textures/icon_item_24_static/gQuestIconGoldSkulltulaTex",
        .check = [](PlayState* play) { return HasSkulltulas(1); },
    });

    list.push_back({
        .id = "fifty_skulltulas",
        .name = "Meio Caminho Andado",
        .description = "Entregue 50 Skulltulas de Ouro.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = "__OTR__textures/icon_item_24_static/gQuestIconGoldSkulltulaTex",
        .check = [](PlayState* play) { return HasSkulltulas(50); },
    });

    list.push_back({
        .id = "hundred_skulltulas",
        .name = "A Maldição Quebrada",
        .description = "Entregue as 100 Skulltulas de Ouro.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = "__OTR__textures/icon_item_24_static/gQuestIconGoldSkulltulaTex",
        .check = [](PlayState* play) { return HasSkulltulas(100); },
    });

    list.push_back({
        .id = "ten_big_poes",
        .name = "Caçador de Fantasmas",
        .description = "Entregue 10 Big Poes ao Poe Collector nas Ruínas do Mercado.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_BIG_POE),
        .check =
            [](PlayState* play) {
                // AJUSTAR conforme o grep confirmar: >= 10 (count) ou >= 1000 (pontos)
                return gSaveContext.highScores[HS_POE_POINTS] >= 1000;
            },
    });

    list.push_back({
        .id = "first_bottle",
        .name = "Primeira Garrafa",
        .description = "Obtenha sua primeira garrafa vazia.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_BOTTLE),
        .check =
            [](PlayState* play) {
                return gSaveContext.inventory.items[SLOT_BOTTLE_1] != ITEM_NONE ||
                       gSaveContext.inventory.items[SLOT_BOTTLE_2] != ITEM_NONE ||
                       gSaveContext.inventory.items[SLOT_BOTTLE_3] != ITEM_NONE ||
                       gSaveContext.inventory.items[SLOT_BOTTLE_4] != ITEM_NONE;
            },
    });

    list.push_back({
        .id = "four_bottles",
        .name = "Colecionador de Garrafas",
        .description = "Obtenha as quatro garrafas do jogo.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_BOTTLE),
        .check =
            [](PlayState* play) {
                return gSaveContext.inventory.items[SLOT_BOTTLE_1] != ITEM_NONE &&
                       gSaveContext.inventory.items[SLOT_BOTTLE_2] != ITEM_NONE &&
                       gSaveContext.inventory.items[SLOT_BOTTLE_3] != ITEM_NONE &&
                       gSaveContext.inventory.items[SLOT_BOTTLE_4] != ITEM_NONE;
            },
    });

    list.push_back({
        .id = "max_quiver",
        .name = "Arqueiro Supremo",
        .description = "Obtenha a aljava de 50 flechas.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_BOW),
        .check = [](PlayState* play) {
            // Extrai os 2 bits do UPG_QUIVER. Se for 2, é o nível máximo.
            return ((gSaveContext.inventory.upgrades >> (UPG_QUIVER * 2)) & 3) == 3;
        },
    });

    list.push_back({
        .id = "max_bomb_bag",
        .name = "Arsenal Explosivo",
        .description = "Obtenha a bolsa de bombas de 40.",
        .category = AchievementCategory::Collection,
        .hidden = false,
        .icon = Icon(ITEM_BOMB),
        .check = [](PlayState* play) {
            // Extrai os 2 bits do UPG_BOMB_BAG. Se for 2, é o nível máximo.
            return ((gSaveContext.inventory.upgrades >> (UPG_BOMB_BAG * 2)) & 3) == 3;
        },
    });

    // --------------------------------------------------------
    // MISC — Extras
    // --------------------------------------------------------
    list.push_back({
        .id = "watched_castle_collapse",
        .name = "O Fim de Um Mundo",
        .description = "Assista ao colapso do Castelo de Ganon.",
        .category = AchievementCategory::Misc,
        .hidden = true, // conquista secreta
        .check =
            [](PlayState* play) {
                return Flags_GetEventChkInf(EVENTCHKINF_WATCHED_GANONS_CASTLE_COLLAPSE_CAUGHT_BY_GERUDO);
            },
    });

    list.push_back({
        .id = "caught_by_guards",
        .name = "Visitante Indesejado",
        .description = "Seja pego pelos guardas do Castelo de Hyrule.",
        .category = AchievementCategory::Misc,
        .hidden = true,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_CAUGHT_BY_CASTLE_GUARDS); },
    });

        list.push_back({
        .id = "frogs_choir",
        .name = "Maestro dos Sapos",
        .description = "Toque a Canção da Floresta para o Maestro dos Sapos.",
        .category = AchievementCategory::Misc,
        .hidden = false,
        .check = [](PlayState* play) { return Flags_GetEventChkInf(EVENTCHKINF_SONGS_FOR_FROGS_CHOIR); },
    });

    return list;
}