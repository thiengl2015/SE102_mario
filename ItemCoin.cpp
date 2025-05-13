#include "ItemCoin.h"
#include "Animations.h"

CItemCoin::CItemCoin(float x, float y) : CGameObject(x, y)
{
    spawn_time = GetTickCount64();
}

void CItemCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - spawn_time > ITEM_COIN_LIFETIME)
    {
        this->Delete();
        return;
    }

    y -= 0.2f * dt; 
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
