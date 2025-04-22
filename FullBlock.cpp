#include "FullBlock.h"
#include "Sprites.h"

CFullBlock::CFullBlock(float x, float y, float width, float height, int spriteId)
    : CGameObject(x, y), width(width), height(height), spriteId(spriteId) {
}

void CFullBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}

void CFullBlock::Render() {
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

void CFullBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height;
}

int CFullBlock::IsBlocking() {
    return 1;
}

int CFullBlock::IsDirectionColliable(float nx, float ny) {
    return 1; 
}
