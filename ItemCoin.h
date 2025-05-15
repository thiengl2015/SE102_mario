#pragma once
#include "GameObject.h"

#define ITEM_COIN_WIDTH 10
#define ITEM_COIN_HEIGHT 16
#define ID_ANI_ITEM_COIN 11000

class CItemCoin : public CGameObject
{
    int pointSpriteId;
    float startY;

    bool isRising;
    bool isFalling;

    static constexpr float RISE_DISTANCE = 64.0f;
    static constexpr float FALL_DISTANCE = 50.0f;
    static constexpr float RISE_SPEED = 0.3f;
    static constexpr float FALL_SPEED = 0.3f;

public:
    CItemCoin(float x, float y, int pointId);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
