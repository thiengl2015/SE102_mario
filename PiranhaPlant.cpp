#include "PiranhaPlant.h"
#include "Game.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"

 

CPiranhaPlant::CPiranhaPlant(float x, float y) : CGameObject(x, y)
{
    bullet = nullptr; 
    fire_start = 0; 
    initialY = y; 
    fireCooldown = 0;
    SetState(PIRANHA_PLANT_STATE_IDLE);
}

void CPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y; 
    right = x + PIRANHA_PLANT_BBOX_WIDTH; 
    bottom = y + PIRANHA_PLANT_BBOX_HEIGHT; 
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();
    float marioX, marioY;
    mario->GetPosition(marioX, marioY);
    if (mario) 
    {
        float distance = abs(marioX - x); 


        if (distance <= PIRANHA_PLANT_DETECTION_RANGE)
        {
            if (state == PIRANHA_PLANT_STATE_IDLE )
            {
                SetState(PIRANHA_PLANT_STATE_MOVE_UP);
                nx = (marioX < x) ? -1 : 1;
            }
        }
        else
        {
            if (state == PIRANHA_PLANT_STATE_FIRE || state == PIRANHA_PLANT_STATE_MOVE_UP)
                SetState(PIRANHA_PLANT_STATE_MOVE_DOWN);
        }
    }


    if (state == PIRANHA_PLANT_STATE_MOVE_UP)
    {
        if (y > initialY - PIRANHA_PLANT_IDLE_OFFSET)
            y -= PIRANHA_PLANT_MOVE_SPEED * dt;
        else
            SetState(PIRANHA_PLANT_STATE_FIRE);
    }

    else if (state == PIRANHA_PLANT_STATE_MOVE_DOWN)
    {
        if (y < initialY+5) 
            y += PIRANHA_PLANT_MOVE_SPEED * dt;
        else
            SetState(PIRANHA_PLANT_STATE_IDLE);
    }

    else if (state == PIRANHA_PLANT_STATE_FIRE)
    {
        if (GetTickCount64() - fire_start > 2000) // Bắn đạn sau mỗi 1 giây
        {
            fire_start = GetTickCount64();
            fireCooldown = fire_start + 300; // Chờ đúng 0.3 giây

            if (!bullet || bullet->IsDeleted())
            {
                bullet = new CBullet(x, y, marioX, marioY-27);
            }
        }

        // Kiểm tra nếu đã hết thời gian chờ 0.3 giây sau khi bắn
        if (fireCooldown > 0 && GetTickCount64() >= fireCooldown)
        {
            SetState(PIRANHA_PLANT_STATE_MOVE_DOWN);
            fireCooldown = 0; // Reset thời gian chờ
        }
    }


    if (bullet) 
        bullet->Update(dt, coObjects);

    CGameObject::Update(dt, coObjects);
}


void CPiranhaPlant::Render()
{
    int aniId = 0;
    if (this->x < 300)
    {
         aniId = ID_ANI_PIRANHA_PLANT_LEFT;

        if (nx > 0)
        {
            aniId = ID_ANI_PIRANHA_PLANT_RIGHT;
            if (state == PIRANHA_PLANT_STATE_FIRE)
            {
                aniId = ID_ANI_PIRANHA_PLANT_FIRE_RIGHT;
            }
            else if (state == PIRANHA_PLANT_STATE_MOVE_UP || state == PIRANHA_PLANT_STATE_MOVE_DOWN)
            {
                aniId = ID_ANI_PIRANHA_PLANT_RIGHT;
            }
        }
        else if (nx < 0)
        {
            if (state == PIRANHA_PLANT_STATE_FIRE)
            {
                aniId = ID_ANI_PIRANHA_PLANT_FIRE_LEFT;
            }
            else if (state == PIRANHA_PLANT_STATE_MOVE_UP || state == PIRANHA_PLANT_STATE_MOVE_DOWN)
            {
                aniId = ID_ANI_PIRANHA_PLANT_LEFT;
            }
        }
    }
    else
    {
        aniId = ID_ANI_PIRANHA_PLANT_GREEN_LEFT;

        if (nx > 0)
        {
            aniId = ID_ANI_PIRANHA_PLANT_GREEN_RIGHT;
            if (state == PIRANHA_PLANT_STATE_FIRE)
            {
                aniId = ID_ANI_PIRANHA_PLANT_GREEN_FIRE_RIGHT;
            }
            else if (state == PIRANHA_PLANT_STATE_MOVE_UP || state == PIRANHA_PLANT_STATE_MOVE_DOWN)
            {
                aniId = ID_ANI_PIRANHA_PLANT_GREEN_RIGHT;
            }
        }
        else if (nx < 0)
        {
            if (state == PIRANHA_PLANT_STATE_FIRE)
            {
                aniId = ID_ANI_PIRANHA_PLANT_GREEN_FIRE_LEFT;
            }
            else if (state == PIRANHA_PLANT_STATE_MOVE_UP || state == PIRANHA_PLANT_STATE_MOVE_DOWN)
            {
                aniId = ID_ANI_PIRANHA_PLANT_GREEN_LEFT;
            }
        }
    }
    

    LPANIMATION ani = CAnimations::GetInstance()->Get(aniId);
    if (ani) 
        ani->Render(x, y);
    if (bullet && !bullet->IsDeleted())
        bullet->Render();
}

void CPiranhaPlant::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case PIRANHA_PLANT_STATE_FIRE: 
        fire_start = GetTickCount64();
        break;

    case PIRANHA_PLANT_STATE_IDLE:
        if (bullet)
            bullet->SetExist(false);
        break;

    case PIRANHA_PLANT_STATE_MOVE_UP: 
        vy = -PIRANHA_PLANT_MOVE_SPEED;
        break;

    case PIRANHA_PLANT_STATE_MOVE_DOWN:
        vy = PIRANHA_PLANT_MOVE_SPEED;
        break;
    }
}