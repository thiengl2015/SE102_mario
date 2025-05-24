#pragma once
#include "ItemMushroom.h"

class CItemGreenMushroom : public CItemMushroom {
public:
    CItemGreenMushroom(float x, float y, int spriteId, int pointSpriteId)
        : CItemMushroom(x, y, spriteId, pointSpriteId) {
    }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void Render() override;
};
