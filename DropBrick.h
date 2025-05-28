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
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    int IsBlocking();
    int IsCollidable() override { return 1; }
    void setVx(float vx) { this->vx = vx; }
public:
    CDropBrick(float x, float y);

};