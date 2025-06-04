#include "Brick.h"
#include "Animations.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "ItemCoin.h"
#include "ItemLeaf.h"
#include "ItemMushroom.h"
#include "ItemGreenMushroom.h"
#include "ItemPoint.h"
#include "turle.h"
#include "BlueSwitch.h"
#include "BounceEffect.h"

CBrick::CBrick(float x, float y, float w, float h, int brickType, int spawnType, bool debris, float vx, float vy)
    : CGameObject(x, y), width(w), height(h), brickType(brickType), spawnType(spawnType) {
    if (debris) {
        this->isDebris = true;
        this->debris_start = GetTickCount64();
        this->vx = vx;
        this->vy = vy;
    }
}

CBrick::CBrick(float x, float y, float w, float h, int brickType, int spawnType)
    : CGameObject(x, y), width(w), height(h), brickType(brickType), spawnType(spawnType) {
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    if (isDebris) {
        vy += BRICK_DEBRIS_GRAVITY * dt;
        x += vx * dt;
        y += vy * dt;

        if (GetTickCount64() - debris_start > BRICK_DEBRIS_LIFETIME)
            this->Delete();

        return;
    }
    if (isBouncing) {
        DWORD now = GetTickCount64();
        DWORD elapsed = now - bounce_start;

        if (elapsed < BRICK_BOUNCE_DURATION) {
            float progress = (float)elapsed / BRICK_BOUNCE_DURATION;
            bounceOffset = -sinf(progress * 3.1415f) * BRICK_BOUNCE_HEIGHT;
        }
        else {
            isBouncing = false;
            bounceOffset = 0.0f;
        }
    }
    else {
        bounceOffset = 0.0f;
    }

    if (!isBouncing && pendingSpawn) {
        SpawnItem();
        pendingSpawn = false;
    }
}


void CBrick::Render() {
    int aniId = 23000;

    if (isDebris) {
        CAnimations::GetInstance()->Get(23010)->Render(x, y);
        return;
    }

    if (brickType == 1)
        aniId = isUsed ? 22000 : 21000;
    else if (brickType == 0)
        aniId = 23000;
    else if (brickType == 2)
        aniId = isUsed ? 22000 : 23000;
    else if (brickType == 3)
		aniId = 22000; 
    else if (brickType == 4)
        aniId = 23001;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y + bounceOffset);

    if (showSwitchBounceEffect) {
        if (GetTickCount64() - switchBounce_start <= 1000) {
            CAnimations::GetInstance()->Get(1702)->Render(x, y);
        }
        else {
            showSwitchBounceEffect = false;
        }
    }

}


void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height;
}

void CBrick::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (isUsed || (brickType != 1 && brickType != 0 && brickType !=2 )) return;

    bool shouldBounce = false;

    if (dynamic_cast<CMario*>(e->src_obj) && e->ny > 0 && e->obj == this) {
        shouldBounce = true;
    }
    else if (dynamic_cast<CTurtle*>(e->src_obj) && abs(e->nx) > 0 && e->obj == this) {
        CTurtle* turtle = dynamic_cast<CTurtle*>(e->src_obj);
        if (turtle && turtle->IsShellState()) {
            DebugOut(L"Rùa phá viên gạch!\n");
            shouldBounce = true;
        }
    }
    else if (dynamic_cast<CMarioTail*>(e->src_obj)) {
        DebugOut(L"Đuôi đập viên gạch!\n");
        shouldBounce = true;
    }

    if (shouldBounce) {
        isUsed = true;
        isBouncing = true;
        bounce_start = GetTickCount64();

        if (brickType == 0) {
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

            scene->AddObject(new CBrick(x, y, 12, 12, 0, 0, true, 0.15f, -0.4f));
            scene->AddObject(new CBrick(x, y, 12, 12, 0, 0, true, -0.15f, -0.4f));
            scene->AddObject(new CBrick(x, y, 12, 12, 0, 0, true, 0.1f, -0.2f));
            scene->AddObject(new CBrick(x, y, 12, 12, 0, 0, true, -0.1f, -0.2f));

            SpawnItem();

            this->Delete();
        }

        else if (brickType == 2 && spawnType == 4) {
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            scene->AddObject(new CSwitchBlock(x, y));
            scene->AddObject(new CBounceEffect(x, y - 5.0f, 1702));

            return;
        }

        if (spawnType == 1) {
            SpawnItem();
        }
        else if (spawnType == 2) {
            pendingSpawn = true; 
        }
    }
}


void CBrick::SpawnItem() {
    float spawnX = x;
    float spawnY = y - height / 2;

    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (spawnType == 1) {
        scene->AddObject(new CItemCoin(spawnX, spawnY));
    }
    else if (spawnType == 2) {
        if (mario->GetLevel() == MARIO_LEVEL_SMALL)
            scene->AddObject(new CItemMushroom(spawnX, spawnY, ID_SPRITE_ITEM_MUSHROOM_RED));
        else if (mario->GetLevel() == MARIO_LEVEL_BIG)
            scene->AddObject(new CItemLeaf(spawnX, spawnY, ID_SPRITE_ITEM_LEAF));
        else if (mario->GetLevel() == MARIO_LEVEL_RACCOON)
            scene->AddObject(new CItemGreenMushroom(spawnX, spawnY, ID_SPRITE_ITEM_MUSHROOM_GREEN));

        scene->AddObject(new CBrick(spawnX, spawnY + 8.0f, width, height, 3, 0));
    }
    else if (spawnType == 3) {
        scene->AddObject(new CItemPoint(spawnX, spawnY, -1, 10));
    }
    else if (spawnType == 4) {
        scene->AddObject(new CSwitchBlock(spawnX, spawnY));
    }
}
