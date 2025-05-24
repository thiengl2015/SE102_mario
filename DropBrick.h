#pragma once
#include "Platform.h"

#define DROPBRICK_SPRITE_ID 41001

#define DROPBRICK_WIDTH 48
#define DROPBRICK_HEIGHT 16

#define DROPBRICK_DROP_SPEED 0.1f
#define DROPBRICK_GRAVITY 0.01f


class CDropBrick : public CGameObject
{
protected:
	bool isMarioOn = false;
	float ax;
	float ay;
	bool isFalling = false;
public:
	CDropBrick(float x, float y) : CGameObject(x, y)
	{
		this->isMarioOn = false;
		this->vy = 0;
		this->ax = DROPBRICK_GRAVITY;
		this->ay = DROPBRICK_GRAVITY;
		this->isFalling = false;
	}

	void Update(DWORD dt);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	int IsBlocking() override { return 1; }
	void OnNoCollision(DWORD dt);
	int IsCollidable() override { return 1; }

};

