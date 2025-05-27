#pragma once
#include "ItemMushroom.h"
#define GREEN_MUSHROOM_POINT_ID 20111

class CItemGreenMushroom : public CItemMushroom {
public:
    CItemGreenMushroom(float x, float y, int spriteId)
        : CItemMushroom(x, y, spriteId) {
        this->pointSpriteId = GREEN_MUSHROOM_POINT_ID;
    }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void Render() override;
};
