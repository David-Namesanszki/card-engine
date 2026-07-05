#pragma once

enum class TurnType {
    Enemy,
    Player
};

enum class BattleDifficultyType {
    Minor,
    Major,
    Boss
};

struct BattleInfo {
    int turnCount;
    TurnType turnType;
    BattleDifficultyType difficulty;
};