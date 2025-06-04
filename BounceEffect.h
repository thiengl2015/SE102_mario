#pragma once
#include "GameObject.h"

class CBounceEffect : public CGameObject {
protected:
    ULONGLONG start_time;
    int aniId;
    ULONGLONG duration;

public:
    CBounceEffect(float x, float y, int aniId, ULONGLONG duration = 1000) {
        this->x = x;
        this->y = y;
        this->aniId = aniId;
        this->duration = duration;
        this->start_time = GetTickCount64();
    }

    void Render() override {
        CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    }

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {
        if (GetTickCount64() - start_time > duration)
            this->Delete();
    }

    void GetBoundingBox(float& l, float& t, float& r, float& b) override {
        l = t = r = b = 0; 
    }

    int IsCollidable() override { return 0; }
    int IsBlocking() override { return 0; }
};
