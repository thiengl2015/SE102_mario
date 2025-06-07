#pragma once
#include "GameObject.h"
#include "ItemBoxEffect.h"

#define ITEMBOX_SWITCH_TIME 100
#define ITEMBOX_EFFECT_VY -0.1f
#define ITEMBOX_BBOX_WIDTH 16
#define ITEMBOX_BBOX_HEIGHT 16

#define ITEM_TYPE_MUSHROOM 1
#define ITEM_TYPE_FLOWER   2
#define ITEM_TYPE_STAR     3

class CItemBox : public CGameObject {
	int currentSprite = 0;
	ULONGLONG lastSwitchTime = 0;
	bool isCollected = false;

public:
	CItemBox(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	bool IsCollected() const { return isCollected; }
	void SetCollected(bool val) { isCollected = val; }

	int GetCurrentItemType() const {
		return (currentSprite == 0) ? ITEM_TYPE_MUSHROOM :
			(currentSprite == 1) ? ITEM_TYPE_FLOWER :
			ITEM_TYPE_STAR;
	}

	float getX() { return x; }
	float getY() { return y; }
	int GetType() override { return 9999; }
};
