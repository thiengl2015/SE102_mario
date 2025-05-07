#pragma once
#include "GameObject.h"

#define FLOWER_BBOX_WIDTH 30
#define FLOWER_BBOX_HEIGHT 55

#define ID_ANI_MOVE_RIGHT 6200
#define ID_ANI_FIRE_RIGHT 6201
#define ID_ANI_MOVE_LEFT 6202
#define ID_ANI_FIRE_LEFT 6203

#define FLOWER_STATE_IDLE 100    
#define FLOWER_STATE_FIRE 200    
#define FLOWER_STATE_HIDE 300   

class CFlower : public CGameObject
{
protected:
    ULONGLONG fire_start; 

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; }
    virtual int IsBlocking() { return 0; }
public:
    CFlower(float x, float y);
    void FireBullet();
    virtual void SetState(int state);
    bool IsMarioOnLeft(); 
};