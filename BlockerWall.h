#pragma once
#include "GameObject.h"

#define BLOCKER_WALL_WIDTH 16
#define BLOCKER_WALL_HEIGHT 400

class CBlockerWall : public CGameObject
{
public:
	CBlockerWall(float x, float y);
	void Render() override;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	int IsCollidable() override;
	int IsBlocking() override;
	int GetType() override { return TYPE_BLOCKERWALL; }

};