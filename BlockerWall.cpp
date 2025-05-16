#include "BlockerWall.h"
#include "PlayScene.h"
#include "Mario.h"
#include "debug.h"
#include "Game.h"

CBlockerWall::CBlockerWall(float x, float y) : CGameObject(x, y) {}

void CBlockerWall::Render()
{
}

void CBlockerWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

void CBlockerWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLOCKER_WALL_WIDTH;
	b = y + BLOCKER_WALL_HEIGHT;
}

int CBlockerWall::IsCollidable() { return 1; }

int CBlockerWall::IsBlocking()
{
	LPGAMEOBJECT mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (dynamic_cast<CMario*>(mario))
		return 1; 
	return 0;
}