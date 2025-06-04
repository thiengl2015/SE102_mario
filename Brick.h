#pragma once
#include "GameObject.h"


#define BRICK_BOUNCE_DURATION 150
#define BRICK_BOUNCE_HEIGHT 10

#define BRICK_STATE_DEBRIS 999
#define BRICK_DEBRIS_LIFETIME 500
#define BRICK_DEBRIS_GRAVITY 0.002f

class CBrick : public CGameObject {
protected:
    int brickType;
    int spawnType;
    float width, height;
    float bounceOffset = 0.0f;

    bool isUsed = false;
    bool isBouncing = false;
    ULONGLONG bounce_start = 0;

    bool pendingSpawn = false;
    int pendingSpawnType = 0;
    DWORD spawn_delay_start = 0;

    bool isDebris = false;
    DWORD debris_start = 0;

    ULONGLONG switchBounce_start = 0;
    bool showSwitchBounceEffect = false;

public:
    CBrick(float x, float y, float w, float h, int brickType, int spawnType, bool isDebris, float vx, float vy);
    CBrick(float x, float y, float w, float h, int brickType, int spawnType);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void OnCollisionWith(LPCOLLISIONEVENT e) override;

    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
    void SpawnItem();
    int GetBrickType() const { return brickType; }
};
