#pragma once
#include "GameObject.h"

class CHalfSolidBlock : public CGameObject {
    int spriteId;
    float width, height;

public:
    CHalfSolidBlock(float x, float y, float width, float height, int spriteId);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    int IsBlocking() override;
    int IsDirectionColliable(float nx, float ny) override;
};
