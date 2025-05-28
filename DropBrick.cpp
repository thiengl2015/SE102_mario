#include "DropBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Sprites.h"

CDropBrick::CDropBrick(float x, float y) : CGameObject(x, y)
{
    vx = -0.05f; 
    vy = 0;
}

void CDropBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;
    y += vy * dt;
    CGameObject::Update(dt, coObjects);
}

void CDropBrick::Render()
{
    CAnimations::GetInstance()->Get(ANIMATION_DROPBRICK)->Render(x, y);
}

void CDropBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - DROPBRICK_BBOX_WIDTH / 2;
    t = y - DROPBRICK_BBOX_HEIGHT / 2;
    r = l + DROPBRICK_BBOX_WIDTH;
    b = t + DROPBRICK_BBOX_HEIGHT;
}
int CDropBrick::IsBlocking()
{
    LPGAMEOBJECT mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (dynamic_cast<CMario*>(mario))
        return 1;
    return 0;
}