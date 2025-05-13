#include "ItemPoint.h"
#include "Sprites.h"

CItemPoint::CItemPoint(float x, float y, int spriteId) : CGameObject(x, y)
{
    this->spriteId = spriteId;
    spawn_time = GetTickCount64();
}

void CItemPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - spawn_time > ITEM_POINT_LIFETIME)
    {
        this->Delete();
        return;
    }

    y -= 0.1f * dt;
}

void CItemPoint::Render()
{
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

void CItemPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y;
    r = x + ITEM_POINT_WIDTH;
    b = y + ITEM_POINT_HEIGHT;
}
