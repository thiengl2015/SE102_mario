#pragma once
#include "GameObject.h"

#define ITEM_COIN_WIDTH 10
#define ITEM_COIN_HEIGHT 16
#define ITEM_COIN_LIFETIME 200 
#define ID_ANI_ITEM_COIN 11000

class CItemCoin : public CGameObject
{
    DWORD spawn_time;
public:
    CItemCoin(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};