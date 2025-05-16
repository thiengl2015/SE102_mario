#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "turle.h"
#include "PipeTeleport.h"
#include "RedGoomba.h"
#include "PiranhaPlant.h"
#include "Brick.h"
#include "ItemMushroom.h"
#include "Bullet.h"
#include "ItemLeaf.h"

#include "Collision.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ((vx > 0 && ax < 0) || (vx < 0 && ax > 0))
		isSkidding = true;
	else
		isSkidding = false;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	if (ax == 0)
	{
		if (vx > 0)
		{
			SetState(MARIO_STATE_WALKING_RIGHT);
			vx -= MARIO_DECELERATION * dt;
			if (vx < 0) vx = 0;
		}
		else if (vx < 0)
		{
			SetState(MARIO_STATE_WALKING_LEFT);
			vx += MARIO_DECELERATION * dt;
			if (vx > 0) vx = 0;
		}
	}

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (isEnteringPipe && GetTickCount64() - pipe_start_time > 2500)
	{
		SetPosition(pipe_dest_x, pipe_dest_y);
		isEnteringPipe = false;
		vy = 0.005f;

		float camX = pipe_dest_x - CGame::GetInstance()->GetBackBufferWidth() / 2;
		float camY = pipe_dest_y - CGame::GetInstance()->GetBackBufferHeight() / 2;
		if (camY < 0) camY = 0;
		CGame::GetInstance()->SetCamPos(camX, camY);
	}

	if (isEnteringPipe)
	{
		vx = 0;
		ax = 0;
		ay = 0;

		vy = pipe_is_going_up ? -0.01f : 0.01f;

		if (GetTickCount64() - pipe_start_time > 1500)
		{
			isEnteringPipe = false;
			isExitingPipe = true;
			pipe_exit_start_time = GetTickCount64();

			SetPosition(pipe_dest_x, pipe_dest_y + (pipe_is_going_up ? 16 : -16));
			vy = pipe_is_going_up ? -0.001f : 0.001f;

			return;
		}

		y += vy * dt;
		return;
	}

	if (isExitingPipe)
	{
		vx = 0;
		ay = 0;
		vy = pipe_is_going_up ? -0.015f : 0.015f;

		if (GetTickCount64() - pipe_exit_start_time > MARIO_TRANSFORM_DURATION)
		{
			isExitingPipe = false;
			vy = 0;
			ay = MARIO_GRAVITY;
		}

		y += vy * dt;
		return;
	}

	if (isTransforming)
	{
		vx = vy = ax = ay = 0;

		if (GetTickCount64() - transform_start_time > MARIO_TRANSFORM_DURATION)
		{
			isTransforming = false;
			ay = MARIO_GRAVITY;

			if (nextLevel != -1)
			{
				SetLevel(nextLevel);
				nextLevel = -1;
			}
		}
		return;
	}

	if (y > 230 && x < 2013)
	{
		SetState(MARIO_STATE_DIE);
	}
}


void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CTurtle*>(e->obj))
		OnCollisionWithTurtle(e);
	else if (dynamic_cast<CPipeTeleport*>(e->obj))
		((CPipeTeleport*)e->obj)->OnCollisionWith(this);
	else if (dynamic_cast<CRedGoomba*>(e->obj))
		OnCollisionWithRedGoomba(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);

	else if (dynamic_cast<CBullet*>(e->obj))
	{
		if (untouchable == 0)
			OnAttacked();
	}

	else if (dynamic_cast<CBrick*>(e->obj))
		e->obj->OnCollisionWith(e);
	else if (dynamic_cast<CItemMushroom*>(e->obj))
	{
		if (level == MARIO_LEVEL_SMALL)
			StartTransforming(MARIO_LEVEL_BIG);
		e->obj->Delete();
	}
	else if (dynamic_cast<CItemLeaf*>(e->obj))
	{
		if (level == MARIO_LEVEL_BIG)
			StartTransforming(MARIO_LEVEL_RACCOON);
		e->obj->Delete();
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else
	{
		if (untouchable == 0 && goomba->GetState() != GOOMBA_STATE_DIE)
		{
			OnAttacked();
		}
	}
}


