#pragma once
#include "GameObject.h"
#include "Sprite.h"

class CSimpleSpriteObject : public CGameObject
{
	LPSPRITE sprite;

public:
	CSimpleSpriteObject(float x, float y, int sprite_id);
	void Render();
	void Update(DWORD dt) {}
};