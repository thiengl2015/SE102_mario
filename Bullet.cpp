#include "Bullet.h"
#include "Game.h"
#include "Animations.h"

CBullet::CBullet(float x, float y) : CGameObject(x, y)
{
    this->vx = 0.05f;  
    isExist = true;
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + BULLET_BBOX_WIDTH;
    bottom = y + BULLET_BBOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!isExist)
    {
        isDeleted = true;
        return;
    }
    x += vx * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CBullet::Render()
{
  
}