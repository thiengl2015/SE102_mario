#include "JumpingKoopas.h"
#include "Game.h"
#include "Platform.h"
#include "PlayScene.h"
#include "Brick.h"
#include "ItemPoint.h"

CJumpingKoopas::CJumpingKoopas(float x, float y, int pointJump, int pointWalk, int pointKick)
    : CGameObject(x, y)
{
    this->ax = 0;
    this->ay = JKOOPAS_GRAVITY;
    this->walkingDirection = -1;
    this->isBeingHeld = false;
    this->shell_start = -1;
    this->jump_start = GetTickCount64();

    this->pointIdJump = pointJump;
    this->pointIdWalk = pointWalk;
    this->pointIdKick = pointKick;

    SetState(JKOOPAS_STATE_JUMPING);
}

void CJumpingKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (IsShellState())
    {
        left = x - JKOOPAS_BBOX_WIDTH / 2;
        top = y - JKOOPAS_BBOX_HEIGHT_DIE / 2;
        right = left + JKOOPAS_BBOX_WIDTH;
        bottom = top + JKOOPAS_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - JKOOPAS_BBOX_WIDTH / 2;
        top = y - JKOOPAS_BBOX_HEIGHT / 2;
        right = left + JKOOPAS_BBOX_WIDTH;
        bottom = top + JKOOPAS_BBOX_HEIGHT;
    }
}

void CJumpingKoopas::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CJumpingKoopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
    // Không xử lý va chạm với chính mình
    if (e->obj == this) return;

    // Shell Moving mới xử lý knock out
    if (state == JKOOPAS_STATE_SHELL_MOVING)
    {

        // Va chạm với JumpingKoopas khác
        CJumpingKoopas* otherKoopas = dynamic_cast<CJumpingKoopas*>(e->obj);
        if (otherKoopas && !otherKoopas->IsBeingHeld() && otherKoopas != this)
        {
            otherKoopas->SetState(JKOOPAS_STATE_DIE_FALL);
            return;
        }
    }

    // Còn lại: xử lý cơ bản
    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
        vy = 0;
    else if (e->nx != 0)
    {
        vx = -vx;
        walkingDirection = -walkingDirection;
    }
}



void CJumpingKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == JKOOPAS_STATE_HELD && holder != nullptr)
    {
        x = holder->GetX() + holder->GetDirection() * 12.0f;
        y = holder->GetY();
        return;
    }

    vy += ay * dt;
    vx += ax * dt;

    if (state == JKOOPAS_STATE_SHELL && GetTickCount64() - shell_start > JKOOPAS_REVIVE_TIMEOUT)
    {
        SetState(JKOOPAS_STATE_REVIVING);
        SetPosition(x, y - JKOOPAS_BBOX_HEIGHT / 2); 
    }

    if (state == JKOOPAS_STATE_JUMPING && GetTickCount64() - jump_start > JKOOPAS_JUMP_INTERVAL)
    {
        vy = -JKOOPAS_JUMP_SPEED_Y;
        jump_start = GetTickCount64();
    }
    if (state == JKOOPAS_STATE_DIE_FALL)
    {
        vy += ay * dt;
        y += vy * dt;

        return;
    }

    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CJumpingKoopas::Render()
{
    int aniId = -1;
    switch (state)
    {
    case JKOOPAS_STATE_JUMPING:
        aniId = (walkingDirection > 0) ? ID_ANI_JKOOPAS_JUMP_RIGHT : ID_ANI_JKOOPAS_JUMP_LEFT;
        break;
    case JKOOPAS_STATE_WALKING:
    case JKOOPAS_STATE_REVIVING:
        aniId = (walkingDirection > 0) ? ID_ANI_JKOOPAS_WALK_RIGHT : ID_ANI_JKOOPAS_WALK_LEFT;
        ay = JKOOPAS_GRAVITY;
        break;
    case JKOOPAS_STATE_SHELL:
    case JKOOPAS_STATE_SHELL_MOVING:
    case JKOOPAS_STATE_HELD:
	case JKOOPAS_STATE_DIE_FALL:
        aniId = ID_ANI_JKOOPAS_SHELL;
        break;

    }

    CAnimation* ani = CAnimations::GetInstance()->Get(aniId);
    if (ani)
    {
        DebugOut(L"[JumpingKoopas] Render aniId = %d\n", aniId);
        ani->Render(x, y);
    }
    else
    {
        DebugOut(L"[ERROR] Animation ID %d NOT FOUND!!!\n", aniId);
    }
}


void CJumpingKoopas::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case JKOOPAS_STATE_JUMPING:
        vx = walkingDirection * JKOOPAS_WALKING_SPEED;
        break;

    case JKOOPAS_STATE_WALKING:
        vx = walkingDirection * JKOOPAS_WALKING_SPEED;
        if (!hasSpawnedPointJump)
        {
            hasSpawnedPointJump = true;
            auto scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            if (scene)
                scene->AddObject(new CItemPoint(x, y, pointIdJump, 100));
        }
        break;

    case JKOOPAS_STATE_SHELL:
        vx = 0;
        shell_start = GetTickCount64();
        if (!hasSpawnedPointWalk)
        {
            hasSpawnedPointWalk = true;
            auto scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            if (scene)
                scene->AddObject(new CItemPoint(x, y, pointIdWalk, 200));
        }
        break;

    case JKOOPAS_STATE_SHELL_MOVING:
        vx = walkingDirection * JKOOPAS_SHELL_SLIDE_SPEED;
        if (!hasSpawnedPointKick)
        {
            hasSpawnedPointKick = true;
            auto scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
            if (scene)
                scene->AddObject(new CItemPoint(x, y, pointIdKick, 400));
        }
        break;

    case JKOOPAS_STATE_HELD:
        vx = 0;
        break;

    case JKOOPAS_STATE_REVIVING:
        vx = walkingDirection * JKOOPAS_WALKING_SPEED;
        break;
    case JKOOPAS_STATE_DIE_FALL:  
        vy = -0.5f; 
        ay = JKOOPAS_GRAVITY;
        vx = 0;
        break;

    }

}

bool CJumpingKoopas::IsShellState() const
{
    return (state == JKOOPAS_STATE_SHELL || state == JKOOPAS_STATE_SHELL_MOVING || state == JKOOPAS_STATE_HELD);
}

void CJumpingKoopas::KickShell(int dir)
{
    walkingDirection = dir;
    SetState(JKOOPAS_STATE_SHELL_MOVING);
}

void CJumpingKoopas::SetHolder(CMario* mario)
{
    holder = mario;
}

CMario* CJumpingKoopas::GetHolder()
{
    return holder;
}
