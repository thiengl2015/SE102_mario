#include "Bullet.h"
#include "Game.h"

CBullet::CBullet(float x, float y, int direction) : CGameObject(x, y)
{
    vx = direction * BULLET_SPEED; // Xác định hướng bay dựa vào direction
    startTime = GetTickCount64(); // Ghi lại thời điểm đạn được bắn
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt; // Đạn di chuyển theo hướng vx

    // Kiểm tra thời gian tồn tại của viên đạn
    if (GetTickCount64() - startTime > BULLET_LIFETIME)
    {
        isDeleted = true; // Sau thời gian quy định, viên đạn tự xóa
        return;
    }

}

void CBullet::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_BULLET)->Render(x, y);
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - BULLET_BBOX_WIDTH / 2;
    top = y - BULLET_BBOX_HEIGHT / 2;
    right = left + BULLET_BBOX_WIDTH;
    bottom = top + BULLET_BBOX_HEIGHT;
}