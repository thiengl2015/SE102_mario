#include "BoomerangBrother.h"
#include "PlayScene.h"
#include "Mario.h"
#include "ItemPoint.h"

CBoomerangBrother::CBoomerangBrother(float x, float y) : CGameObject(x, y) {
    startX = x;
    startY = y;
    direction = -1;
    vx = direction * BB_WALK_SPEED;
    lastThrow = GetTickCount64();
}

void CBoomerangBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
    if (isFlipped) {
        vy += ay * dt;
        y += vy * dt;

        if (y > 280)
            Delete();
        return;
    }
    vy += ay * dt;

    CGameObject::Update(dt, coObjects);

    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
    if (!mario) return;

    switch (step) {
    case 0:
        if (x < startX - walkRange) {
            vx = BB_WALK_SPEED;
        }
        else if (x > startX + walkRange) {
            vx = -BB_WALK_SPEED;
        }

        direction = (mario->GetX() > x) ? 1 : -1;

        isHolding = true;

        if (GetTickCount64() - lastThrow > BB_THROW_INTERVAL) {
            vx = 0;
            prepareTime = GetTickCount64();
            step = 1;
        }
        break;

    case 1:
        if (GetTickCount64() - prepareTime > BB_THROW_PREPARE_TIME) {
            CBoomerang* b = new CBoomerang(x, y, direction);
            ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(b);
            boomerangs.push_back(b);
            lastThrow = GetTickCount64();
            step = 2;
        }
        break;
    case 2:
        if (x < startX - walkRange) {
            vx = BB_WALK_SPEED;
        }
        else if (x > startX + walkRange) {
            vx = -BB_WALK_SPEED;
        }
        else {
            if (vx == 0)
                vx = direction * BB_WALK_SPEED;
        }

        direction = (mario->GetX() > x) ? 1 : -1;

        if (GetTickCount64() - lastThrow > 500) {
            step = 0;
        }
        break;
    }
    bool allBoomerangsFar = true;
    for (CBoomerang* b : boomerangs) {
        if (!b->IsDeleted() && abs(b->GetX() - x) < 10.0f) {
            allBoomerangsFar = false;
            break;
        }
    }
    isHolding = !allBoomerangsFar;

    CCollision::GetInstance()->Process(this, dt, coObjects);
    isOnPlatform = false;
}

void CBoomerangBrother::Render() {
    int ani;

    if (isFlipped) {
        ani = (direction == -1) ? ID_ANI_BB_NOHOLD_LEFT : ID_ANI_BB_NOHOLD_RIGHT;
    }
    else {
        ani = (isHolding ?
            (direction == -1 ? ID_ANI_BB_HOLD_LEFT : ID_ANI_BB_HOLD_RIGHT) :
            (direction == -1 ? ID_ANI_BB_NOHOLD_LEFT : ID_ANI_BB_NOHOLD_RIGHT));
    }

    for (CBoomerang* b : boomerangs) {
        if (b && !b->IsDeleted())
            b->Render();
    }

    CAnimations::GetInstance()->Get(ani)->Render(x, y);
}

void CBoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b) {
    l = x;
    t = y;
    r = x + BB_BBOX_WIDTH;
    b = y + BB_BBOX_HEIGHT;
}

void CBoomerangBrother::OnNoCollision(DWORD dt) {
    x += vx * dt;
    y += vy * dt;
}

void CBoomerangBrother::OnCollisionWith(LPCOLLISIONEVENT e) {
    if (isFlipped) return;

    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CBoomerang*>(e->obj)) return;

    if (e->ny < 0 && dynamic_cast<CMario*>(e->obj)) {
        CMario* mario = dynamic_cast<CMario*>(e->obj);
        if (mario && mario->GetUntouchable() == 0) {
            mario->SetVY(-0.4f);
            CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            scene->AddObject(new CItemPoint(x, y - 10, 21000, 1000));
            this->SetState(BB_STATE_FLIPPED);
        }
    }
    else {
        if (e->nx != 0) vx = -vx;
        if (e->ny != 0) vy = 0;
    }
}

void CBoomerangBrother::SetState(int state) {
    this->state = state;

    switch (state) {
    case BB_STATE_WALKING:
        vx = direction * BB_WALK_SPEED;
        break;

    case BB_STATE_THROWING:
        vx = 0;
        break;

    case BB_STATE_FLIPPED:
        isFlipped = true;
        vx = 0;
        vy = -0.2f;
        ay = 0.0012f;
        break;
    }
}