void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithRedGoomba(LPCOLLISIONEVENT e)
{
	CRedGoomba* redgoomba = dynamic_cast<CRedGoomba*>(e->obj);
	if (e->ny < 0)
	{
		if (redgoomba->GetState() == RED_GOOMBA_STATE_WINGED)
		{
			redgoomba->SetState(RED_GOOMBA_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (redgoomba->GetState() == RED_GOOMBA_STATE_WALKING)
		{
			redgoomba->SetState(RED_GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else
	{
		if (untouchable == 0 && redgoomba->GetState() != RED_GOOMBA_STATE_DIE)
		{
			OnAttacked();
		}
	}
}


void CMario::OnCollisionWithTurtle(LPCOLLISIONEVENT e)
{
	CTurtle* turtle = dynamic_cast<CTurtle*>(e->obj);

	if (e->ny < 0)
	{
		if (!turtle->IsShellState())
		{
			turtle->StartShell();
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (!turtle->IsBeingHeld())
		{
			turtle->KickShell(nx);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else
	{
		if (turtle->IsShellState())
		{
			if (turtle->getvx() != 0)
			{
				OnAttacked();
			}
			else
			{
				turtle->KickShell(nx);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
		else if (untouchable == 0)
		{
			OnAttacked();
		}
	}
}

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* piranha = dynamic_cast<CPiranhaPlant*>(e->obj);

	if (untouchable == 0 && piranha->GetState() != PIRANHA_PLANT_STATE_IDLE)
	{
		OnAttacked();
	}
}

int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (isSkidding)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else 
			{
				if (isSkidding)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_SMALL_IDLE_RIGHT : ID_ANI_MARIO_SMALL_IDLE_LEFT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			aniId = (nx >= 0) ? ID_ANI_MARIO_JUMP_RUN_RIGHT : ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			aniId = (nx >= 0) ? ID_ANI_MARIO_JUMP_WALK_RIGHT : ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else if (isSitting)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_SIT_RIGHT : ID_ANI_MARIO_SIT_LEFT;
	}
	else if (vx == 0)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (isSkidding)
			aniId = ID_ANI_MARIO_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (isSkidding)
			aniId = ID_ANI_MARIO_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_WALKING_LEFT;
	}

	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;

	return aniId;
}


void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (isEnteringPipe || isExitingPipe)
		aniId = ID_ANI_MARIO_ENTER_PIPE;
	else if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	else if (level == MARIO_LEVEL_RACCOON)
		aniId = GetAniIdRaccoon(); 

	if (isTransforming)
	{
		if (
			(level == MARIO_LEVEL_SMALL && nextLevel == MARIO_LEVEL_BIG) ||
			(level == MARIO_LEVEL_BIG && nextLevel == MARIO_LEVEL_SMALL)
			)
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_TRANSFORM_RIGHT : ID_ANI_MARIO_TRANSFORM_LEFT;
		}
		else if (
			(level == MARIO_LEVEL_BIG && nextLevel == MARIO_LEVEL_RACCOON) ||
			(level == MARIO_LEVEL_RACCOON && nextLevel == MARIO_LEVEL_BIG)
			)
		{
			aniId = ID_ANI_MARIO_TRANSFORM_TO_RACCOON;
		}
		else
		{
			aniId = (nx > 0) ? ID_ANI_MARIO_TRANSFORM_RIGHT : ID_ANI_MARIO_TRANSFORM_LEFT;
		}

		animations->Get(aniId)->Render(x, y - 8.0f);
		return;
	}
	if (untouchable)
	{
		if ((GetTickCount64() / 100) % 2 == 0)
			animations->Get(aniId)->Render(x, y);
	}
	else
	{
		animations->Get(aniId)->Render(x, y);
	}


	animations->Get(aniId)->Render(x, y);

	DebugOutTitle(L"Coins: %d", coin);
}

int CMario::GetAniIdRaccoon()
{
	int aniId = -1;

	if (isSitting)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_SIT_RIGHT : ID_ANI_MARIO_RACCOON_SIT_LEFT;
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
			aniId = (nx >= 0) ? ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		else
			aniId = (nx >= 0) ? ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT : ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
	}
	else if (vx == 0)
	{
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (isSkidding)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (isSkidding)
			aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
	}

	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;

	return aniId;
}


void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (!isSitting && isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;

			if (level == MARIO_LEVEL_BIG)
				y += MARIO_SIT_HEIGHT_ADJUST;
			else if (level == MARIO_LEVEL_RACCOON)
				y += MARIO_RACCOON_SIT_HEIGHT_ADJUST;
		}
		break;


	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;

			if (level == MARIO_LEVEL_BIG)
				y -= MARIO_SIT_HEIGHT_ADJUST;
			else if (level == MARIO_LEVEL_RACCOON)
				y -= MARIO_RACCOON_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		isDead = true;
		break;
	case MARIO_STATE_ENTER_PIPE:
		vx = 0;
		vy = 0.05f;
		break;

	}


	CGameObject::SetState(state);
	if (isTransforming) return;
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_RACCOON_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
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
	else // MARIO_LEVEL_SMALL
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	float oldHeight = 0, newHeight = 0;

	// Lấy chiều cao hiện tại
	if (level == MARIO_LEVEL_SMALL)
		oldHeight = MARIO_SMALL_BBOX_HEIGHT;
	else if (level == MARIO_LEVEL_BIG)
		oldHeight = MARIO_BIG_BBOX_HEIGHT;
	else if (level == MARIO_LEVEL_RACCOON)
		oldHeight = MARIO_RACCOON_BBOX_HEIGHT;

	// Lấy chiều cao level mới
	if (l == MARIO_LEVEL_SMALL)
		newHeight = MARIO_SMALL_BBOX_HEIGHT;
	else if (l == MARIO_LEVEL_BIG)
		newHeight = MARIO_BIG_BBOX_HEIGHT;
	else if (l == MARIO_LEVEL_RACCOON)
		newHeight = MARIO_RACCOON_BBOX_HEIGHT;

	float deltaHeight = newHeight - oldHeight;

	y -= deltaHeight / 2;

	level = l;
}




void CMario::StartPipeTeleport(float destX, float destY, bool goingUp)
{
	SetState(MARIO_STATE_ENTER_PIPE);

	pipe_dest_x = destX;
	pipe_dest_y = destY;
	pipe_start_time = GetTickCount64();
	isEnteringPipe = true;
	isExitingPipe = false;
	pipe_is_going_up = goingUp;

	float pipeWidth = 30.0f;
	float marioWidth = MARIO_BIG_BBOX_WIDTH;

	this->x = destX + (pipeWidth - marioWidth) / 2.0f;
}

void CMario::StartTransforming(int targetLevel)
{
	isTransforming = true;
	transform_start_time = GetTickCount64();
	nextLevel = targetLevel;
	StartUntouchable();
}

void CMario::OnAttacked()
{
	if (level == MARIO_LEVEL_RACCOON)
	{
		StartTransforming(MARIO_LEVEL_BIG); 
		StartUntouchable();
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		StartTransforming(MARIO_LEVEL_SMALL);
		StartUntouchable();
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}
}
