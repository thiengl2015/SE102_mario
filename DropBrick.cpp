#include "DropBrick.h"
#include "PlayScene.h"
#include "Game.h"
#include "Mario.h"
#include "Collision.h"
#include "Animations.h"

#define MARGIN_X 25.0f
#define MARGIN_Y 25.0f

CDropBrick::CDropBrick(float x, float y) : CGameObject(x, y) {}


void CDropBrick::SetMario()
{
    CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (scene)
        mario = dynamic_cast<CMario*>(scene->GetPlayer());
}

void CDropBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!mario) SetMario();
    if (!mario || mario->IsDeleted()) return;

    float mx, my;
    mario->GetPosition(mx, my);

    if (!isActivated && abs(mx - x) <= activationRange) {
        isActivated = true;
        vx = DROPBRICK_MOVE_VX;
    }

    if (isFalling) {
        vy += DROPBRICK_GRAVITY * dt;
        if (hasMario)
            mario->SetIsStickToPlatform(this);
    }

    x += vx * dt;
    y += vy * dt;

    CCollision::GetInstance()->Process(this, dt, coObjects);

    float pl_left, pl_top, pl_right, pl_bottom;
    GetBoundingBox(pl_left, pl_top, pl_right, pl_bottom);

    bool outOfBounds =
        mx < pl_left - MARGIN_X ||
        mx > pl_right + MARGIN_X ||
        my < pl_top - MARGIN_Y ||
        my > pl_bottom + MARGIN_Y;

    if (outOfBounds && hasMario) {
        if (mario->IsStickToPlatform())
            mario->SetIsStickToPlatform(nullptr);
        hasMario = false;
    }
}

void CDropBrick::Render()
{
    CAnimations::GetInstance()->Get(ANIMATION_DROPBRICK)->Render(x, y);
    //RenderBoundingBox();
}

void CDropBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - DROPBRICK_BBOX_WIDTH / 2;
    t = y - DROPBRICK_BBOX_HEIGHT / 2;
    r = l + DROPBRICK_BBOX_WIDTH;
    b = t + DROPBRICK_BBOX_HEIGHT;
}
void CDropBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<CMario*>(e->obj)) {
        OnCollisionWithMario(e);
    }
}

void CDropBrick::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
    if (!mario) SetMario();
    if (!mario || mario->IsDeleted()) return;

    if (e->ny > 0 && !isFalling) 
    {
        isFalling = true;
        hasMario = true;
        vx = 0;
        vy = DROPBRICK_FALL_VY;
        mario->SetIsStickToPlatform(this);
    }
}

