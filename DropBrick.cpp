#include "DropBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Sprites.h"

CDropBrick::CDropBrick(float x, float y) : CGameObject(x, y)
{
    isMarioOn = false;
    ax = 0;
    ay = DROPBRICK_DROP_SPEED; 
    vx = -DROPBRICK_DROP_SPEED; 
}

void CDropBrick::Update(DWORD dt)
{
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float marioL, marioT, marioR, marioB;
    mario->GetBoundingBox(marioL, marioT, marioR, marioB);

    float brickL, brickT, brickR, brickB;
    GetBoundingBox(brickL, brickT, brickR, brickB);


    isMarioOn = (marioB >= brickT && marioB <= brickT + 5 && marioR > brickL && marioL < brickR);


    if (isMarioOn)
    {
        vx = 0;
        vy += DROPBRICK_DROP_SPEED * dt;
    }
	DebugOut(L"[DROPBRICK] Vx: %d\n", vx);

    x += vx * dt;
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