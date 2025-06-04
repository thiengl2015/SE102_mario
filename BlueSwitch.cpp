#include "BlueSwitch.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Brick.h"
#include "Mario.h"
CSwitchBlock::CSwitchBlock(float x, float y) : CGameObject(x, y) {}

void CSwitchBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    if (isActivated) return;

    for (auto obj : *coObjects) {
        CMario* mario = dynamic_cast<CMario*>(obj);
        if (!mario) continue;

        float ml, mt, mr, mb;
        mario->GetBoundingBox(ml, mt, mr, mb);

        float sl, st, sr, sb;
        GetBoundingBox(sl, st, sr, sb);

        bool isStandingOn = mb >= st && mb <= st + 5.0f && mr > sl && ml < sr;
        if (isStandingOn) {
            Activate();
            break;
        }
    }
}


void CSwitchBlock::Render() {
    int spriteId = (state == SWITCH_STATE_DOWN) ? 200007 : 200006;
    CSprites::GetInstance()->Get(spriteId)->Draw(x, y - 15.0f);
}

void CSwitchBlock::GetBoundingBox(float& l, float& t, float& r, float& b) {
    float yOffset = (state == SWITCH_STATE_DOWN) ? 8.0f : 0.0f;

    l = x - SWITCH_WIDTH / 2;
    t = y - 15.0f + yOffset - SWITCH_HEIGHT / 2;
    r = l + SWITCH_WIDTH;
    b = t + SWITCH_HEIGHT;
}


void CSwitchBlock::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (isActivated) return;

    if (e->nx != 0) vx = 0;
    if (e->ny != 0) vy = 0;

    if (dynamic_cast<CMario*>(e->src_obj)) {
        if (e->ny < 0) {
            DebugOut(L"[Switch] Mario hit from below!\n");
            Activate();
        }
        else if (e->ny > 0) {
            DebugOut(L"[Switch] Mario jumped on top!\n");
            Activate();
        }
    }
}


void CSwitchBlock::Activate() {
    state = SWITCH_STATE_DOWN;
    isActivated = true;

    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
    auto& objects = scene->GetObjects();

    for (auto obj : objects) {
        CBrick* brick = dynamic_cast<CBrick*>(obj);
        if (brick && brick->GetBrickType() == 0) {
            float bx, by;
            brick->GetPosition(bx, by);
            float dx = abs(bx - x);
            float dy = abs(by - y);

            if (dx <= SWITCH_AOE_RANGE && dy <= SWITCH_AOE_RANGE) {
                scene->AddObject(new CCoin(bx, by));
                brick->Delete();
            }
        }
    }
    scene->StartCameraShake();
}
