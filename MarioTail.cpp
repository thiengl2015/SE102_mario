#include "MarioTail.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "turle.h"
#include "debug.h"
#include "Game.h"
#include "Collision.h"
#include "Brick.h"
#include "Texture.h"

CMarioTail::CMarioTail(CMario* mario)
{
    this->mario = mario;
}

void CMarioTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    for (LPGAMEOBJECT obj : *coObjects)
    {
        if (obj->IsDeleted() || !obj->IsCollidable()) continue;
        if (obj == this) continue;

        float l1, t1, r1, b1;
        float l2, t2, r2, b2;

        this->GetBoundingBox(l1, t1, r1, b1);
        obj->GetBoundingBox(l2, t2, r2, b2);

        if (CGameObject::AABBCheck(l1, t1, r1, b1, l2, t2, r2, b2))
        {
            LPCOLLISIONEVENT e = new CCollisionEvent(0, 0, 0, 0, 0, obj, this);
            OnCollisionWith(e);
            delete e;
        }
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMarioTail::Render()
{
    RenderBoundingBox();
}

void CMarioTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = mario->GetX() + (mario->GetDirection() > 0 ? 10 : -TAIL_WIDTH);
    t = mario->GetY();
    r = l + TAIL_WIDTH;
    b = t + TAIL_HEIGHT;
}

void CMarioTail::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->obj == nullptr || isUntouchable) return;

    if (auto g = dynamic_cast<CGoomba*>(e->obj))
    {
        if (g->GetState() != GOOMBA_STATE_DIE)
        {
            g->SetState(GOOMBA_STATE_DIE);
            DebugOut(L">>> Tail hit Goomba\n");
            StartUntouchable();
        }
    }
    else if (auto rg = dynamic_cast<CRedGoomba*>(e->obj))
    {
        if (rg->GetState() == RED_GOOMBA_STATE_WINGED)
        {
            rg->SetState(RED_GOOMBA_STATE_WALKING);
            DebugOut(L">>> Tail hit Red Goomba (Winged)\n");
            StartUntouchable();
        }
        else if (rg->GetState() == RED_GOOMBA_STATE_WALKING)
        {
            rg->SetState(RED_GOOMBA_STATE_DIE);
            DebugOut(L">>> Tail hit Red Goomba (Walking)\n");
            StartUntouchable();
        }
    }
    else if (auto t = dynamic_cast<CTurtle*>(e->obj))
    {
        if (!t->IsShellState())
        {
            t->StartShell();
            DebugOut(L">>> Tail hit Turtle\n");
            StartUntouchable();
        }
    }
    else if (auto brick = dynamic_cast<CBrick*>(e->obj))
    {
        brick->OnCollisionWith(e);
        StartUntouchable();
    }
}
