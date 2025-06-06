#pragma once
#include "GameObject.h"
#include "StretchableBlockShadow.h"
#include "PlayScene.h"

class CStretchableBlock : public CGameObject {
    int sprites[9];
    int cellSize;
    int nWidth;    
    int nHeight;  

public:
    CStretchableBlock(float x, float y, int cellSize, int nWidth, int nHeight, int* spriteIds);

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {}
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    int IsBlocking() override { return 1; }
    int IsDirectionColliable(float nx, float ny) override {
        return (nx == 0 && ny == -1) ? 1 : 0;
    }
};
