#pragma once
#include "GameObject.h"

#define DROPBRICK_BBOX_WIDTH 46
#define DROPBRICK_BBOX_HEIGHT 14

#define ANIMATION_DROPBRICK 42001

#define DROPBRICK_MOVE_VX -0.05f
#define DROPBRICK_FALL_VY 0.02f
#define DROPBRICK_GRAVITY 0.00025f

class CMario;

class CDropBrick : public CGameObject
{
protected:
    CMario* mario = nullptr;

    float activationRange = 200.0f;

    float vx = 0, vy = 0;
public:
    CDropBrick(float x, float y);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
    bool isFalling = false;
    bool hasMario = false;
    bool isActivated = false;
    void SetMario();
    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnCollisionWithMario(LPCOLLISIONEVENT e);
};
