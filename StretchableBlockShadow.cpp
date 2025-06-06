#include "StretchableBlockShadow.h"
#include "Sprites.h"

CStretchableBlockShadow::CStretchableBlockShadow(float x, float y, int cellSize, int nWidth, int nHeight, int* spriteIds)
    : CGameObject(x, y), cellSize(cellSize), nWidth(nWidth), nHeight(nHeight) {
    for (int i = 0; i < 9; i++) {
        sprites[i] = spriteIds[i]; 
    }
}

void CStretchableBlockShadow::Render() {
    CSprites* s = CSprites::GetInstance();

    for (int row = 0; row < nHeight; row++) {
        for (int col = 0; col < nWidth; col++) {
            int spriteIdx = 4;

            if (row == 0 && col == 0) spriteIdx = 0;
            else if (row == 0 && col == nWidth - 1) spriteIdx = 2;
            else if (row == nHeight - 1 && col == 0) spriteIdx = 6;
            else if (row == nHeight - 1 && col == nWidth - 1) spriteIdx = 8;
            else if (row == 0) spriteIdx = 1;
            else if (row == nHeight - 1) spriteIdx = 7;
            else if (col == 0) spriteIdx = 3;
            else if (col == nWidth - 1) spriteIdx = 5;

            float drawX = x + col * cellSize;
            float drawY = y - row * cellSize;

            s->Get(sprites[spriteIdx])->Draw(drawX + 8, drawY + 8);
        }
    }
}

