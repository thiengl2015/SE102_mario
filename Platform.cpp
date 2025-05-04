#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CPlatform::Render()
{

	if (this->length <= 0 && this->width>0)
	{
		float yy = y;
		CSprites* s = CSprites::GetInstance();

		s->Get(this->spriteIdBegin)->Draw(x, yy);
		yy += this->cellHeight;
		for (int i = 1; i < this->width - 1; i++)
		{
			s->Get(this->spriteIdMiddle)->Draw(x, yy);
			yy += this->cellHeight;
		}
	}
	float xx = x; 
	CSprites * s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);

}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	return 1;
}