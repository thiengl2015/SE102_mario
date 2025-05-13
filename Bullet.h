#pragma once
#include "GameObject.h"
#define BULLET_BBOX_WIDTH 8
#define BULLET_BBOX_HEIGHT 10

#define ID_ANI_BULLET 5610

class CBullet : public CGameObject
{
public:
	bool isExist;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	CBullet(float x, float y);
	void SetExist(bool isExist) { this->isExist = isExist; }
	bool IsExist() { return isExist; }
};