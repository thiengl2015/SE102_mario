#pragma once
#include "Mario.h"
#include "GameObject.h"
#include "PlayScene.h"

#define ID_SPRITE_BLACK 1000000000

#define ID_SPRITE_HUD 1000000001

#define ID_SPRITE_NUMBER_0 1000000002
#define ID_SPRITE_NUMBER_1 1000000003
#define ID_SPRITE_NUMBER_2 1000000004
#define ID_SPRITE_NUMBER_3 1000000005
#define ID_SPRITE_NUMBER_4 1000000006
#define ID_SPRITE_NUMBER_5 1000000007
#define ID_SPRITE_NUMBER_6 1000000008
#define ID_SPRITE_NUMBER_7 1000000009
#define ID_SPRITE_NUMBER_8 1000000010
#define ID_SPRITE_NUMBER_9 1000000011


#define ID_SPRITE_ITEM_BOX_1 1000000012
#define ID_SPRITE_ITEM_BOX_2 1000000013
#define ID_SPRITE_ITEM_BOX_3 1000000014
#define ID_SPRITE_ITEM_BOX_4 1000000015 

#define ID_SPRITE_ARRAW_BLACK 1000000016
#define ID_SPRITE_ARRAW_WHITE 1000000017

#define ID_SPRITE_POWER_BLACK 1000000018
#define ID_SPRITE_POWER_WHITE 1000000019



class CHud : public CGameObject
{
protected:
	int coin;
	int	score;
	int itemBox[3]; // 0: empty, 1: mushroom, 2: flower, 3: star
	int type; // 0: HUD, 1: NUMBER, 2: ITEM_BOX, 3: BLACK
	int time;
	ULONGLONG startTime = GetTickCount64(); 
public:
	CHud(float x, float y);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void Render() override;

	void SetCoin(int c) { coin = c; }
	void SetScore(int s) { score = s; }
	void SetItemBox(int index, int itemType) { if (index >= 0 && index < 3) itemBox[index] = itemType; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override
	{
		left = x;
		top = y;
		right = x + 100; // Width of HUD
		bottom = y + 50; // Height of HUD
	}
	void AddScore(int s) { score += s; }
	int GetCoin() const { return coin; }
};
