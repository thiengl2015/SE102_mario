#pragma once
#include "Platform.h"

#define DROPBRICK_SPRITE_ID 41001
#define ANIMATION_DROPBRICK 42001

#define DROPBRICK_BBOX_WIDTH 46
#define DROPBRICK_BBOX_HEIGHT 14

#define DROPBRICK_DROP_SPEED 0.1f

class CDropBrick : public CGameObject
{
protected:
    bool isMarioOn = false;
    float ax;
    float ay;

public:
    CDropBrick(float x, float y);
    void Update(DWORD dt) ;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    int IsBlocking() override { return 1; }
    int IsCollidable() override { return 1; }
};