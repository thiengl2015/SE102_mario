#include "RacoonMario.h"
#include "Animations.h"

CMarioRaccoon::CMarioRaccoon(float x, float y) : CMario(x, y)
{
	SetLevel(MARIO_LEVEL_RACCOON); 
}


int CMarioRaccoon::GetAniIdRaccoon()
{
	int aniId = -1;
	if (!IsOnPlatform())
	{
		if (abs(GetAx()) == MARIO_ACCEL_RUN_X)
			aniId = (nx >= 0) ? ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		else
			aniId = (nx >= 0) ? ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
	}
	else if (IsSitting())
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_SIT_RIGHT : ID_ANI_MARIO_RACCOON_SIT_LEFT;
	}
	else if (vx == 0)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (GetAx() < 0)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
		else if (GetAx() == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
		else if (GetAx() == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
	}
	else
	{
		if (GetAx() > 0)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
		else if (GetAx() == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
		else if (GetAx() == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
	}

	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;

	return aniId;
}


void CMarioRaccoon::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (IsEnteringPipe() || isExitingPipe)
		aniId = ID_ANI_MARIO_ENTER_PIPE;
	else if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else
		aniId = GetAniIdRaccoon();

	if (IsTransforming())
	{
		aniId = (GetDirection() > 0) ? ID_ANI_MARIO_TRANSFORM_RIGHT : ID_ANI_MARIO_TRANSFORM_LEFT;
		animations->Get(aniId)->Render(x, y);
		return;
	}

	animations->Get(aniId)->Render(x, y);
}



void CMarioRaccoon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (IsSitting())
	{
		left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
		top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
		right = left + MARIO_RACCOON_BBOX_WIDTH;
		bottom = top + MARIO_RACCOON_SITTING_BBOX_HEIGHT;
	}
	else
	{
		left = x - MARIO_RACCOON_BBOX_WIDTH / 2;
		top = y - MARIO_RACCOON_BBOX_HEIGHT / 2;
		right = left + MARIO_RACCOON_BBOX_WIDTH;
		bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
	}
}

void CMarioRaccoon::SetState(int state)
{
	switch (state)
	{
	case MARIO_STATE_SIT:
		if (IsOnPlatform())
		{
			isSitting = true;
			vx = vy = 0;
			y += MARIO_RACCOON_SIT_HEIGHT_ADJUST;
		}
		return;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			y -= MARIO_RACCOON_SIT_HEIGHT_ADJUST;
		}
		return;
	}

	CMario::SetState(state);
}
