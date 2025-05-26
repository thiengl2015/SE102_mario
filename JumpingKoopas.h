#pragma once
#include "GameObject.h"

class CMario;

#define JKOOPAS_GRAVITY 0.002f
#define JKOOPAS_WALKING_SPEED 0.05f
#define JKOOPAS_JUMP_SPEED_Y 0.5f
#define JKOOPAS_JUMP_INTERVAL 850
#define JKOOPAS_REVIVE_TIMEOUT 5000
#define JKOOPAS_SHELL_SLIDE_SPEED 0.2f

#define JKOOPAS_BBOX_WIDTH 16
#define JKOOPAS_BBOX_HEIGHT 26
#define JKOOPAS_BBOX_HEIGHT_DIE 7

#define JKOOPAS_STATE_JUMPING 100
#define JKOOPAS_STATE_WALKING 200
#define JKOOPAS_STATE_SHELL 300
#define JKOOPAS_STATE_SHELL_MOVING 400
#define JKOOPAS_STATE_HELD 500
#define JKOOPAS_STATE_REVIVING 600
#define JKOOPAS_STATE_DIE_FALL 700

#define ID_ANI_JKOOPAS_JUMP_LEFT 5203
#define ID_ANI_JKOOPAS_JUMP_RIGHT 5204
#define ID_ANI_JKOOPAS_WALK_LEFT 5205
#define ID_ANI_JKOOPAS_WALK_RIGHT 5206
#define ID_ANI_JKOOPAS_SHELL 5207

class CJumpingKoopas : public CGameObject {
protected:
    float ax, ay;
    int walkingDirection;
    bool isBeingHeld;

    ULONGLONG shell_start;
    ULONGLONG jump_start;

    CMario* holder = nullptr;

    int pointIdJump; 
    int pointIdWalk; 
    int pointIdKick;

    bool hasSpawnedPointJump = false;
    bool hasSpawnedPointWalk = false;
    bool hasSpawnedPointKick = false;

public:
    CJumpingKoopas(float x, float y, int pointJump, int pointWalk, int pointKick);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
    void OnNoCollision(DWORD dt) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    void SetState(int state) override;
    bool IsShellState() const;

    void KickShell(int dir);
    void SetHolder(CMario* mario);
    CMario* GetHolder();
    void SetBeingHeld(bool held) { isBeingHeld = held; }
    bool IsBeingHeld() const { return isBeingHeld; }

    int IsBlocking() override { return 0; }
    int IsCollidable() override {
        return (state != JKOOPAS_STATE_DIE_FALL);
    }
	void SetVx(float vx) { this->vx = vx; }
	void SetVy(float vy) { this->vy = vy; }
	void SetAy(float ay) { this->ay = ay; }
	void SetWalkingDirection(int dir) { walkingDirection = dir; }
};
