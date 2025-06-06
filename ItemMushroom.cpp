﻿#include "ItemMushroom.h"
#include "Game.h"
#include "Animations.h"
#include "ItemPoint.h"
#include "Mario.h"
#include "PlayScene.h"
#include "BlockerWall.h"


CItemMushroom::CItemMushroom(float x, float y, int spriteId)
    : CGameObject(x, y)
{
    this->spriteId = spriteId;
    this->startY = y;
    this->pointSpriteId = ID_SPRITE_ITEM_POINT_1000;
    SetState(MUSHROOM_STATE_ON_RISE);
}

void CItemMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - MUSHROOM_WIDTH/2;
    top = y - MUSHROOM_HEIGHT/2;
    right = x + MUSHROOM_WIDTH/2;
    bottom = y + MUSHROOM_HEIGHT/2;
}

void CItemMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == MUSHROOM_STATE_ON_RISE)
    {
        y -= MUSHROOM_RISE_SPEED * dt;
        if (startY - y >= MUSHROOM_RISE_DISTANCE) 
        {
            SetState(MUSHROOM_STATE_FULL);
        }
    }
    else if (state == MUSHROOM_STATE_FULL)
    {
        vx = -MUSHROOM_SPEED; 
        ay = MUSHROOM_GRAVITY; 
    }

    vx += ax * dt;
    vy += ay * dt;

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CItemMushroom::Render()
{
    CSprites::GetInstance()->Get(ID_SPRITE_ITEM_MUSHROOM_RED)->Draw(x, y);
}

void CItemMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->obj->GetType() == TYPE_BLOCKERWALL)
    {
        this->Delete();
        return;
    }

    if (e->nx != 0)
        vx = -vx;
    if (e->ny != 0)
        vy = 0;

    if (dynamic_cast<CMario*>(e->obj)) {
        CMario* mario = dynamic_cast<CMario*>(e->obj);
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(new CItemPoint(x, y - 10, pointSpriteId, 1000));

        if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
            mario->StartTransforming(MARIO_LEVEL_BIG);
        }

        this->Delete();
    }
}


void CItemMushroom::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CItemMushroom::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case MUSHROOM_STATE_IDLE:
        vx = 0;
        vy = 0;
        riseDistance = 0;
        break;

    case MUSHROOM_STATE_ON_RISE:
        vx = 0;
        vy = -MUSHROOM_RISE_SPEED; 
        break;

    case MUSHROOM_STATE_FULL:
        vx = -MUSHROOM_SPEED; // Di chuyển sang trái
        ay = MUSHROOM_GRAVITY; // Bắt đầu chịu ảnh hưởng trọng lực
        break;
    }
}