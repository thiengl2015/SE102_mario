#pragma once
#include "GameObject.h"
#include <vector>

#define BOOMERANG_BBOX_WIDTH 16
#define BOOMERANG_BBOX_HEIGHT 16
#define BOOMERANG_SPEED_X 0.15f
#define BOOMERANG_SPEED_Y -0.15f
#define BOOMERANG_GRAVITY 0.0005f
#define BOOMERANG_MAX_DISTANCE 96.0f
#define ID_ANI_BOOMERANG 5800

class CBoomerang : public CGameObject {
protected:
    float startX, startY;
    int direction;
    float ax = 0, ay = BOOMERANG_GRAVITY;
    int step = 0;
    bool returning = false;

public:
    CBoomerang(float x, float y, int direction);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& l, float& t, float& r, float& b);

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 0; }
    void OnCollisionWith(LPCOLLISIONEVENT e);
};
