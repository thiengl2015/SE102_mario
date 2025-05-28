#include "GreenPiranhaPlant.h"
#include "PlayScene.h"
#include "PiranhaPlant.h"

CGreenPiranhaPlant::CGreenPiranhaPlant(float x, float y) : CPiranhaPlant(x, y)
{
	SetState(GREEN_PIRANHA_PLANT_STATE_IDLE);
}
void CGreenPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - GREEN_PIRANHA_PLANT_BBOX_WIDTH / 2;
	top = y - GREEN_PIRANHA_PLANT_BBOX_HEIGHT / 2;
	right = left + GREEN_PIRANHA_PLANT_BBOX_WIDTH;
	bottom = top + GREEN_PIRANHA_PLANT_BBOX_HEIGHT;
}
void CGreenPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
            if (state == GREEN_PIRANHA_PLANT_STATE_IDLE)
            {
                SetState(GREEN_PIRANHA_PLANT_STATE_MOVE_UP);
            }
        }
        else
        {
            if (state == GREEN_PIRANHA_PLANT_STATE_ATTACK || state == GREEN_PIRANHA_PLANT_STATE_MOVE_UP)
                SetState(GREEN_PIRANHA_PLANT_STATE_MOVE_DOWN);
        }
    }


    if (state == GREEN_PIRANHA_PLANT_STATE_MOVE_UP)
    {
        if (y > initialY - PIRANHA_PLANT_IDLE_OFFSET)
            y -= PIRANHA_PLANT_MOVE_SPEED * dt;
        else
            SetState(GREEN_PIRANHA_PLANT_STATE_ATTACK);
    }

    else if (state == GREEN_PIRANHA_PLANT_STATE_MOVE_DOWN)
    {
        if (y < initialY + 5)
            y += PIRANHA_PLANT_MOVE_SPEED * dt;
        else
            SetState(GREEN_PIRANHA_PLANT_STATE_IDLE);
    }
	else if (state == GREEN_PIRANHA_PLANT_STATE_ATTACK)
	{
        if (GetTickCount() - fire_start >= 2000)
        {
            SetState(GREEN_PIRANHA_PLANT_STATE_MOVE_DOWN);
        }
	}
}
void CGreenPiranhaPlant::Render()
{
	int aniId = ID_ANI_GREEN_PIRANHA_PLANT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CGreenPiranhaPlant::SetState(int state)
{
	CPiranhaPlant::SetState(state);
	if (state == GREEN_PIRANHA_PLANT_STATE_ATTACK)
	{
		fire_start = GetTickCount();
	}
}