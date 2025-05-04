#pragma once
#include "GameObject.h"

class CMario;

class CPipeTeleport : public CGameObject
{
protected:
    float width, height;
    float targetX, targetY;
    bool isTriggered = false;

public:
    CPipeTeleport(float x, float y, float width, float height, float tx, float ty);

    void OnCollisionWith(CMario* mario);
    void Render() override;
    void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects = NULL) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
