#include "turle.h"
#include "debug.h" // optional: for debugging output
#include "Platform.h" // để kiểm tra mặt đất
#include "Mario.h"
#include "Block.h"
CTurtle::CTurtle(float x, float y) : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = TURTLE_GRAVITY;
    shell_start = -1;
    this->walkingDirection = -1;
    this->isBeingHeld = false;
    edgeSensor = new CEdgeSensor(x, y);
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


    CMario* mario = dynamic_cast<CMario*>(e->obj);
    if (mario && IsShellState() && vx == 0)
    {
        int direction = (mario->getX() < this->x) ? 1 : -1;
        KickShell(direction);
        return;
    }
    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0 && vx != 0)
    {
        vx = -vx;
        walkingDirection = -walkingDirection;
    }

}

void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vx += ax * dt;
    vy += ay * dt;

    if (state == TURTLE_STATE_SHELL)
    {
        if (GetTickCount64() - shell_start > TURTLE_REVIVE_TIMEOUT)
        {
            SetState(TURTLE_STATE_REVIVING);
			walkingDirection = -walkingDirection;
            edgeSensor->SetPosition(x + walkingDirection * TURTLE_BBOX_WIDTH / 2, y + TURTLE_BBOX_HEIGHT / 2);
            if (!edgeSensor->IsOnHalfSolidBlock(coObjects)) {
                walkingDirection = -walkingDirection;
                vx = walkingDirection * TURTLE_WALKING_SPEED;
            }
        }
    }
    if (state == TURTLE_STATE_WALKING)
    {
        edgeSensor->SetPosition(x + walkingDirection * TURTLE_BBOX_WIDTH / 2, y + TURTLE_BBOX_HEIGHT / 2);

        if (!edgeSensor->IsOnHalfSolidBlock(coObjects)) {
            walkingDirection = -walkingDirection;
            vx = walkingDirection * TURTLE_WALKING_SPEED;
        }
    }
    



    CGameObject::Update(dt, coObjects); // Cập nhật dx, dy
    CCollision::GetInstance()->Process(this, dt, coObjects);

}

void CTurtle::Render()
{
    int aniId = ID_ANI_TURTLE_WALKING_LEFT;

    if (state == TURTLE_STATE_SHELL)
    {
        aniId = ID_ANI_TURTLE_SHELL;
    }
    else if (state == TURTLE_STATE_SHELL_MOVING)
    {
        aniId = ID_ANI_TURTLE_SHELL;
    }
    else if (state == TURTLE_STATE_WALKING)
    {
        aniId = (walkingDirection > 0) ? ID_ANI_TURTLE_WALKING_LEFT : ID_ANI_TURTLE_WALKING_RIGHT;
    }
    else if (state == TURTLE_STATE_REVIVING)
    {
        aniId = (walkingDirection > 0) ? ID_ANI_TURTLE_WALKING_RIGHT : ID_ANI_TURTLE_WALKING_LEFT;
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
        vx = 0.05f;
        break;
    }
}

bool CTurtle::IsShellState()
{
    return (state == TURTLE_STATE_SHELL || state == TURTLE_STATE_SHELL_MOVING || state == TURTLE_STATE_HELD);
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
bool CTurtle::IsBeingHeld()
{
    return isBeingHeld;
}