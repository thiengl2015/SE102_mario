#include "BrickN.h"
#include "ItemCoin.h"
#include "ItemPoint.h"
#include "PlayScene.h"
#include "Game.h"
#include "Animations.h"
#include "turle.h"

CBrickN::CBrickN(float x, float y, float w, float h) : CGameObject(x, y), width(w), height(h) {}

void CBrickN::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    if (isBouncing) {
        DWORD now = GetTickCount64();
        DWORD elapsed = now - bounce_start;

        if (elapsed < BOUNCING_BRICK_BOUNCE_DURATION) {
            float progress = (float)elapsed / BOUNCING_BRICK_BOUNCE_DURATION;
            bounceOffset = -sinf(progress * 3.1415f) * BOUNCING_BRICK_BOUNCE_HEIGHT;
        }
        else {
            isBouncing = false;
            bounceOffset = 0.0f;
            HurtTurtleIfOnTop();
            if (bounceCount < MAX_BOUNCES) {
                SpawnItem();
            }
            else {
                isUsed = true;
            }
        }
    }
}

void CBrickN::Render() {
    int aniId = isUsed ? 22000 : 23000; 
    CAnimations::GetInstance()->Get(aniId)->Render(x, y + bounceOffset);
}

void CBrickN::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x - width / 2;
    t = y - height / 2;
    r = l + width;
    b = t + height;
}

void CBrickN::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (isUsed) return;

    if (dynamic_cast<CMario*>(e->src_obj) && e->ny > 0 && e->obj == this) {
        isBouncing = true;
        bounce_start = GetTickCount64();
        bounceCount++;
    }
}

void CBrickN::SpawnItem() {
    float spawnX = x;
    float spawnY = y - height / 2;
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

    if (bounceCount % 2 == 0)
        scene->AddObject(new CItemCoin(spawnX, spawnY));
    else
        scene->AddObject(new CItemPoint(spawnX, spawnY, ID_SPRITE_ITEM_POINT_100, 100));
}

void CBrickN::HurtTurtleIfOnTop() {
    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    if (!scene) return;

    float bl, bt, br, bb;
    GetBoundingBox(bl, bt, br, bb);

    for (LPGAMEOBJECT obj : scene->GetObjects()) {
        CTurtle* turtle = dynamic_cast<CTurtle*>(obj);
        if (turtle) {
            float xl, yt, xr, yb;
            turtle->GetBoundingBox(xl, yt, xr, yb);

            bool isAbove = yb >= bt - 2 && yb <= bt + 2 && xl < br && xr > bl;
            if (isAbove && !turtle->IsShellState()) {
                turtle->StartShell();

                turtle->SetVy(-0.25f);
                float dir = (rand() % 2 == 0) ? -1.0f : 1.0f;
                turtle->SetVx(dir * 0.1f);

                float tx, ty;
                turtle->GetPosition(tx, ty);
                turtle->SetPosition(tx, ty - 1.0f);

                turtle->isKnockbacked = true;
                turtle->knockback_start = GetTickCount64();
            }
        }
    }
}
