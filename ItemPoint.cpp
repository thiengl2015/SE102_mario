#include "ItemPoint.h"
#include "Sprites.h"
#include "HUD.h"

CItemPoint::CItemPoint(float x, float y, int spriteId, int value) : CGameObject(x, y)
{
    this->spriteId = spriteId;
	this->value = value;
    spawn_time = GetTickCount64();
}

void CItemPoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    y -= 0.1f * dt;

    if (!hasAddedScore)
    {
        CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
        if (scene)
        {
            for (auto obj : scene->GetObjects())
            {
                CHud* hud = dynamic_cast<CHud*>(obj);
                if (hud)
                {
                    hud->AddScore(value);
                    hasAddedScore = true;
                    break;
                }
            }
        }
    }

    if (GetTickCount64() - spawn_time > ITEM_POINT_LIFETIME)
        this->Delete();
}

void CItemPoint::Render()
{
    if (spriteId >= 0)
        CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

void CItemPoint::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y;
    r = x + ITEM_POINT_WIDTH;
    b = y + ITEM_POINT_HEIGHT;
}
