#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_BOUNCE_DURATION 150
#define BRICK_BOUNCE_HEIGHT 6

enum BrickSpawnType {
    BRICK_SPAWN_NONE = 0,     
    BRICK_SPAWN_COIN = 1,       
    BRICK_SPAWN_MUSHROOM = 2,   
    BRICK_SPAWN_LEAF = 3
};


class CBrick : public CGameObject
{
    bool isUsed = false;
    int brickType = 0;
    int spawnType = 0;
    int itemSpriteId = -1;
    int pointSpriteId = 40001;
    float bounceOffsetY = 0;
    bool isBouncing = false;
    DWORD bounce_start = 0;

public:
    CBrick(float x, float y, int brickType = 0, int spawnType = 0, int itemSpriteId = -1, int pointSpriteId = 40001);
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
};
