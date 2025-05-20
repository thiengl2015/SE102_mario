#include "turle.h"
#include "debug.h" // optional: for debugging output
#include "Platform.h" // để kiểm tra mặt đất
#include "Mario.h"
#include "Block.h"
#include "PlayScene.h"
#include "ItemPoint.h"
#include "Brick.h"
#include "Goomba.h"
#include "RedGoomba.h"
CTurtle::CTurtle(float x, float y, int pointIdStomp, int pointIdKick)
    : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = TURTLE_GRAVITY;
    shell_start = -1;
    this->walkingDirection = -1;
    this->isBeingHeld = false;
    this->pointIdStomp = pointIdStomp;
    this->pointIdKick = pointIdKick;
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

void CTurtle::SetHolder(CMario* mario) {
    holder = mario;
}

CMario* CTurtle::GetHolder() {
    return holder;
}

void CTurtle::OnCollisionWith(LPCOLLISIONEVENT e)
{
    CBrick* brick = dynamic_cast<CBrick*>(e->obj);
    if (brick && IsShellState())
    {
        DebugOut(L"Rùa va chạm với Brick khi ở trạng thái shell!\n");
        brick->OnCollisionWith(e); 
    }

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
    CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
    if (goomba && state == TURTLE_STATE_SHELL_MOVING && goomba->GetState() == GOOMBA_STATE_WALKING)
    {
        goomba->SetState(GOOMBA_STATE_DIE);
    }

    CRedGoomba* redgoomba = dynamic_cast<CRedGoomba*>(e->obj);
    if (redgoomba && state == TURTLE_STATE_SHELL_MOVING)
    {
        if (redgoomba->GetState() == RED_GOOMBA_STATE_WINGED)
        {
            redgoomba->SetState(RED_GOOMBA_STATE_WALKING);
        }
        else if (redgoomba->GetState() == RED_GOOMBA_STATE_WALKING)
        {
            redgoomba->SetState(RED_GOOMBA_STATE_DIE);
        }
    }

}
void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == TURTLE_STATE_HELD && holder != nullptr) {
        x = holder->GetX() + holder->GetDirection() * 12.0f;
        y = holder->GetY();
    }
    else {
        vx += ax * dt;
        vy += ay * dt;

        if (state == TURTLE_STATE_SHELL)
        {
            if (GetTickCount64() - shell_start > TURTLE_REVIVE_TIMEOUT)
            {
                SetState(TURTLE_STATE_REVIVING);
            }
        }

        if (state == TURTLE_STATE_WALKING || state == TURTLE_STATE_REVIVING)
        {
            edgeSensor->SetPosition(x + walkingDirection * TURTLE_BBOX_WIDTH / 2, y + TURTLE_BBOX_HEIGHT / 2);

            if (!edgeSensor->IsOnHalfSolidBlock(coObjects)) {
                walkingDirection = -walkingDirection;
                vx = walkingDirection * TURTLE_WALKING_SPEED;
            }
        }
    }

    CGameObject::Update(dt, coObjects);

    if (state == TURTLE_STATE_DIE_FALL && y > CGame::GetInstance()->GetBackBufferHeight())
    {
        Delete(); 
    }

    CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CTurtle::Render()
{
    int aniId = ID_ANI_TURTLE_WALKING_LEFT;

    if (state == TURTLE_STATE_SHELL || state == TURTLE_STATE_SHELL_MOVING || state == TURTLE_STATE_HELD)
    {
        aniId = ID_ANI_TURTLE_SHELL; 
    }
    else if (state == TURTLE_STATE_WALKING || state == TURTLE_STATE_REVIVING)
    {
        aniId = (walkingDirection > 0) ? ID_ANI_TURTLE_WALKING_LEFT : ID_ANI_TURTLE_WALKING_RIGHT;
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

        if (!hasSpawnedPointOnStomp)
        {
            hasSpawnedPointOnStomp = true;

            if (CGame::GetInstance()->GetCurrentScene())
            {
                CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
                if (scene)
                {
                    scene->AddObject(new CItemPoint(x, y, pointIdStomp));
                }
            }
        }

        break;

    case TURTLE_STATE_SHELL_MOVING:
        vx = walkingDirection * TURTLE_SHELL_SLIDE_SPEED;
        ay = TURTLE_GRAVITY;
        break;

    case TURTLE_STATE_HELD:
        vx = 0;
        break;

    case TURTLE_STATE_REVIVING:
        if (walkingDirection == 0)
            walkingDirection = -1;
        vx = walkingDirection * TURTLE_WALKING_SPEED;
        break;

    case TURTLE_STATE_DIE_FALL:
        vx = 0;
        vy = 0.15f;
        ay = TURTLE_GRAVITY;
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

    if (!hasSpawnedPointOnKick)
    {
        hasSpawnedPointOnKick = true;

        if (CGame::GetInstance()->GetCurrentScene())
        {
            CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
            if (scene)
            {
                scene->AddObject(new CItemPoint(x, y, pointIdKick));
            }
        }
    }

    SetState(TURTLE_STATE_SHELL_MOVING);
}

bool CTurtle::checkMario()
{
	if (holder != nullptr)
	{
		return true;
	}
	return false;
}
