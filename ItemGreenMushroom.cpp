#include "ItemGreenMushroom.h"
#include "PlayScene.h"
#include "Mario.h"
#include "ItemPoint.h"
#include "Sprites.h"
#include "AssetIDs.h"

void CItemGreenMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->obj->GetType() == TYPE_BLOCKERWALL) {
        this->Delete();
        return;
    }

    if (e->nx != 0) vx = -vx;
    if (e->ny != 0) vy = 0;

    if (dynamic_cast<CMario*>(e->obj)) {
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(new CItemPoint(x, y - 10, pointSpriteId, 1000)); 
        this->Delete(); 
    }
}

void CItemGreenMushroom::Render()
{
    CSprites::GetInstance()->Get(ID_SPRITE_ITEM_MUSHROOM_GREEN)->Draw(x, y);
}
