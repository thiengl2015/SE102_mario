#include "turle.h"
#include "debug.h" // optional: for debugging output
#include "Platform.h" // để kiểm tra mặt đất

CTurtle::CTurtle(float x, float y, bool isRed) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = TURTLE_GRAVITY;
    this->shell_start = -1;
    this->isRed = isRed;
    this->isBeingHeld = false;
    this->isReviving = false;
    this->walkingDirection = -1;
    SetState(TURTLE_STATE_WALKING);
}

void CTurtle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == TURTLE_STATE_SHELL || state == TURTLE_STATE_SHELL_MOVING || state == TURTLE_STATE_HELD || state == TURTLE_STATE_REVIVING)
    {
        left = x - TURTLE_BBOX_WIDTH / 2;
        top = y - TURTLE_BBOX_HEIGHT_DIE / 2;
        right = left + TURTLE_BBOX_WIDTH;
        bottom = top + TURTLE_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - TURTLE_BBOX_WIDTH / 2;
        top = y - TURTLE_BBOX_HEIGHT / 2;
        right = left + TURTLE_BBOX_WIDTH;
        bottom = top + TURTLE_BBOX_HEIGHT;
    }
}

void CTurtle::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CTurtle::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;
    if (dynamic_cast<CTurtle*>(e->obj)) return;

    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        if (state == TURTLE_STATE_SHELL_MOVING)
            vx = -vx;
        else if (state == TURTLE_STATE_WALKING)
            walkingDirection = -walkingDirection;
    }
}

void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    if (state == TURTLE_STATE_SHELL)
    {
        if (GetTickCount64() - shell_start > TURTLE_REVIVE_TIMEOUT)
        {
            SetState(TURTLE_STATE_REVIVING);
        }
    }

    if (state == TURTLE_STATE_REVIVING)
    {
        if (GetTickCount64() - shell_start > TURTLE_REVIVE_TIMEOUT + 1000)
        {
            SetState(TURTLE_STATE_WALKING);
        }
    }

    if (state != TURTLE_STATE_HELD)
        CGameObject::Update(dt, coObjects);

    // === Kiểm tra mép platform để quay đầu nếu cần ===
    if (state == TURTLE_STATE_WALKING && isRed)
    {
        float checkX = x + (walkingDirection > 0 ? TURTLE_BBOX_WIDTH / 2 + 1 : -TURTLE_BBOX_WIDTH / 2 - 1);
        float checkY = y + TURTLE_BBOX_HEIGHT / 2 + 1;

        bool hasGround = false;
        for (auto& obj : *coObjects)
        {
            if (dynamic_cast<CPlatform*>(obj))
            {
                float l, t, r, b;
                obj->GetBoundingBox(l, t, r, b);
                if (checkX >= l && checkX <= r && checkY >= t && checkY <= b)
                {
                    hasGround = true;
                    break;
                }
            }
        }

        if (!hasGround)
        {
            walkingDirection = -walkingDirection;
            vx = walkingDirection * TURTLE_WALKING_SPEED;
        }
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTurtle::Render()
{
    int aniId = ID_ANI_TURTLE_WALKING_LEFT;

    if (state == TURTLE_STATE_SHELL || state == TURTLE_STATE_HELD)
    {
        aniId = ID_ANI_TURTLE_SLEEP;
    }
    else if (state == TURTLE_STATE_SHELL_MOVING)
    {
        aniId = ID_ANI_TURTLE_SLEEP;
    }
    else if (state == TURTLE_STATE_WALKING)
    {
        aniId = (vx > 0) ? ID_ANI_TURTLE_WALKING_LEFT : ID_ANI_TURTLE_WALKING_RIGHT;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CTurtle::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case TURTLE_STATE_WALKING:
        vx = walkingDirection * TURTLE_WALKING_SPEED;
        ay = TURTLE_GRAVITY;
        break;
    case TURTLE_STATE_SHELL:
        vx = 0;
        shell_start = GetTickCount64();
        break;
    case TURTLE_STATE_SHELL_MOVING:
        vx = walkingDirection * 0.15f;
        break;
    case TURTLE_STATE_HELD:
        vx = 0;
        break;
    case TURTLE_STATE_REVIVING:
        vx = 0;
        break;
    }
}

void CTurtle::StartShell()
{
    SetState(TURTLE_STATE_SHELL);
}

void CTurtle::KickShell(int dir)
{
    walkingDirection = dir;
    SetState(TURTLE_STATE_SHELL_MOVING);
}

void CTurtle::StartReviving()
{
    shell_start = GetTickCount64();
    SetState(TURTLE_STATE_REVIVING);
}

void CTurtle::SetBeingHeld(bool held)
{
    isBeingHeld = held;
    if (held)
        SetState(TURTLE_STATE_HELD);
    else
        shell_start = GetTickCount64();
}

bool CTurtle::IsBeingHeld()
{
    return isBeingHeld;
}

bool CTurtle::IsShellState()
{
    return (state == TURTLE_STATE_SHELL || state == TURTLE_STATE_SHELL_MOVING || state == TURTLE_STATE_HELD);
}