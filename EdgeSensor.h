#pragma once
#include "GameObject.h"
#include "Block.h"
class CEdgeSensor : public CGameObject {
public:
    CEdgeSensor(float x, float y) : CGameObject(x, y) {}

    float GetVy() { return vy; }
    float GetY() { return y; }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {
        vy += 0.0f * dt; 
        CGameObject::Update(dt, coObjects);
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }

    void GetBoundingBox(float& l, float& t, float& r, float& b) override {
        l = x - 3;
        t = y;            
        r = x + 3;
        b = y + 3;

    }

    void Render() override {}

    int IsBlocking() override { return 0; }

    bool IsOnHalfSolidBlock(vector<LPGAMEOBJECT>* coObjects) {
        float sensorL, sensorT, sensorR, sensorB;
        GetBoundingBox(sensorL, sensorT, sensorR, sensorB);

        for (LPGAMEOBJECT obj : *coObjects)
        {
            float l, t, r, b;
            obj->GetBoundingBox(l, t, r, b);

            if ((dynamic_cast<CHalfSolidBlock*>(obj) || obj->IsBlocking() == 1) &&
                sensorR >= l && sensorL <= r && sensorB >= t && sensorT <= b)
            {
                return true;
            }
        }

        return false;

    }

};
