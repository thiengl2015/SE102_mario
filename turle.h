#pragma once
#include "GameObject.h"
#include "Mario.h"

#define TURTLE_GRAVITY 0.002f
#define TURTLE_WALKING_SPEED 0.05f

#define TURTLE_BBOX_WIDTH 16
#define TURTLE_BBOX_HEIGHT 14
#define TURTLE_BBOX_HEIGHT_DIE 7

#define ID_ANI_TURTLE_WALKING_RIGHT 5200
#define ID_ANI_TURTLE_WALKING_LEFT 5201
#define ID_ANI_TURTLE_SHELL 5202

#define TURTLE_STATE_WALKING 100
#define TURTLE_STATE_SHELL 300
#define TURTLE_STATE_SHELL_MOVING 400
#define TURTLE_STATE_HELD 500
#define TURTLE_STATE_REVIVING 600

#define TURTLE_REVIVE_TIMEOUT 5000

class CTurtle : public CGameObject
{
protected:
	float ax, ay;
	int walkingDirection;
	bool isBeingHeld;
	ULONGLONG shell_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CTurtle(float x, float y);
	virtual void SetState(int state);
	bool checkMario();
	void StartShell();
	void KickShell(int dir);
	bool IsShellState();
	bool IsBeingHeld();
	float getvx() { return vx; }
}; 