#include "SimpleSpriteObject.h"

CSimpleSpriteObject::CSimpleSpriteObject(float x, float y, int sprite_id)
{
	this->x = x;
	this->y = y;
	this->sprite = CSprites::GetInstance()->Get(sprite_id);
}

void CSimpleSpriteObject::Render()
{
	
}
