#pragma once
#include "GameObject.h"

class CEdgeSensor : public CGameObject {
public:
    CEdgeSensor(float x, float y) : CGameObject(x, y) {}

    float GetVy() { return vy; }
    float GetY() { return y; }
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {
        vy += 0.0f * dt; // cho sensor rơi nhẹ
        CGameObject::Update(dt, coObjects);
        CCollision::GetInstance()->Process(this, dt, coObjects);
    }

    void GetBoundingBox(float& l, float& t, float& r, float& b) override {
        l = x - 1; t = y - 1; r = x + 1; b = y + 1;
    }

    void Render() override {}

    int IsBlocking() override { return 0; }
};
