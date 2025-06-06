#include "Goomba.h"
#include "PlayScene.h"
#include "ItemPoint.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"
#include "turle.h"

CGoomba::CGoomba(float x, float y, int pointSpriteId) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	spawmX = x - GOOMBA_ACTIVATE_DISTANCE;
	originalX = x;
	die_start = -1;
	this->pointSpriteId = pointSpriteId;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT_DIE/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CTurtle*>(e->obj))
	{
		CTurtle* turle = dynamic_cast<CTurtle*>(e->obj);
		if (turle->GetState() == TURTLE_STATE_SHELL_MOVING)
		{
			SetState(GOOMBA_STATE_DIE);	
		}
	}
	if (!e->obj->IsBlocking()) return;

	if (e->obj->GetType() == TYPE_BLOCKERWALL)
	{
		this->Delete();
		return;
	}

	if (dynamic_cast<CGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

}


void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)scene->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);

	if (marioX < x - GOOMBA_ACTIVATE_DISTANCE)
	{
		vx = 0;
		return;
	}
	else if (vx == 0 && state == GOOMBA_STATE_WALKING)
	{
		vx = -GOOMBA_WALKING_SPEED;
	}

	vy += ay * dt;
	vx += ax * dt;

	if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	float distanceToMario = abs(marioX - x);
	if (distanceToMario > GOOMBA_ACTIVATE_DISTANCE + 200 && state != GOOMBA_STATE_DIE)
	{
		x = originalX;
		vx = 0;
		SetState(GOOMBA_STATE_WALKING);
		return;
	}

}



void CGoomba::Render()
{
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x,y);
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;

		if (CGame::GetInstance()->GetCurrentScene() != nullptr)
		{
			CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
			if (scene)
			{
				scene->AddObject(new CItemPoint(x, y, pointSpriteId, 100));
			}
		}
		break;

	case GOOMBA_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	}
}