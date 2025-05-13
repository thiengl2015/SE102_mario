#include "Brick.h"
#include "Mario.h"
#include "ItemPoint.h"
#include "Game.h"
#include "PlayScene.h"
#include "AssetIDs.h"
#include "ItemCoin.h"

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

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
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
    if (dynamic_cast<CMario*>(e->src_obj) && e->ny > 0)
    {
        isUsed = true;
        float spawnX = x;
        float spawnY = y - BRICK_BBOX_HEIGHT;
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

        scene->AddObject(new CItemPoint(spawnX, spawnY, pointSpriteId));
        switch (spawnType)
        {
        case 1: scene->AddObject(new CItemCoin(spawnX, spawnY)); break;
            /*case 2:
            {
                auto mushroom = new CItemMushroom(spawnX, y, itemSpriteId);
                mushroom->StartMovingUp();
                scene->AddObject(mushroom);
                break;
            }
            case 3:
            {
                auto leaf = new CItemLeaf(spawnX, y, itemSpriteId); // nếu có
                leaf->StartMovingUp();
                scene->AddObject(leaf);
                break;
            }
            */
        }
    }
}
