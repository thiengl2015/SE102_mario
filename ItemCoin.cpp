#include "ItemCoin.h"
#include "Animations.h"
#include "PlayScene.h"
#include "ItemPoint.h"

CItemCoin::CItemCoin(float x, float y) : CGameObject(x, y)
{
    this->pointSpriteId = COIN_POINT_ID;
    this->startY = y;
    isRising = true;
    isFalling = false;
}

void CItemCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (isRising)
    {
        y -= RISE_SPEED * dt;
        if (startY - y >= RISE_DISTANCE)
        {
            y = startY - RISE_DISTANCE; 
            isRising = false;
            isFalling = true;
        }
    }
    else if (isFalling)
    {
        y += FALL_SPEED * dt;
        if (y - (startY - RISE_DISTANCE) >= FALL_DISTANCE)
        {
            float pointY = startY - RISE_DISTANCE + FALL_DISTANCE;
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            scene->AddObject(new CItemPoint(x, pointY, pointSpriteId));

            this->Delete();
            return;
        }
    }
}

void CItemCoin::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_ITEM_COIN)->Render(x, y);
}

void CItemCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + ITEM_COIN_WIDTH;
    b = y + ITEM_COIN_HEIGHT;
}
