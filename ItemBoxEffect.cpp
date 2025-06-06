#include "ItemBoxEffect.h"
#include "Animations.h"

CItemBoxEffect::CItemBoxEffect(float x, float y, int type) {
	this->x = x;
	this->y = y;
	this->type = type;
	this->vy = ITEMBOX_EFFECT_VY;
	spawnTime = GetTickCount64();
}

void CItemBoxEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	y += vy * dt;

	if (GetTickCount64() - spawnTime > 1000) {
		this->Delete();
	}
}

void CItemBoxEffect::Render() {
	int aniId = (type == ITEM_TYPE_MUSHROOM) ? 210010 :
		(type == ITEM_TYPE_FLOWER) ? 210009 :
		(type == ITEM_TYPE_STAR) ? 210008 : -1;

	if (aniId != -1)
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
