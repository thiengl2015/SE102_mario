#include "DropBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Sprites.h"

CDropBrick::CDropBrick(float x, float y) : CGameObject(x, y)
{
    ay = 0.0f;
	vx = 0.0f;
	vy = 0.0f; 
	isActivated = false;
	activationRange = 150.0f; // Distance within which the brick activates
}

void CDropBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CMario* mario = (CMario*)((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (!mario) return;

    float mx, my;
    mario->GetPosition(mx, my);
    if (!isActivated && abs(mx - x) <= activationRange)
    {
        isActivated = true;
    }
    if (!isActivated)
        return;
    vy += ay * dt;

    // Cập nhật vị trí
    x += -0.05f * dt;
    y += vy * dt;
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