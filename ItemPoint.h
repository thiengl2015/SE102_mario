#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define ITEM_POINT_WIDTH 16
#define ITEM_POINT_HEIGHT 16
#define ITEM_POINT_LIFETIME 500

class CItemPoint : public CGameObject
{
    DWORD spawn_time;
    int spriteId;
    int value;
public:
    CItemPoint(float x, float y, int spriteId, int value);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    int GetType() override { return TYPE_ITEM_POINT; }
    int GetValue() const { return value; }
    bool hasAddedScore = false;
};
