#include "Flower.h"
#include "Bullet.h"
#include "Game.h"
#include "Mario.h"
#include "PlayScene.h"

CFlower::CFlower(float x, float y) : CGameObject(x, y)
{
    fire_start = GetTickCount64(); 
    SetState(FLOWER_STATE_IDLE);  
}

void CFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - FLOWER_BBOX_WIDTH / 2;
    top = y - FLOWER_BBOX_HEIGHT / 2;
    right = left + FLOWER_BBOX_WIDTH;
    bottom = top + FLOWER_BBOX_HEIGHT;
}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == FLOWER_STATE_FIRE)
    {
        if (GetTickCount64() - fire_start > 2000) 
        {
            FireBullet();
            fire_start = GetTickCount64(); 
        }
    }

    CGameObject::Update(dt, coObjects); 
}

void CFlower::Render()
{
    int aniId = ID_ANI_MOVE_RIGHT; 

    if (state == FLOWER_STATE_FIRE)
    {
        aniId = (IsMarioOnLeft() ? ID_ANI_FIRE_LEFT : ID_ANI_FIRE_RIGHT);
    }
    else if (state == FLOWER_STATE_IDLE)
    {
        aniId = (IsMarioOnLeft() ? ID_ANI_MOVE_LEFT : ID_ANI_MOVE_RIGHT);
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CFlower::FireBullet()
{
    int bulletDirection = (IsMarioOnLeft() ? -1 : 1);
    CBullet* bullet = new CBullet(x, y, bulletDirection);

    CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (scene)
    {
        scene->AddObject(bullet);
    }
}

void CFlower::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case FLOWER_STATE_IDLE:
        break;
    case FLOWER_STATE_FIRE:
        fire_start = GetTickCount64();
        break;
    case FLOWER_STATE_HIDE:
        break;
    }
}

bool CFlower::IsMarioOnLeft()
{
    CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (!scene || !scene->GetPlayer()) return false;

    return scene->GetPlayer()->GetX() < this->x;
}