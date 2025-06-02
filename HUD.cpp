#include "Hud.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Sprites.h"

CHud::CHud(float x, float y) : CGameObject(x, y), coin(0), score(0), time(300) // Thời gian mặc định là 300 giây
{
    for (int i = 0; i < 3; i++) itemBox[i] = 0;
}

void CHud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    ULONGLONG now = GetTickCount64();
    int elapsed = (int)((now - startTime) / 1000);
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (mario)
    {

        time = max(0, 300 - elapsed);  // Cập nhật thời gian còn lại
        itemBox[0] = 1;
        itemBox[1] = 1;
        itemBox[2] = 1;
    }
}

void CHud::Render()
{
    float hudX = 84;
    float hudY = 215;
    CSprites::GetInstance()->Get(ID_SPRITE_BLACK)->DrawStatic(hudX + 50, hudY + 30);
    CSprites::GetInstance()->Get(ID_SPRITE_HUD)->DrawStatic(hudX, hudY);
    
    int coinDigit1 = coin / 10;
    int coinDigit2 = coin % 10;
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + coinDigit1)->DrawStatic(hudX + 61, hudY - 10);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + coinDigit2)->DrawStatic(hudX + 68, hudY - 10);

    int tempScore = score;
    for (int i = 5; i >= 0; i--)
    {
        int digit = tempScore % 10;
        tempScore /= 10;
        CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + digit)->DrawStatic(hudX - 20+ (i * 10), hudY - 1);
    }

    int timeDigit1 = time / 100;
    int timeDigit2 = (time / 10) % 10;
    int timeDigit3 = time % 10;
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit1)->DrawStatic(hudX + 53, hudY - 1);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit2)->DrawStatic(hudX + 61, hudY - 1);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit3)->DrawStatic(hudX + 69, hudY - 1);

    float itemBoxX = hudX + 150;
    float itemBoxY = hudY + 10;
    for (int i = 0; i < 3; i++)
    {
        int itemSpriteId = ID_SPRITE_ITEM_BOX_4; 
        if (itemBox[i] == 1) itemSpriteId = ID_SPRITE_ITEM_BOX_1; 
        else if (itemBox[i] == 2) itemSpriteId = ID_SPRITE_ITEM_BOX_2; 
        else if (itemBox[i] == 3) itemSpriteId = ID_SPRITE_ITEM_BOX_3; 

        CSprites::GetInstance()->Get(itemSpriteId)->DrawStatic(itemBoxX - 50 + (i * 25), itemBoxY - 15);
    }


}
