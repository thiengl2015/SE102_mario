#include "BoomerangBrother.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Collision.h"
#include "Animations.h"
#include <cmath>

CBoomerang::CBoomerang(float x, float y, int direction) : CGameObject(x, y) {
    this->startX = x;
    this->startY = y;
    this->direction = direction;
    vx = direction * BOOMERANG_SPEED_X;
    vy = BOOMERANG_SPEED_Y;
    step = 0;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    vy += ay * dt;
    CGameObject::Update(dt, coObjects);

    x += dx;
    y += dy;

    if (step == 0 && abs(x - startX) >= BOOMERANG_MAX_DISTANCE) {
        vx = -vx;
        step = 1;
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBoomerang::Render() {
    CAnimations::GetInstance()->Get(ID_ANI_BOOMERANG)->Render(x, y);
}

void CBoomerang::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + BOOMERANG_BBOX_WIDTH;
    b = y + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (!e->obj->IsBlocking()) return;

    if (dynamic_cast<CMario*>(e->obj)) {
        CMario* mario = dynamic_cast<CMario*>(e->obj);
        if (mario->GetUntouchable() == 0) {
            mario->OnAttacked();
            this->Delete();
        }
    }
}
