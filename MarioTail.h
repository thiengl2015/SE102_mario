#pragma once
#include "GameObject.h"
#include "Mario.h"

#define TAIL_ATTACK_DURATION 300
#define TAIL_WIDTH 16
#define TAIL_HEIGHT 12

class CMario;

class CMarioTail : public CGameObject
{
protected:
    CMario* mario;
    bool isUntouchable = false;
    ULONGLONG untouchable_start = 0;

public:
    CMarioTail(CMario* mario);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void StartUntouchable() {
        isUntouchable = true;
        untouchable_start = GetTickCount64();
    }

    int IsBlocking() override { return 0; }
    int IsCollidable() override { return 1; }
};
