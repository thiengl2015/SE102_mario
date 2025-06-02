#include "ItemLeaf.h"
#include "Mario.h"
#include "ItemPoint.h"
#include "PlayScene.h"

CItemLeaf::CItemLeaf(float x, float y, int spriteId)
    : CGameObject(x, y)
{
    this->spriteId = spriteId;
    startX = x;
    startY = y;
    this->pointSpriteId = ID_SPRITE_ITEM_POINT_1000;
    SetState(LEAF_STATE_ON_RISE);
}


void CItemLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - LEAF_WIDTH / 2;
    top = y - LEAF_HEIGHT / 2;
    right = x + LEAF_WIDTH/2;
    bottom = y + LEAF_HEIGHT/2;
}

void CItemLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == LEAF_STATE_ON_RISE)
    {
        y -= LEAF_RISE_SPEED * dt;
        riseDistance += LEAF_RISE_SPEED * dt;

        if (riseDistance >= LEAF_RISE_DISTANCE)
            SetState(LEAF_STATE_SWAY);
    }
    else if (state == LEAF_STATE_SWAY)
    {
        x += swayDirection * LEAF_SWAY_SPEED * dt;
        y += LEAF_GRAVITY * dt;

        if (x - startX >= 10)
            swayDirection = -1;
		if (startX - x >= 10)
			swayDirection = 1;
    }

    CGameObject::Update(dt, coObjects);
}

void CItemLeaf::Render()
{
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y);
}

void CItemLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CMario*>(e->obj)) {
        CMario* mario = dynamic_cast<CMario*>(e->obj);
        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
        scene->AddObject(new CItemPoint(x, y - 10, pointSpriteId, 1000));

        if (mario->GetLevel() == MARIO_LEVEL_BIG) {
            mario->StartTransforming(MARIO_LEVEL_RACCOON);
        }

        this->Delete();
        return;
    }

    if (e->obj->IsBlocking()) {
        vy = 0;
    }
}


void CItemLeaf::OnNoCollision(DWORD dt)
{
    y += LEAF_GRAVITY * dt;
}

void CItemLeaf::SetState(int state)
{
    CGameObject::SetState(state);
    switch (state)
    {
    case LEAF_STATE_ON_RISE:
        ay = -LEAF_RISE_SPEED;
        break;
    case LEAF_STATE_SWAY:
        ay = LEAF_GRAVITY;
        swayDirection = 1; 
        break;
    case LEAF_STATE_FULL:

        break;
    }
}