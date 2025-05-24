#pragma once
#include "GameObject.h"


#define BRICK_BOUNCE_DURATION 150
#define BRICK_BOUNCE_HEIGHT 10

class CBrick : public CGameObject {
protected:
    int brickType;
    int spawnType;
    int itemSpriteId;
    int pointSpriteId;
    float width, height;
    float bounceOffset = 0.0f;

    bool isUsed = false;
    bool isBouncing = false;
    ULONGLONG bounce_start = 0;

    bool pendingSpawn = false;
    int pendingSpawnType = 0;
    DWORD spawn_delay_start = 0;

public:
    CBrick(float x, float y, float width, float height, int brickType, int spawnType, int itemSpriteId, int pointSpriteId);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
    void SpawnItem();
};
