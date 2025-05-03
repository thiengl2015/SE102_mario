#pragma once
#include "Goomba.h"

#define RED_GOOMBA_JUMP_SPEED_Y		0.3f
#define RED_GOOMBA_GRAVITY			0.001f
#define RED_GOOMBA_WALK_SPEED		0.05f

#define RED_GOOMBA_BBOX_WIDTH 16
#define RED_GOOMBA_BBOX_HEIGHT 14
#define RED_GOOMBA_BBOX_HEIGHT_DIE 7

#define RED_GOOMBA_JUMP_INTERVAL     1000 
#define RED_GOOMBA_DIE_TIMEOUT 500

#define RED_GOOMBA_STATE_WINGED    110   // Trạng thái có cánh (ban đầu)
#define RED_GOOMBA_STATE_WALKING   111   // Không cánh (sau khi bị đạp)
#define RED_GOOMBA_STATE_DIE       112   // Chết hoàn toàn

#define ID_ANI_RED_GOOMBA_WING     5000001   
#define ID_ANI_RED_GOOMBA_WALK     5000002   
#define ID_ANI_RED_GOOMBA_DIE      5000003

class CRedGoomba : public CGoomba
{
protected:
    ULONGLONG jump_start;
    bool isOnPlatform;

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CRedGoomba(float x, float y);
    virtual void SetState(int state);
};