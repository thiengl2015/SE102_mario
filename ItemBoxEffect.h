#pragma once
#include "GameObject.h"

#define ITEMBOX_EFFECT_VY -0.1f

#define ITEM_TYPE_MUSHROOM 1
#define ITEM_TYPE_FLOWER   2
#define ITEM_TYPE_STAR     3

class CItemBoxEffect : public CGameObject {
	int type;
	ULONGLONG spawnTime;

public:
	CItemBoxEffect(float x, float y, int type);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
	int GetType() override { return type; }

	float getX() { return x; }
	float getY() { return y; }
};
