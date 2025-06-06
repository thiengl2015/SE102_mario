#include "StretchableBlock.h"
#include "Sprites.h"

CStretchableBlock::CStretchableBlock(float x, float y, int cellSize, int nWidth, int nHeight, int* spriteIds)
    : CGameObject(x, y), cellSize(cellSize), nWidth(nWidth), nHeight(nHeight) {

    for (int i = 0; i < 9; i++) {
        sprites[i] = spriteIds[i];
    }

    int shadowSpriteIds[9] = {
        90029, 90028, 90027,
        90026, 90025, 90024,
        90023, 90022, 90021
    };

    CStretchableBlockShadow* shadow = new CStretchableBlockShadow(
        x, y, cellSize, nWidth, nHeight, shadowSpriteIds
    );
    ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(shadow);

}
void CStretchableBlock::Render() {
    CSprites* s = CSprites::GetInstance();

    for (int row = 0; row < nHeight; row++) {
        for (int col = 0; col < nWidth; col++) {
            int spriteIdx = 4;

            if (row == 0 && col == 0) spriteIdx = 0;             // Bottom Left
            else if (row == 0 && col == nWidth - 1) spriteIdx = 2; // Bottom Right
            else if (row == nHeight - 1 && col == 0) spriteIdx = 6; // Top Left
            else if (row == nHeight - 1 && col == nWidth - 1) spriteIdx = 8; // Top Right
            else if (row == 0) spriteIdx = 1;                   // Bottom
            else if (row == nHeight - 1) spriteIdx = 7;         // Top
            else if (col == 0) spriteIdx = 3;                   // Left
            else if (col == nWidth - 1) spriteIdx = 5;          // Right

            float drawX = x + col * cellSize;
            float drawY = y - row * cellSize;

            s->Get(sprites[spriteIdx])->Draw(drawX, drawY);
        }
    }
}

void CStretchableBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - cellSize / 2.0f;
    t = y - cellSize / 2.0f - (nHeight - 1) * cellSize;
    r = l + nWidth * cellSize;
    b = t + nHeight /5;
}
