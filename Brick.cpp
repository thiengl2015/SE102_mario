#include "Brick.h"
#include "Animations.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "ItemCoin.h"
#include "ItemLeaf.h"
#include "ItemMushroom.h"
#include "ItemPoint.h"
#include "turle.h"

CBrick::CBrick(float x, float y, float width, float height, int brickType, int spawnType, int itemSpriteId, int pointSpriteId)
    : CGameObject(x, y), width(width), height(height), brickType(brickType), spawnType(spawnType), itemSpriteId(itemSpriteId), pointSpriteId(pointSpriteId) {
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
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
        float spawnX = x;
        float spawnY = y - height / 2;

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

        switch (pendingSpawnType) {
        case 2:
            scene->AddObject(new CItemMushroom(spawnX, spawnY, itemSpriteId, pointSpriteId));
            scene->AddObject(new CBrick(spawnX, spawnY + 8.0f, width, height, 3, 0, itemSpriteId, pointSpriteId));
            break;
        case 3:
            scene->AddObject(new CItemLeaf(spawnX, spawnY, itemSpriteId, pointSpriteId));
            scene->AddObject(new CBrick(spawnX, spawnY + 8.0f, width, height, 3, 0, itemSpriteId, pointSpriteId));
            break;
        }

        pendingSpawn = false;
    }
}

void CBrick::Render() {
    int aniId = 23000; // default pattern

    if (brickType == 1)
        aniId = isUsed ? 22000 : 21000; // question
    else if (brickType == 0)
        aniId = isUsed ? 23000 : 23000;
    else if (brickType == 3)
        aniId = 22000;

    CAnimations::GetInstance()->Get(aniId)->Render(x, y + bounceOffset);
}

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height;
}

void CBrick::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (isUsed || brickType != 1) return;

    if (dynamic_cast<CMario*>(e->src_obj) && e->ny > 0 && e->obj == this) {
        isUsed = true;
        isBouncing = true;
        bounce_start = GetTickCount64();

        float spawnX = x;
        float spawnY = y - height / 2;

        CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

        switch (spawnType) {
        case 1:
            scene->AddObject(new CItemCoin(spawnX, spawnY, pointSpriteId));
            break;
        case 2:
        case 3:
            pendingSpawn = true;
            pendingSpawnType = spawnType;
            break;
        }
    }
    else if (dynamic_cast<CTurtle*>(e->src_obj) && abs(e->nx) > 0 && e->obj == this)
    {
        CTurtle* turtle = dynamic_cast<CTurtle*>(e->src_obj);
        if (turtle && turtle->IsShellState()) 
        {
            DebugOut(L"Rùa phá viên gạch!\n");
            isUsed = true; 
            isBouncing = true;
            bounce_start = GetTickCount64();

            float spawnX = x;
            float spawnY = y - height / 2;
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

            switch (spawnType)
            {
            case 1:
                scene->AddObject(new CItemCoin(spawnX, spawnY, pointSpriteId));
                break;
            case 2:
            case 3:
                pendingSpawn = true;
                pendingSpawnType = spawnType;
                break;
            }
        }

    }
}
