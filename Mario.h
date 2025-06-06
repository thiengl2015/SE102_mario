#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "turle.h"
#include "debug.h"
#include "MarioTail.h"
#include "AssetIDs.h"

#define MARIO_WALKING_SPEED		0.05f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.003f
#define MARIO_ACCEL_RUN_X	0.00075f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.0012f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_ENTER_PIPE 2000

#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

#define ID_ANI_MARIO_ENTER_PIPE 850

#define ID_ANI_MARIO_HOLD_IDLE_RIGHT 810
#define ID_ANI_MARIO_HOLD_IDLE_LEFT 811

#define ID_ANI_MARIO_HOLD_WALK_RIGHT 812
#define ID_ANI_MARIO_HOLD_WALK_LEFT 813


// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_TRANSFORM_RIGHT 1700
#define ID_ANI_MARIO_TRANSFORM_LEFT 1701
#define ID_ANI_MARIO_TRANSFORM_TO_RACCOON  1702


#pragma endregion

#define GROUND_Y 160.0f
#define MARIO_DECELERATION 0.0001f


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_RACCOON 3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_TRANSFORM_DURATION 1000

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

#define MARIO_TRANSFORM_FREEZE_TIME 500
#define MARIO_UNTOUCHABLE_TIME 2000

#define POWERMETER_MAX 1.0f
#define POWERMETER_INCREASE 0.0004f
#define POWERMETER_DECREASE 0.0002f

#define FLY_MAX_DURATION 8000      
#define FLAP_IMPULSE -0.3f          
#define FLAP_ANIMATION_TIME 600    

#define ID_ANI_MARIO_RACCOON_FLY_RIGHT 2020
#define ID_ANI_MARIO_RACCOON_FLY_LEFT  2021


#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT   2000
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT    2001
#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 2002
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT  2003
#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 2004
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT  2005
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2006
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT  2007
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT  2008
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT   2009
#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2010
#define ID_ANI_MARIO_RACCOON_SIT_LEFT  2011
#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2012
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT  2013

#define ID_ANI_MARIO_RACCOON_HOLD_IDLE_RIGHT 2014
#define ID_ANI_MARIO_RACCOON_HOLD_IDLE_LEFT  2015
#define ID_ANI_MARIO_RACCOON_HOLD_WALK_RIGHT 2016
#define ID_ANI_MARIO_RACCOON_HOLD_WALK_LEFT  2017

#define ID_ANI_MARIO_RACCOON_ATTACK_RIGHT 2018
#define ID_ANI_MARIO_RACCOON_ATTACK_LEFT 2019

#define ID_ANI_MARIO_RACCOON_FLY_RIGHT 2020
#define ID_ANI_MARIO_RACCOON_FLY_LEFT  2021

#define MARIO_RACCOON_BBOX_WIDTH  20
#define MARIO_RACCOON_BBOX_HEIGHT 26
#define MARIO_RACCOON_SITTING_BBOX_HEIGHT 16
#define MARIO_RACCOON_SIT_HEIGHT_ADJUST ((MARIO_RACCOON_BBOX_HEIGHT - MARIO_RACCOON_SITTING_BBOX_HEIGHT) / 2)

class CTurtle;
class CJumpingKoopas;
class CMarioTail;
class CFlyingKoopas;

class CMario : public CGameObject
{
protected:
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;

	int nextLevel = -1;

	bool isTransforming = false;
	ULONGLONG transform_start_time = 0;

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithTurtle(LPCOLLISIONEVENT e);
	void OnCollisionWithRedGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithJumpingKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithFlyingKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithDropBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBoomerangBrother(LPCOLLISIONEVENT e);
	void OnCollisionWithItemBox(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon(); 
	bool isSkidding = false;
	bool isHolding = false;

	bool isFlying = false;
	ULONGLONG flyStartTime = 0;
	ULONGLONG lastFlapTime = 0;
	float powerMeter = 0.0f;
	ULONGLONG flyingDuration = 0;
	bool isAutoWalking = false;
	CGameObject* stickingObj = nullptr; 

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		time = 300;
		startTime = GetTickCount64();

	}
	CTurtle* heldTurtle = nullptr;
	CJumpingKoopas* heldKoopas = nullptr;
	CFlyingKoopas* heldFlyingKoopas = nullptr;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	int GetLevel() { return level; }
	int GetUntouchable() { return untouchable; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	//Pipe
	void StartPipeTeleport(float destX, float destY, bool goingup);

	bool isEnteringPipe = false;
	float pipe_dest_x = 0, pipe_dest_y = 0;
	ULONGLONG pipe_start_time = 0;
	bool IsEnteringPipe() const { return isEnteringPipe; }

	bool pipe_is_going_up = false;
	bool isExitingPipe = false;
	ULONGLONG pipe_exit_start_time = 0;

	bool isDead = false;
	bool IsDead() { return isDead; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void StartTransforming(int targetLevel);
	bool IsTransforming() const { return isTransforming; }
	int GetDirection() const { return nx; }

	bool IsSitting() const { return isSitting; }
	bool IsOnPlatform() const { return isOnPlatform; }
	float GetAx() const { return ax; }

	float GetX() const { return x; }
	float GetY() const { return y; }
	void SetVY(float vy) { this->vy = vy; }
	void OnAttacked();
	int GetType() override { return TYPE_MARIO; }

	void PickOrThrowTurtle(); 
	bool isPressingA = false;
	void PickOrThrowKoopas();

	ULONGLONG tail_attack_start = 0;
	bool isTailAttacking = false;
	CMarioTail* tailAttack = nullptr;

	bool IsAutoWalking() const { return isAutoWalking; }
	void SetAutoWalking(bool value) { isAutoWalking = value; }
	int time;
	ULONGLONG startTime;
	void SetIsStickToPlatform(CGameObject* obj) { stickingObj = obj; }
	bool IsStickToPlatform() const { return stickingObj != nullptr; }
	void SetIsOnPlatform(bool value) { isOnPlatform = value; }
};