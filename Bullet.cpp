#include "Bullet.h"
#include "Game.h"
#include "Animations.h"
#define BULLET_LIFETIME 2000
CBullet::CBullet(float x, float y, float targetX, float targetY) : CGameObject(x, y)
{
    this->targetX = targetX;
    this->targetY = targetY;
    this->isExist = true;
    this->createTime = GetTickCount64();

    float dx = targetX - x;
    float dy = targetY - y;
    float length = sqrt(dx * dx + dy * dy);

    if (length == 0)
    {
        vx = 0.05f * ((targetX < x) ? -1 : 1); 
        vy = 0;
    }
    else
    {
        vx = (dx / length) * 0.1f;
        vy = (dy / length) * 0.1f;
    }

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
    if (GetTickCount64() - createTime > BULLET_LIFETIME)
    {
        isExist = false;
        isDeleted = true;
        return;
    }


    if (!isExist)
    {
        isDeleted = true;
        return;
    }
    x += vx * dt;
    y += vy * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);

}
void CBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) isExist=false;

}

void CBullet::Render()
{
    LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_BULLET);
    if (ani)
        ani->Render(x, y);
}