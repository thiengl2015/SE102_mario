#pragma once
#include "GameObject.h"
#include "Boomerang.h"
#include "AssetIDs.h"

#define BB_BBOX_WIDTH 16
#define BB_BBOX_HEIGHT 14
#define BB_WALK_SPEED 0.03f
#define BB_THROW_INTERVAL 3000
#define BB_THROW_PREPARE_TIME 500

#define ID_ANI_BB_HOLD_LEFT 5801
#define ID_ANI_BB_HOLD_RIGHT 5802
#define ID_ANI_BB_NOHOLD_LEFT 5803
#define ID_ANI_BB_NOHOLD_RIGHT 5804

#define BB_STATE_WALKING     0   
#define BB_STATE_THROWING    1   
#define BB_STATE_FLIPPED     2 

class CBoomerangBrother : public CGameObject {
protected:
    float startX, startY;
    int direction;
    bool isHolding = true;
    int step = 0;
    ULONGLONG prepareTime = 0;
    ULONGLONG lastThrow = 0;
    vector<CBoomerang*> boomerangs;
    float ay = 0.0012f;
    bool isOnPlatform = false;
    int state = BB_STATE_WALKING;
    bool isFlipped = false;
    float walkRange = 30.0f; 
    int walkDirection = -1;  
public:
    CBoomerangBrother(float x, float y);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void OnNoCollision(DWORD dt);
    void OnCollisionWith(LPCOLLISIONEVENT e);

    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 1; }
    int GetType() override { return OBJECT_TYPE_BOOMERANG_BROTHER; }
    float GetX() { return x; }
    float GetY() { return y; }

    void SetState(int state);
};