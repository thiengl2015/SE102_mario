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
#include "JumpingKoopas.h"
#include "ItemPoint.h"
#include "Collision.h"
#include "FlyingKoopas.h"
#include "DropBrick.h"
#include "BoomerangBrother.h"
#include "HUD.h"
#include "BlueSwitch.h"
#include "ItemBox.h"
#include "ItemBoxEffect.h"
#include "HUD.h"

int CMario::savedLevel = MARIO_LEVEL_SMALL;

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
  DebugOut(L"Mario vx: %f\n", vx);
	ULONGLONG now = GetTickCount64();
	int elapsed = (int)((now - startTime) / 1000);
	time = max(0, 300 - elapsed);
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (scene)
	{
		for (auto obj : scene->GetObjects())
		{
			CHud* hud = dynamic_cast<CHud*>(obj);
			if (hud)
			{
				hud->SetTime(time);
				hud->SetMarioVx(vx);
			}
		}
	}


	if (isTailAttacking)
	{
		if (isSitting && level == MARIO_LEVEL_RACCOON)
		{
			if (tailAttack)
			{
				tailAttack->Delete();
				tailAttack = nullptr;
			}
			isTailAttacking = false;
		}
		if (GetTickCount64() - tail_attack_start > TAIL_ATTACK_DURATION)
		{
			if (tailAttack) {
				tailAttack->Delete();
				tailAttack = nullptr;
			}
			isTailAttacking = false;
		}
	}
	if (state == MARIO_STATE_RUNNING_LEFT)
	{
		maxVx = vx + ax * dt / 5;
	}
	else if (state == MARIO_STATE_RUNNING_RIGHT)
	{
		maxVx = vx + ax * dt / 5;
	}
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > 0.3f) vx = nx * 0.3f;

	if (level == MARIO_LEVEL_RACCOON)
	{
		if (abs(vx) >= MARIO_RUNNING_SPEED * 0.9f && isOnPlatform)
			powerMeter = min(powerMeter + POWERMETER_INCREASE * dt, POWERMETER_MAX);
		else if (!isFlying)
			powerMeter = max(powerMeter - POWERMETER_DECREASE * dt, 0.0f);
	}

	if (isFlying && GetTickCount64() - flyStartTime > flyingDuration)
		isFlying = false;

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

	if (heldTurtle)
	{
		heldTurtle->SetPosition(x + nx * 16.0f, y);
	}
	if (heldKoopas)
	{
		heldKoopas->SetPosition(x + nx * 16.0f, y);
	}

	if (isOnPlatform && GetTickCount64() - flyStartTime > 500)
	{
		isFlying = false;
	}

	if (!isOnPlatform && vy > 0 && isFlying && GetTickCount64() - lastFlapTime < 200)
	{
		vy = 0.012f; 
	}
	if (isAutoWalking) {
		vx = MARIO_WALKING_SPEED;
		ax = 0;
		ay = MARIO_GRAVITY;
		SetState(MARIO_STATE_WALKING_RIGHT);
	}
	if (stickingObj)
	{
		y += stickingObj->GetVy() * dt;

		float ml, mt, mr, mb, bl, bt, br, bb;
		GetBoundingBox(ml, mt, mr, mb);
		stickingObj->GetBoundingBox(bl, bt, br, bb);

		bool outOfBounds = mr < bl || ml > br || abs(mb - bt) > 1.0f;

		if (outOfBounds)
		{
			stickingObj = nullptr;
		}
		else
		{
			isOnPlatform = true;
		}
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
	else if (dynamic_cast<CFlyingKoopas*>(e->obj))
		OnCollisionWithFlyingKoopas(e);  
	else if (dynamic_cast<CSwitchBlock*>(e->obj))
		e->obj->OnCollisionWith(e);
	else if (dynamic_cast<CDropBrick*>(e->obj))
			OnCollisionWithDropBrick(e);
	else if (dynamic_cast<CItemBox*>(e->obj))
		OnCollisionWithItemBox(e);

	else if (dynamic_cast<CBullet*>(e->obj))
	{
		if (untouchable == 0)
			OnAttacked();
	}

	else if (dynamic_cast<CBrick*>(e->obj))
		e->obj->OnCollisionWith(e);
	else if (dynamic_cast<CItemMushroom*>(e->obj))
	{

	}
	else if (dynamic_cast<CItemLeaf*>(e->obj))
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(new CItemPoint(x, y - 10, ID_SPRITE_ITEM_POINT_1000, 1000));

		if (level == MARIO_LEVEL_BIG)
			StartTransforming(MARIO_LEVEL_RACCOON);

		e->obj->Delete();
	}

	else if (dynamic_cast<CJumpingKoopas*>(e->obj))
		OnCollisionWithJumpingKoopas(e);
	else if (dynamic_cast<CBoomerangBrother*>(e->obj)) {
		OnCollisionWithBoomerangBrother(e);
	}

}
void CMario::OnCollisionWithDropBrick(LPCOLLISIONEVENT e)
{
	if (e->ny > 0) {
		CDropBrick* brick = dynamic_cast<CDropBrick*>(e->obj);
		if (brick) {
			brick->SetMario();
			if (!brick->isFalling) {
				brick->isFalling = true;
				brick->hasMario = true;
				vx = 0;
				vy = DROPBRICK_FALL_VY;
				SetIsStickToPlatform(brick);
			}
		}
	}
}

