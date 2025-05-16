#pragma once
#include "GameObject.h"

class CDecoration : public CGameObject {
    int spriteId;
public:
    CDecoration(float x, float y, int spriteId) : CGameObject(x, y) {
        this->spriteId = spriteId;
    }

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {} // không làm gì cả

    void Render() override {
        CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
    }

    void GetBoundingBox(float& l, float& t, float& r, float& b) override {
        l = t = r = b = 0;
    }

    int IsCollidable() override { return 0; }
    int IsBlocking() override { return 0; }
};
