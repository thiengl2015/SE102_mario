#include "PiranhaPlant.h"
#include "Game.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"

#define PIRANHA_PLANT_MOVE_SPEED 0.02f
#define PIRANHA_PLANT_IDLE_OFFSET 20 
#define PIRANHA_PLANT_DETECTION_RANGE 100 

CPiranhaPlant::CPiranhaPlant(float x, float y) : CGameObject(x, y)
{
    bullet = nullptr; 
    fire_start = 0; 
    initialY = y; 
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

    if (mario) 
    {
        float marioX, marioY;
        mario->GetPosition(marioX, marioY);

        float distance = abs(marioX - x); 


        if (distance <= PIRANHA_PLANT_DETECTION_RANGE)
        {
            if (state == PIRANHA_PLANT_STATE_IDLE || state == PIRANHA_PLANT_STATE_MOVE_DOWN)
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
        if (y < initialY) 
            y += PIRANHA_PLANT_MOVE_SPEED * dt;
        else
            SetState(PIRANHA_PLANT_STATE_IDLE);
    }

    else if (state == PIRANHA_PLANT_STATE_FIRE && GetTickCount64() - fire_start > 1000)
    {
        fire_start = GetTickCount64(); 


        if (!bullet || bullet->IsDeleted())
        {
            bullet = new CBullet(x + (nx * BULLET_BBOX_WIDTH), y);
        }
    }

    if (bullet) 
        bullet->Update(dt, coObjects);

    CGameObject::Update(dt, coObjects);
}


void CPiranhaPlant::Render()
{
    int aniId = ID_ANI_PIRANHA_PLANT_LEFT; 

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