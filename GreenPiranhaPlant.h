#pragma once
#include "PiranhaPlant.h"

#define ID_ANI_GREEN_PIRANHA_PLANT 5604

#define GREEN_PIRANHA_PLANT_BBOX_WIDTH 16
#define GREEN_PIRANHA_PLANT_BBOX_HEIGHT 32

#define GREEN_PIRANHA_PLANT_STATE_IDLE 100
#define GREEN_PIRANHA_PLANT_STATE_ATTACK 200
#define GREEN_PIRANHA_PLANT_STATE_MOVE_UP 300
#define GREEN_PIRANHA_PLANT_STATE_MOVE_DOWN 400
#define GREEN_PIRANHA_PLANT_STATE_DIE 500

class CGreenPiranhaPlant : public CPiranhaPlant
{
protected:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) override;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	virtual void Render() override;
	virtual int IsCollidable() override { return 1; }
	virtual int IsBlocking() override { return 0; }
public:
	CGreenPiranhaPlant(float x, float y);
	virtual void SetState(int state) override;
	virtual void isTailHit();
};
