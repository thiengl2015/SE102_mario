#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define ID_ANI_PIRANHA_PLANT_LEFT 5600
#define ID_ANI_PIRANHA_PLANT_FIRE_LEFT 5602
#define ID_ANI_PIRANHA_PLANT_RIGHT 5601
#define ID_ANI_PIRANHA_PLANT_FIRE_RIGHT 5603

#define ID_ANI_PIRANHA_PLANT_GREEN_LEFT 5605
#define ID_ANI_PIRANHA_PLANT_GREEN_FIRE_LEFT 5607
#define ID_ANI_PIRANHA_PLANT_GREEN_RIGHT 5606
#define ID_ANI_PIRANHA_PLANT_GREEN_FIRE_RIGHT 5608

#define PIRANHA_PLANT_BBOX_WIDTH 16
#define PIRANHA_PLANT_BBOX_HEIGHT 32

#define PIRANHA_PLANT_STATE_IDLE 100
#define PIRANHA_PLANT_STATE_FIRE 200
#define PIRANHA_PLANT_STATE_MOVE_UP 300
#define PIRANHA_PLANT_STATE_MOVE_DOWN 400
#define PIRANHA_PLANT_STATE_DIE 500

#define PIRANHA_PLANT_MOVE_SPEED 0.015f
#define PIRANHA_PLANT_IDLE_OFFSET 20 
#define PIRANHA_PLANT_DETECTION_RANGE 150

class CPiranhaPlant : public CGameObject
{
protected:
	CBullet* bullet;
	ULONGLONG fire_start;
	ULONGLONG fireCooldown = 0;
	int initialY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	ULONGLONG die_start = 0;
	int pointSpriteId;
public:
	CPiranhaPlant(float x, float y);
	virtual void SetState(int state);
};