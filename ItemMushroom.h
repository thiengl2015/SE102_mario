#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

#define MUSHROOM_WIDTH 14
#define MUSHROOM_HEIGHT 14
#define MUSHROOM_RISE_DISTANCE 16
#define MUSHROOM_SPEED 0.03f
#define MUSHROOM_GRAVITY 0.001f
#define MUSHROOM_RISE_SPEED 0.01f

#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_ON_RISE 200
#define MUSHROOM_STATE_FULL 300

class CItemMushroom : public CGameObject
{
protected:
    float startY; 

    float ax = 0; 
    float ay = 0; 
    float riseDistance = 0; 
    int pointSpriteId;
    int spriteId; 

public:
    CItemMushroom(float x, float y, int spriteId, int pointSpriteId);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnNoCollision(DWORD dt) override;
    int IsBlocking() override { return 0; }
    int IsCollidable() override { return true; }
    virtual void SetState(int state);
};

