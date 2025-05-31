#pragma once
#include "GameObject.h"
#include "Mario.h"

#define FKOOPAS_STATE_FLYING 100
#define FKOOPAS_STATE_WALKING 200
#define FKOOPAS_STATE_SHELL 300
#define FKOOPAS_STATE_SHELL_MOVING 400
#define FKOOPAS_STATE_HELD 500
#define FKOOPAS_STATE_REVIVING 600
#define FKOOPAS_STATE_DIE_FALL 700

#define FKOOPAS_BBOX_WIDTH 16
#define FKOOPAS_BBOX_HEIGHT 26
#define FKOOPAS_BBOX_HEIGHT_DIE 7

#define FKOOPAS_GRAVITY 0.002f
#define FKOOPAS_WALKING_SPEED 0.05f
#define FKOOPAS_SHELL_SLIDE_SPEED 0.2f
#define FKOOPAS_REVIVE_TIMEOUT 5000

#define FKOOPAS_FLY_SPEED 0.12f
#define FKOOPAS_FLY_RANGE 48

class CFlyingKoopas : public CGameObject {
protected:
    float ax, ay;
    int walkingDirection;
    bool isBeingHeld;
    float flyTime = 0;

    ULONGLONG shell_start;

    float rootY;

    CMario* holder;

public:
    CFlyingKoopas(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void SetState(int state) override;

    bool IsShellState();
    void KickShell(int dir);

    void SetBeingHeld(bool held) { isBeingHeld = held; }
    bool IsBeingHeld() const { return isBeingHeld; }

    void SetHolder(CMario* mario) { holder = mario; }
    CMario* GetHolder() { return holder; }

    int IsBlocking() override { return 0; }
    int IsCollidable() override { return (state != FKOOPAS_STATE_DIE_FALL); }
};
