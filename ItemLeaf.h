#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

#define LEAF_WIDTH 16
#define LEAF_HEIGHT 16
#define LEAF_RISE_DISTANCE 30
#define LEAF_SPEED 0.02f
#define LEAF_GRAVITY 0.02f
#define LEAF_RISE_SPEED 0.2f
#define LEAF_SWAY_SPEED 0.05f

#define LEAF_STATE_IDLE 100
#define LEAF_STATE_ON_RISE 200
#define LEAF_STATE_SWAY 300
#define LEAF_STATE_FULL 400

#define LEAF_POINT_ID 21000

class CItemLeaf : public CGameObject
{
protected:
    float startY;
	float startX; 
    float ax = 0;
    float ay = 0;
    float riseDistance = 0;
    float swayDirection = 1; // Điều hướng lắc lư
    int pointSpriteId;
	int spriteId;
    bool isCollected = false; 

public:
    CItemLeaf(float x, float y, int spriteId);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnNoCollision(DWORD dt) override;
    int IsBlocking() override { return 0; }
    int IsCollidable() override { return true; }
    virtual void SetState(int state);
};