// BouncingBrick.h
#pragma once

#include "GameObject.h"

#define BOUNCING_BRICK_BOUNCE_DURATION 150
#define BOUNCING_BRICK_BOUNCE_HEIGHT 10
#define MAX_BOUNCES 15

class CBrickN : public CGameObject {
protected:
    float width, height;
    float bounceOffset = 0.0f;
    bool isBouncing = false;
    ULONGLONG bounce_start = 0;
    int bounceCount = 0;
    bool isUsed = false;

public:
    CBrickN(float x, float y, float w, float h);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void SpawnItem();
    void HurtTurtleIfOnTop();

    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
    int GetType() override { return 9999; }
};
