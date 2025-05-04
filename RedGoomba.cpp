#include "RedGoomba.h"

CRedGoomba::CRedGoomba(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = RED_GOOMBA_GRAVITY;
    die_start = -1;
    this->jump_start = GetTickCount64();
    this->isOnPlatform = false;
    SetState(RED_GOOMBA_STATE_WINGED);
}

void CRedGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == RED_GOOMBA_STATE_DIE)
    {
        left = x - RED_GOOMBA_BBOX_WIDTH / 2;
        top = y - RED_GOOMBA_BBOX_HEIGHT_DIE / 2;
        right = left + RED_GOOMBA_BBOX_WIDTH;
        bottom = top + RED_GOOMBA_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - RED_GOOMBA_BBOX_WIDTH / 2;
        top = y - RED_GOOMBA_BBOX_HEIGHT / 2;
        right = left + RED_GOOMBA_BBOX_WIDTH;
        bottom = top + RED_GOOMBA_BBOX_HEIGHT;
    }
}

void CRedGoomba::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
    isOnPlatform = false;
}

void CRedGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CRedGoomba*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
        if (e->ny < 0) isOnPlatform = true;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
    }
}

void CRedGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (state == RED_GOOMBA_STATE_WINGED && isOnPlatform &&
        GetTickCount64() - jump_start > RED_GOOMBA_JUMP_INTERVAL)
    {
        vy = -RED_GOOMBA_JUMP_SPEED_Y;
        jump_start = GetTickCount64();
    }

    if ((state == RED_GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > RED_GOOMBA_DIE_TIMEOUT))
    {
        isDeleted = true;
        return;
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CRedGoomba::Render()
{
    int aniId = ID_ANI_RED_GOOMBA_WALK;
    if (state == RED_GOOMBA_STATE_WINGED)
        aniId = ID_ANI_RED_GOOMBA_WING;
    else if (state == RED_GOOMBA_STATE_DIE)
        aniId = ID_ANI_RED_GOOMBA_DIE;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CRedGoomba::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case RED_GOOMBA_STATE_WINGED:
        vx = -RED_GOOMBA_WALK_SPEED;
        break;

    case RED_GOOMBA_STATE_WALKING:
        vx = -RED_GOOMBA_WALK_SPEED;
        break;

    case RED_GOOMBA_STATE_DIE:
        die_start = GetTickCount64();
        y += (RED_GOOMBA_BBOX_HEIGHT - RED_GOOMBA_BBOX_HEIGHT_DIE) / 2;
        vx = 0;
        vy = 0;
        ay = 0;
        break;
    }
}