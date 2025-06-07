#pragma once

class CGameData {
    static CGameData* instance;
    int marioLives;
    int score;
    int coins;

public:
    static CGameData* GetInstance();

    void Init();
    void ResetData();

    void DecreaseLives();
    void IncreaseLives();
    int GetLives() { return marioLives; }

    void AddScore(int s) { score += s; }
    int GetScore() { return score; }

    void AddCoin() { coins++; }
    int GetCoins() { return coins; }
};