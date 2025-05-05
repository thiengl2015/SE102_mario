#include "Block.h"
#include "Sprites.h"

CHalfSolidBlock::CHalfSolidBlock(float x, float y, float width, float height, int spriteId)
    : CGameObject(x, y), width(width), height(height), spriteId(spriteId) {
}

void CHalfSolidBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    // Do nothing
}

void CHalfSolidBlock::Render() {
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y);

}

void CHalfSolidBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height / 5;
}

int CHalfSolidBlock::IsBlocking() {
    return 1;
}

int CHalfSolidBlock::IsDirectionColliable(float nx, float ny) {
    return (nx == 0 && ny == -1) ? 1 : 0;
}
