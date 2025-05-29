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
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (mario)
    {
        coin = mario->GetCoin(); // Cập nhật số xu Mario có
        score = mario->GetScore(); // Cập nhật điểm số
        time = mario->GetTime(); // Cập nhật thời gian còn lại
        itemBox[0] = mario->GetItemBox(0);
        itemBox[1] = mario->GetItemBox(1);
        itemBox[2] = mario->GetItemBox(2);
    }
}

void CHud::Render()
{
    float hudX = 100;
    float hudY = 100;
    CSprites::GetInstance()->Get(ID_SPRITE_HUD)->DrawStatic(hudX, hudY);

    int coinDigit1 = coin / 10;
    int coinDigit2 = coin % 10;
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + coinDigit1)->DrawStatic(hudX + 50, hudY);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + coinDigit2)->DrawStatic(hudX + 60, hudY);

    int tempScore = score;
    for (int i = 5; i >= 0; i--)
    {
        int digit = tempScore % 10;
        tempScore /= 10;
        CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + digit)->DrawStatic(hudX + (i * 10), hudY + 20);
    }

    int timeDigit1 = time / 100;
    int timeDigit2 = (time / 10) % 10;
    int timeDigit3 = time % 10;
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit1)->DrawStatic(hudX + 100, hudY);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit2)->DrawStatic(hudX + 110, hudY);
    CSprites::GetInstance()->Get(ID_SPRITE_NUMBER_0 + timeDigit3)->DrawStatic(hudX + 120, hudY);

    float itemBoxX = hudX + 150;
    float itemBoxY = hudY + 10;
    for (int i = 0; i < 3; i++)
    {
        int itemSpriteId = ID_SPRITE_ITEM_BOX_4; 
        if (itemBox[i] == 1) itemSpriteId = ID_SPRITE_ITEM_BOX_1; 
        else if (itemBox[i] == 2) itemSpriteId = ID_SPRITE_ITEM_BOX_2; 
        else if (itemBox[i] == 3) itemSpriteId = ID_SPRITE_ITEM_BOX_3; 

        CSprites::GetInstance()->Get(itemSpriteId)->DrawStatic(itemBoxX + (i * 16), itemBoxY);
    }

}
