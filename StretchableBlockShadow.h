#include "GameObject.h"

class CStretchableBlockShadow : public CGameObject {
    int sprites[9];
    int cellSize;
    int nWidth;
    int nHeight;

public:
    CStretchableBlockShadow(float x, float y, int cellSize, int nWidth, int nHeight, int* spriteIds);
    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override {}
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {
        l = t = r = b = 0;
    }

    int IsBlocking() override { return 0; }
};
