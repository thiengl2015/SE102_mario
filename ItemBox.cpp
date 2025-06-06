#include "ItemBox.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Game.h"
#include "Sprites.h"



CItemBox::CItemBox(float x, float y) : CGameObject(x, y) {
	lastSwitchTime = GetTickCount64();
}

void CItemBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (!isCollected && GetTickCount64() - lastSwitchTime > ITEMBOX_SWITCH_TIME) {
		currentSprite = (currentSprite + 1) % 3;
		lastSwitchTime = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);
}

void CItemBox::Render() {
	int spriteId = (currentSprite == 0) ? 200010 : (currentSprite == 1) ? 200009 : 200008;

	CSprites::GetInstance()->Get(spriteId)->Draw(x, y);

}

void CItemBox::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (isCollected) return;

	if (!dynamic_cast<CMario*>(e->obj)) return;

	DebugOut(L"[ItemBox] Mario collected ItemBox at x=%.1f, y=%.1f\n", x, y);

	isCollected = true;

	int type = currentSprite == 0 ? ITEM_TYPE_MUSHROOM :
		currentSprite == 1 ? ITEM_TYPE_FLOWER : ITEM_TYPE_STAR;

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->AddObject(new CItemBoxEffect(x, y, type));

	this->Delete();
}


void CItemBox::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x - ITEMBOX_BBOX_WIDTH / 2;
	t = y - ITEMBOX_BBOX_HEIGHT / 2;
	r = l + ITEMBOX_BBOX_WIDTH;
	b = t + ITEMBOX_BBOX_HEIGHT;
}
