#include "Brick.h"
#include "Mario.h"
#include "ItemPoint.h"
#include "Game.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "ItemCoin.h"
#include "ItemMushroom.h"

#define ID_ANI_BRICK_QUESTION    21000
#define ID_ANI_BRICK_EMPTY       22000
#define ID_ANI_BRICK_PATTERN     23000

CBrick::CBrick(float x, float y, int brickType, int spawnType, int itemSpriteId, int pointSpriteId)
    : CGameObject(x, y)
{
    this->brickType = brickType;
    this->spawnType = spawnType;
    this->itemSpriteId = itemSpriteId;
    this->pointSpriteId = pointSpriteId;
}


void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (isBouncing)
    {
        DWORD now = GetTickCount64();
        DWORD elapsed = now - bounce_start;

        if (elapsed < BRICK_BOUNCE_DURATION / 2)
        {
            bounceOffsetY = -BRICK_BOUNCE_HEIGHT * (elapsed / (BRICK_BOUNCE_DURATION / 2.0f));
        }
        else if (elapsed < BRICK_BOUNCE_DURATION)
        {
            bounceOffsetY = -BRICK_BOUNCE_HEIGHT * (1 - (elapsed - BRICK_BOUNCE_DURATION / 2.0f) / (BRICK_BOUNCE_DURATION / 2.0f));
        }
        else
        {
            bounceOffsetY = 0;
            isBouncing = false;
        }
    }

    if (!isUsed)
        CCollision::GetInstance()->Process(this, dt, coObjects);
}



void CBrick::Render()
{
    int aniId = ID_ANI_BRICK_QUESTION;

    if (brickType == 0) {
        aniId = isUsed ? ID_ANI_BRICK_EMPTY : ID_ANI_BRICK_PATTERN;
    }
    else if (brickType == 1) {
        aniId = isUsed ? ID_ANI_BRICK_EMPTY : ID_ANI_BRICK_QUESTION;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y + bounceOffsetY);

}


void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BRICK_BBOX_WIDTH / 2;
    t = y - BRICK_BBOX_HEIGHT / 2;
    r = l + BRICK_BBOX_WIDTH;
    b = t + BRICK_BBOX_HEIGHT;
}


void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (isUsed || brickType != 1) return;

    if (dynamic_cast<CMario*>(e->src_obj) &&
        e->ny > 0 && 
        e->obj == this)
    {
        isUsed = true;

        if (!isBouncing)
        {
            isBouncing = true;
            bounce_start = GetTickCount64();
        }

        float spawnX = x;
        float spawnY = y - BRICK_BBOX_HEIGHT / 2;

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(new CItemPoint(spawnX, spawnY, pointSpriteId));

        switch (spawnType)
        {
        case 1: scene->AddObject(new CItemCoin(spawnX, spawnY)); break;
        case 2:
            {
            auto mushroom = new CItemMushroom(spawnX, spawnY, itemSpriteId);
            scene->AddObject(mushroom);
            break;
            }
        }
    }
}
