#include "ItemMushroom.h"
#include "Sprites.h"
#include "Collision.h"
#include "PlayScene.h"
#include "Game.h"
#include "debug.h"

CItemMushroom::CItemMushroom(float x, float y, int spriteId) : CGameObject(x, y)
{
    this->spriteId = spriteId;
    this->startY = y;
    vy = -MUSHROOM_RISE_SPEED;
    vx = 0;
    ay = 0;
    movingStep = 0;
}

void CItemMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects); 

    DebugOut(L"[MUSHROOM] Step: %d | Pos: (%.2f, %.2f) | Vel: (%.3f, %.3f) | dxy: (%.3f, %.3f)\n",
        movingStep, x, y, vx, vy, dx, dy);

    switch (movingStep)
    {
    case 0: 
        y += dy;

        DebugOut(L"[MUSHROOM] Rising: y = %.2f\n", y);

        if (startY - y >= MUSHROOM_RISE_DISTANCE)
        {
            y = startY - MUSHROOM_RISE_DISTANCE;
            vy = 0;
            ay = MUSHROOM_GRAVITY;
            movingStep = 1;

            DebugOut(L"[MUSHROOM] Done Rising → Step 1\n");
        }
        return;

    case 1:
    {
        vy += ay * dt;

        vector<LPCOLLISIONEVENT> coEvents;
        CCollision::GetInstance()->Scan(this, dt, coObjects, coEvents);

        bool landed = false;

        for (auto& e : coEvents)
        {
            if (!e->obj->IsBlocking()) continue;

            if (e->ny < 0)
            {
                vy = 0;
                landed = true;
                DebugOut(L"[MUSHROOM] Landed on platform at y = %.2f\n", y);
                break;
            }
        }

        if (landed)
        {
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            float mx, my;
            scene->GetPlayer()->GetPosition(mx, my);
            vx = (mx < x) ? MUSHROOM_SPEED : -MUSHROOM_SPEED;

            movingStep = 2;
            DebugOut(L"[MUSHROOM] Start moving → Step 2 | vx = %.3f\n", vx);
        }
        else
        {
            y += dy;
        }

        for (auto e : coEvents) delete e;
        break;
    }

    case 2: 
        vy += ay * dt;
        CCollision::GetInstance()->Process(this, dt, coObjects);
        break;
    }
}

void CItemMushroom::OnNoCollision(DWORD dt)
{
    x += dx;
    y += dy;
}

void CItemMushroom::Render()
{
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

void CItemMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - MUSHROOM_WIDTH / 2;
    t = y - MUSHROOM_HEIGHT / 2;
    r = l + MUSHROOM_WIDTH;
    b = t + MUSHROOM_HEIGHT;
}

void CItemMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;

    if (e->ny < 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx; 
        DebugOut(L"[MUSHROOM] Hit wall → reverse vx = %.3f\n", vx);
    }
}
