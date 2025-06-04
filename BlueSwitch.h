#pragma once
#include "GameObject.h"

#define SWITCH_WIDTH 16
#define SWITCH_HEIGHT 16

#define SWITCH_STATE_UP 0
#define SWITCH_STATE_DOWN 1

#define SWITCH_AOE_RANGE 96 

class CSwitchBlock : public CGameObject {
protected:
    bool isActivated = false;
    int state = SWITCH_STATE_UP;

public:
    CSwitchBlock(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }

    void Activate();
};
