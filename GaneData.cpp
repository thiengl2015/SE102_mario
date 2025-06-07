#include "GameData.h"

CGameData* CGameData::instance = nullptr;

CGameData* CGameData::GetInstance() {
    if (!instance) instance = new CGameData();
    return instance;
}

void CGameData::Init() {
    marioLives = 4;
    score = 0;
    coins = 0;
}

void CGameData::ResetData() {
    marioLives = 4;
    score = 0;
    coins = 0;
}

void CGameData::DecreaseLives() {
    if (marioLives > 0) marioLives--;
}

void CGameData::IncreaseLives() {
    marioLives++;
}