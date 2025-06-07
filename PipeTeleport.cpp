#include "PipeTeleport.h"
#include "Mario.h"
#include "Textures.h"
#include "Game.h"

#define BBOX_ALPHA 0.25f
#define ID_TEX_BBOX -100

CPipeTeleport::CPipeTeleport(float x, float y, float width, float height, float tx, float ty)
    : CGameObject(x, y), width(width), height(height), targetX(tx), targetY(ty)
{
}

void CPipeTeleport::OnCollisionWith(CMario* mario)
{
    if (!isTriggered)
    {
        isTriggered = true;
        mario->StartPipeTeleport(targetX, targetY, targetY < y); 
    }
}

void CPipeTeleport::Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects)
{
}

void CPipeTeleport::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height;
}

void CPipeTeleport::Render()
{
}


