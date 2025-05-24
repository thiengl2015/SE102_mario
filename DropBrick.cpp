#include "DropBrick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Sprites.h"


void CDropBrick::Update(DWORD dt)
{
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    float marioL, marioT, marioR, marioB;
    mario->GetBoundingBox(marioL, marioT, marioR, marioB);

    float brickL, brickT, brickR, brickB;
    GetBoundingBox(brickL, brickT, brickR, brickB);

    // Kiểm tra xem Mario có đứng trên gạch không
    isMarioOn = (marioB >= brickT && marioB <= brickT + 2 && marioR > brickL && marioL < brickR);

    // Nếu Mario đứng lên thì gạch bắt đầu rơi
    if (isMarioOn)
    {
        isFalling = true;
    }

    if (isFalling)
    {
        vx = 0; // dừng di chuyển ngang
        vy += DROPBRICK_GRAVITY * dt;
    }
    else
    {
        vx = -0.05f; // di chuyển sang trái khi không có Mario
        vy = 0;
    }

    x += vx * dt;
    y += vy * dt;
}


void CDropBrick::Render()
{
    CSprites::GetInstance()->Get(DROPBRICK_SPRITE_ID)->Draw(x, y);
}

void CDropBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - DROPBRICK_WIDTH / 2;
    t = y - DROPBRICK_HEIGHT / 2;
    r = l + DROPBRICK_WIDTH;
    b = t + DROPBRICK_HEIGHT;
}
void CDropBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}