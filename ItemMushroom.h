#pragma once
#include "GameObject.h"

#define MUSHROOM_WIDTH 16
#define MUSHROOM_HEIGHT 16
#define MUSHROOM_RISE_DISTANCE 16
#define MUSHROOM_SPEED 0.03f
#define MUSHROOM_GRAVITY 0.001f
#define MUSHROOM_RISE_SPEED 0.05f

class CItemMushroom : public CGameObject
{
    float startY;
    int movingStep = 0; 
    int spriteId;

    float ax = 0;
    float ay = MUSHROOM_GRAVITY;

public:
    CItemMushroom(float x, float y, int spriteId);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnNoCollision(DWORD dt) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    int IsBlocking() override { return 0; }
    int IsCollidable() override { return true; }
};