void CMario::OnCollisionWithBoomerangBrother(LPCOLLISIONEVENT e)
{
	CBoomerangBrother* bb = dynamic_cast<CBoomerangBrother*>(e->obj);
	if (!bb) return;

	if (e->ny < 0)
	{
		bb->Delete();
		vy = -MARIO_JUMP_DEFLECT_SPEED;

		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(new CItemPoint(bb->GetX(), bb->GetY() - 10, ID_SPRITE_ITEM_POINT_1000, 1000));
	}
	else
		OnAttacked();
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
	CCoin* coin = dynamic_cast<CCoin*>(e->obj);
	if (!coin) return;

	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	if (!scene) return;

	for (auto obj : scene->GetObjects())
	{
		CHud* hud = dynamic_cast<CHud*>(obj);
		if (hud)
		{
			hud->AddScore(100);                  
			hud->SetCoin(hud->GetCoin() + 1);   
			break;
		}
	}

	coin->Delete(); // Xóa coin sau khi ăn
}


void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	SetAutoWalking(false);
	CMario::savedLevel = level;
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
void CMario::PickOrThrowTurtle()
{
	if (heldTurtle)
	{
		float throwX = x + nx * 18.0f;
		float throwY = y + 5.0f;

		heldTurtle->SetPosition(throwX, throwY);

		float throwVx = nx * 0.25f; 
		float throwVy = 0.18f;     

		heldTurtle->SetVx(throwVx);
		heldTurtle->SetVy(throwVy);
		heldTurtle->SetAy(TURTLE_GRAVITY);

		heldTurtle->SetWalkingDirection(nx);
		heldTurtle->SetState(TURTLE_STATE_SHELL_MOVING);

		heldTurtle->SetBeingHeld(false);
		heldTurtle->SetHolder(nullptr);
		heldTurtle = nullptr;
		isHolding = false;
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
			if (turtle->GetState() != TURTLE_STATE_SHELL_MOVING) 
			{
				turtle->KickShell(nx);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}


	else
	{
		if (turtle->IsShellState())
		{
			if (turtle->GetState() == TURTLE_STATE_SHELL_MOVING)
			{
				if (!isPressingA || level == MARIO_LEVEL_SMALL)
				{
					OnAttacked();
				}
				else if (heldTurtle == nullptr && (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON))
				{
					heldTurtle = turtle;
					isHolding = true;
					turtle->SetState(TURTLE_STATE_HELD);
					turtle->SetBeingHeld(true);
					turtle->SetHolder(this);
				}
			}
			else
			{
				if (isPressingA && heldTurtle == nullptr && (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON))
				{
					heldTurtle = turtle;
					isHolding = true;
					turtle->SetState(TURTLE_STATE_HELD);
					turtle->SetBeingHeld(true);
					turtle->SetHolder(this);
				}
				else
				{
					turtle->KickShell(nx);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
		}
		else if (turtle->GetState() == TURTLE_STATE_WALKING)
		{
			if (untouchable == 0)
				OnAttacked();
		}
	}

}

void CMario::OnCollisionWithFlyingKoopas(LPCOLLISIONEVENT e)
{
	CFlyingKoopas* koopas = dynamic_cast<CFlyingKoopas*>(e->obj);
	if (!koopas) return;

	if (e->ny < 0)
	{
		if (koopas->GetState() == FKOOPAS_STATE_FLYING)
		{
			koopas->SetState(FKOOPAS_STATE_WALKING);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (!koopas->IsShellState())
		{
			koopas->SetState(FKOOPAS_STATE_SHELL);  
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (!koopas->IsBeingHeld())
		{
			if (koopas->GetState() != FKOOPAS_STATE_SHELL_MOVING)
			{
				koopas->KickShell(nx);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}

	else {
		if (koopas->IsShellState()) {
			if (koopas->GetState() == FKOOPAS_STATE_SHELL_MOVING) {
				if (!isPressingA || level == MARIO_LEVEL_SMALL) {
					OnAttacked();
				}
				else {
					heldFlyingKoopas = koopas;
					isHolding = true;
					koopas->SetState(FKOOPAS_STATE_HELD);
					koopas->SetBeingHeld(true);
					koopas->SetHolder(this);
				}
			}
			else {
				if (isPressingA && heldKoopas == nullptr) {
					heldFlyingKoopas = koopas;
					isHolding = true;
					koopas->SetState(FKOOPAS_STATE_HELD);
					koopas->SetBeingHeld(true);
					koopas->SetHolder(this);
				}
				else {
					koopas->KickShell(nx);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
		}
		else if (
			koopas->GetState() == FKOOPAS_STATE_WALKING ||
			koopas->GetState() == FKOOPAS_STATE_FLYING ||
			koopas->GetState() == FKOOPAS_STATE_REVIVING
			) {
			if (untouchable == 0)
				OnAttacked();
		}
	}
}


void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	CPiranhaPlant* piranha = dynamic_cast<CPiranhaPlant*>(e->obj);

	if (untouchable == 0 && piranha->GetState() != PIRANHA_PLANT_STATE_IDLE && piranha->GetState() != PIRANHA_PLANT_STATE_DIE)
	{
		OnAttacked();
	}
}

void CMario::OnCollisionWithJumpingKoopas(LPCOLLISIONEVENT e)
{
	CJumpingKoopas* koopas = dynamic_cast<CJumpingKoopas*>(e->obj);

	if (e->ny < 0)
	{
		if (koopas->GetState() == JKOOPAS_STATE_JUMPING)
		{
			koopas->SetState(JKOOPAS_STATE_WALKING); 
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (!koopas->IsShellState())
		{
			koopas->SetState(JKOOPAS_STATE_SHELL); 
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (!koopas->IsBeingHeld())
		{
			if (koopas->GetState() != JKOOPAS_STATE_SHELL_MOVING)
			{
				koopas->KickShell(nx);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else
			{
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else
	{
		if (koopas->IsShellState())
		{
			if (koopas->GetState() == JKOOPAS_STATE_SHELL_MOVING)
			{
				if (!isPressingA || level == MARIO_LEVEL_SMALL)
				{
					OnAttacked();
				}
				else if (!koopas->IsBeingHeld() && heldTurtle == nullptr && (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON))
				{
					heldKoopas = koopas;
					isHolding = true;
					koopas->SetState(JKOOPAS_STATE_HELD);
					koopas->SetBeingHeld(true);
					koopas->SetHolder(this);
				}
			}
			else
			{
				if (isPressingA && heldTurtle == nullptr && (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON))
				{
					heldKoopas = koopas;
					isHolding = true;
					koopas->SetState(JKOOPAS_STATE_HELD);
					koopas->SetBeingHeld(true);
					koopas->SetHolder(this);
				}
				else
				{
					koopas->KickShell(nx);
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				}
			}
		}
		else if (
			koopas->GetState() == JKOOPAS_STATE_WALKING ||
			koopas->GetState() == JKOOPAS_STATE_JUMPING ||
			koopas->GetState() == JKOOPAS_STATE_REVIVING 
			)
		{
			if (untouchable == 0)
				OnAttacked();
		}

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
	if (isHolding && (heldTurtle !=nullptr || heldKoopas != nullptr))
	{
		if (vx == 0)
			aniId = (nx > 0) ? ID_ANI_MARIO_HOLD_IDLE_RIGHT : ID_ANI_MARIO_HOLD_IDLE_LEFT;
		else
			aniId = (nx > 0) ? ID_ANI_MARIO_HOLD_WALK_RIGHT : ID_ANI_MARIO_HOLD_WALK_LEFT;

		return aniId;
	}
	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;

	return aniId;
}


void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	if (isEnd)
	{
		CSprites::GetInstance()->Get(ID_SPRITE_COURSE)->DrawStatic(100, 40);
		CSprites::GetInstance()->Get(ID_SPRITE_CLEAR)->DrawStatic(160, 40);
		CSprites::GetInstance()->Get(ID_SPRITE_YOU)->DrawStatic(70, 55);
		CSprites::GetInstance()->Get(ID_SPRITE_GOT)->DrawStatic(105, 55);
		CSprites::GetInstance()->Get(ID_SPRITE_A)->DrawStatic(130, 55);
		CSprites::GetInstance()->Get(ID_SPRITE_CARD)->DrawStatic(160, 55);
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		
		for (auto obj : scene->GetObjects())
		{
			CHud* hud = dynamic_cast<CHud*>(obj);
			if (hud)
			{
				// Lấy vật phẩm từ ItemBox[1] của HUD
				int type = hud->GetItemBox(1);

				// Hiển thị sprite tương ứng với loại vật phẩm
				int spriteId = (type == ITEM_TYPE_MUSHROOM) ? ID_SPRITE_ITEM_BOX_2:
					(type == ITEM_TYPE_FLOWER) ? ID_SPRITE_ITEM_BOX_3 :
					(type == ITEM_TYPE_STAR) ? ID_SPRITE_ITEM_BOX_1 :
					ID_SPRITE_ITEM_BOX_4; 

				CSprites::GetInstance()->Get(spriteId)->DrawStatic(200, 60);
			}
		}
	}

		
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
		if (isFlying && GetTickCount64() - lastFlapTime < FLAP_ANIMATION_TIME)
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_FLY_RIGHT : ID_ANI_MARIO_RACCOON_FLY_LEFT;
		else if (abs(ax) == MARIO_ACCEL_RUN_X)
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

	if (isHolding && (heldTurtle != nullptr || heldKoopas != nullptr))
	{
		if (vx == 0)
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_HOLD_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_HOLD_IDLE_LEFT;
		else
			aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_HOLD_WALK_RIGHT : ID_ANI_MARIO_RACCOON_HOLD_WALK_LEFT;

		return aniId;
	}

	if (aniId == -1)
		aniId = (nx > 0) ? ID_ANI_MARIO_RACCOON_IDLE_RIGHT : ID_ANI_MARIO_RACCOON_IDLE_LEFT;

	if (isTailAttacking)
		return (nx > 0) ? ID_ANI_MARIO_RACCOON_ATTACK_RIGHT : ID_ANI_MARIO_RACCOON_ATTACK_LEFT;

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
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
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
			if (abs(vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;

			isOnPlatform = false;

			if (level == MARIO_LEVEL_RACCOON && powerMeter > 0.1f)
			{
				isFlying = true;
				flyStartTime = GetTickCount64();
				flyingDuration = (FLY_MAX_DURATION * powerMeter);
			}

		}
		else if (isFlying)
		{
			vy = FLAP_IMPULSE;                     
			lastFlapTime = GetTickCount64();        
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
	if (heldTurtle)
	{
		heldTurtle->SetState(TURTLE_STATE_SHELL_MOVING);
		heldTurtle->SetVx(nx * TURTLE_SHELL_SLIDE_SPEED);
		heldTurtle->SetVy(-0.05f);
		heldTurtle->SetBeingHeld(false);
		heldTurtle->SetHolder(nullptr);
		heldTurtle = nullptr;
		isHolding = false;
	}

	if (heldKoopas)
	{
		heldKoopas->SetState(JKOOPAS_STATE_SHELL_MOVING);
		heldKoopas->SetVx(nx * JKOOPAS_SHELL_SLIDE_SPEED);
		heldKoopas->SetVy(-0.05f);
		heldKoopas->SetBeingHeld(false);
		heldKoopas->SetHolder(nullptr);
		heldKoopas = nullptr;
		isHolding = false;
	}
	if (isTransforming) return; 

	if (level == MARIO_LEVEL_RACCOON)
	{
		StartTransforming(MARIO_LEVEL_BIG);
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		StartTransforming(MARIO_LEVEL_SMALL);
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}

	StartUntouchable();
}

void CMario::PickOrThrowKoopas()
{
	if (heldKoopas)
	{
		float throwX = x + nx * 18.0f;
		float throwY = y + 5.0f;

		heldKoopas->SetPosition(throwX, throwY);

		float throwVx = nx * 0.25f;
		float throwVy = 0.18f;

		heldKoopas->SetVx(throwVx);
		heldKoopas->SetVy(throwVy);
		heldKoopas->SetAy(JKOOPAS_GRAVITY);

		heldKoopas->SetWalkingDirection(nx);
		heldKoopas->SetState(JKOOPAS_STATE_SHELL_MOVING);

		heldKoopas->SetBeingHeld(false);
		heldKoopas->SetHolder(nullptr);
		heldKoopas = nullptr;
		isHolding = false;
	}
}

void CMario::OnCollisionWithItemBox(LPCOLLISIONEVENT e)
{
	CItemBox* box = dynamic_cast<CItemBox*>(e->obj);
	if (!box || box->IsCollected()) return;

	DebugOut(L"[Mario] Collected ItemBox at x=%.1f\n", box->getX());

	int type = box->GetCurrentItemType(); 
	box->SetCollected(true);

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	for (auto obj : scene->GetObjects()) {
		CHud* hud = dynamic_cast<CHud*>(obj);
		if (hud) {
			hud->SetItemBox(0, type); 
		}
	}
	isEnd = true;
	scene->AddObject(new CItemBoxEffect(box->getX(), box->getY(), type));


	box->Delete();

	SetAutoWalking(true);
	vx = MARIO_WALKING_SPEED;
	ax = 0;
	

}
