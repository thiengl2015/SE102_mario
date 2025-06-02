#pragma once
#include "ItemMushroom.h"
#include "AssetIDs.h"

class CItemGreenMushroom : public CItemMushroom {
public:
    CItemGreenMushroom(float x, float y, int spriteId)
        : CItemMushroom(x, y, spriteId) {
        this->pointSpriteId = ID_SPRITE_ITEM_POINT_1UP;
    }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void Render() override;
};
