#include "FlyingKoopas.h"
#include "PlayScene.h"
#include "ItemPoint.h"
#include "Brick.h"
#include "Goomba.h"
#include "RedGoomba.h"
#include "JumpingKoopas.h"


CFlyingKoopas::CFlyingKoopas(float x, float y) : CGameObject(x, y)
{
    ax = 0;
    ay = FKOOPAS_GRAVITY;
    shell_start = -1;
    walkingDirection = -1;
    isBeingHeld = false;
    holder = nullptr;
    rootY = y;

    SetState(FKOOPAS_STATE_FLYING);
}

void CFlyingKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == FKOOPAS_STATE_SHELL || state == FKOOPAS_STATE_SHELL_MOVING || state == FKOOPAS_STATE_HELD)
    {
        left = x - FKOOPAS_BBOX_WIDTH / 2;
        top = y - FKOOPAS_BBOX_HEIGHT_DIE / 2;
        right = left + FKOOPAS_BBOX_WIDTH;
        bottom = top + FKOOPAS_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - FKOOPAS_BBOX_WIDTH / 2;
        top = y - FKOOPAS_BBOX_HEIGHT / 2;
        right = left + FKOOPAS_BBOX_WIDTH;
        bottom = top + FKOOPAS_BBOX_HEIGHT;
    }
}

void CFlyingKoopas::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CFlyingKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
        vy = 0;
    else if (e->nx != 0)
    {
        vx = -vx;
        walkingDirection = -walkingDirection;
    }
}

void CFlyingKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == FKOOPAS_STATE_HELD && holder != nullptr)
    {
        x = holder->GetX() + holder->GetDirection() * 12.0f;
        y = holder->GetY();
        return;
    }

    vy += ay * dt;
    vx += ax * dt;

    if (state == FKOOPAS_STATE_SHELL && GetTickCount64() - shell_start > FKOOPAS_REVIVE_TIMEOUT)
    {
        SetState(FKOOPAS_STATE_REVIVING);
        SetPosition(x, y - FKOOPAS_BBOX_HEIGHT / 2);
    }

    if (state == FKOOPAS_STATE_FLYING)
    {
        if (y < rootY - FKOOPAS_FLY_RANGE)
            vy = FKOOPAS_FLY_SPEED;
        else if (y > rootY + FKOOPAS_FLY_RANGE)
            vy = -FKOOPAS_FLY_SPEED;

        walkingDirection = -1; 
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CFlyingKoopas::Render()
{
    int aniId = -1;
    switch (state)
    {
    case FKOOPAS_STATE_FLYING:
        aniId = ID_ANI_JKOOPAS_JUMP_LEFT;
        break;
    case FKOOPAS_STATE_WALKING:
    case FKOOPAS_STATE_REVIVING:
        aniId = (walkingDirection > 0) ? ID_ANI_JKOOPAS_WALK_RIGHT : ID_ANI_JKOOPAS_WALK_LEFT;
        break;
    case FKOOPAS_STATE_SHELL:
    case FKOOPAS_STATE_SHELL_MOVING:
    case FKOOPAS_STATE_HELD:
    case FKOOPAS_STATE_DIE_FALL:
        aniId = ID_ANI_JKOOPAS_SHELL;
        break;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CFlyingKoopas::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case FKOOPAS_STATE_FLYING:
        vy = FKOOPAS_FLY_SPEED;
        break;
    case FKOOPAS_STATE_WALKING:
        vx = walkingDirection * FKOOPAS_WALKING_SPEED;
        break;
    case FKOOPAS_STATE_SHELL:
        vx = 0;
        shell_start = GetTickCount64();
        break;
    case FKOOPAS_STATE_SHELL_MOVING:
        vx = walkingDirection * FKOOPAS_SHELL_SLIDE_SPEED;
        break;
    case FKOOPAS_STATE_HELD:
        vx = 0;
        break;
    case FKOOPAS_STATE_REVIVING:
        vx = walkingDirection * FKOOPAS_WALKING_SPEED;
        break;
    case FKOOPAS_STATE_DIE_FALL:
        vy = -0.5f;
        ay = FKOOPAS_GRAVITY;
        break;
    }
}

bool CFlyingKoopas::IsShellState()
{
    return (state == FKOOPAS_STATE_SHELL || state == FKOOPAS_STATE_SHELL_MOVING || state == FKOOPAS_STATE_HELD);
}

void CFlyingKoopas::KickShell(int dir)
{
    walkingDirection = dir;
    SetState(FKOOPAS_STATE_SHELL_MOVING);
}
