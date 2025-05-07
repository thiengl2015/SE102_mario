#pragma once
#include "GameObject.h"

#define BULLET_SPEED 0.15f  // Tốc độ di chuyển của viên đạn
#define BULLET_BBOX_WIDTH 9 // Kích thước hitbox theo chiều ngang
#define BULLET_BBOX_HEIGHT 11 // Kích thước hitbox theo chiều dọc
#define BULLET_LIFETIME 3000 // Đạn tồn tại tối đa 3 giây trước khi bị xóa

#define ID_ANI_BULLET 6210 // ID animation viên đạn

class CBullet : public CGameObject
{
protected:
    ULONGLONG startTime; // Thời điểm viên đạn được bắn
public:
    CBullet(float x, float y, int direction); // direction: hướng bay (trái/phải)
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};