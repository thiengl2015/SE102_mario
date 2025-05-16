#pragma once
#include "GameObject.h"

#define RED_GOOMBA_JUMP_SPEED_Y		0.4f  // Tốc độ nhảy theo trục Y
#define RED_GOOMBA_GRAVITY			0.001f  // Lực hấp dẫn tác dụng lên Red Goomba
#define RED_GOOMBA_WALK_SPEED		0.03f  // Tốc độ di chuyển của Red Goomba

#define RED_GOOMBA_BBOX_WIDTH 16  // Chiều rộng hitbox
#define RED_GOOMBA_BBOX_HEIGHT 14  // Chiều cao hitbox ở trạng thái bình thường
#define RED_GOOMBA_BBOX_HEIGHT_DIE 7  // Chiều cao hitbox khi bị tiêu diệt

#define RED_GOOMBA_JUMP_INTERVAL     1000  // Thời gian giữa các lần nhảy
#define RED_GOOMBA_DIE_TIMEOUT 500  // Thời gian tồn tại sau khi chết

#define RED_GOOMBA_STATE_WINGED    110   // Trạng thái có cánh
#define RED_GOOMBA_STATE_WALKING   111   // Trạng thái đi bộ
#define RED_GOOMBA_STATE_DIE       112   // Trạng thái chết hoàn toàn

#define ID_ANI_RED_GOOMBA_WING     5000001   
#define ID_ANI_RED_GOOMBA_WALK     5000002   
#define ID_ANI_RED_GOOMBA_DIE      5000003   

#define RED_GOOMBA_ACTIVATE_DISTANCE 200

class CRedGoomba : public CGameObject
{
protected:
    ULONGLONG jump_start;  // Thời điểm bắt đầu nhảy
    bool isOnPlatform;  // Kiểm tra nếu đứng trên nền 

    float ax;
    float ay;
    ULONGLONG die_start;
    float spawmX;
    float originalX;
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    int pointIdWinged;
    int pointIdWalking;
    bool hasSpawnedWingedPoint = false;
    bool hasSpawnedWalkingPoint = false;

public:
    CRedGoomba(float x, float y, int pointIdWinged, int pointIdWalking);
    virtual void SetState(int state);
};