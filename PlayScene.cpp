﻿#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Decorations.h"
#include "turle.h"
#include "Block.h"
#include "FullBlock.h"
#include "PipeTeleport.h"
#include "RedGoomba.h"
#include "PiranhaPlant.h"
#include "Bullet.h"
#include "BlockerWall.h"
#include "DropBrick.h"
#include "JumpingKoopas.h"
#include "FlyingKoopas.h"
#include "GreenPiranhaPlant.h"
#include "BoomerangBrother.h"
#include "HUD.h"
#include "SampleKeyEventHandler.h"
#include "ItemBox.h"
#include "BrickN.h"
#define MAX_CAM_X 2610
#define MAX_CAM_Y -2

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		((CMario*)player)->SetLevel(CMario::savedLevel);
		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		if (tokens.size() < 4) return; 
		int pointSpriteId = atoi(tokens[3].c_str());
		obj = new CGoomba(x, y, pointSpriteId);
		break;
	}
	case OBJECT_TYPE_BRICK:
	{
		if (tokens.size() < 7) {
			return;
		}

		int brickType = atoi(tokens[3].c_str());
		int spawnType = atoi(tokens[4].c_str());
		float width = (float)atof(tokens[5].c_str());
		float height = (float)atof(tokens[6].c_str());

		obj = new CBrick(x, y, width, height, brickType, spawnType);

		break;
	}
	case OBJECT_TYPE_DROP_BRICK: obj = new CDropBrick(x, y); DebugOut(L"[INFO] Speed: %f\n", obj->GetVx()); break;

	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_HUD: obj = new CHud(x, y); break;
	case OBJECT_TYPE_TURTLE:
	{
		if (tokens.size() < 5) return;
		int pointIdStomp = atoi(tokens[3].c_str());
		int pointIdKick = atoi(tokens[4].c_str());
		obj = new CTurtle(x, y, pointIdStomp, pointIdKick);
		break;
	}
	case OBJECT_TYPE_REDGOOMBA:
	{
		if (tokens.size() < 5) return;
		int pointIdWinged = atoi(tokens[3].c_str());
		int pointIdWalking = atoi(tokens[4].c_str());
		obj = new CRedGoomba(x, y, pointIdWinged, pointIdWalking);
		break;
	}
	case OBJECT_TYPE_PIRANHA_PLANT: obj = new CPiranhaPlant(x, y); break;
	case OBJECT_TYPE_GREEN_PIRANHA_PLANT: obj = new CGreenPiranhaPlant(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int width = atoi(tokens[9].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, width
		);

		break;
	}

	case OBJECT_TYPE_DECORATION:
	{
		int spriteId = atoi(tokens[3].c_str());
		obj = new CDecoration(x, y, spriteId);
		break;
	}


	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_HALF_SOLID_BLOCK:
	{
		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());
		obj = new CHalfSolidBlock(x, y, w, h, spriteId);
		break;
	}
	case OBJECT_TYPE_BLOCKING:
	{
		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());
		int spriteId = atoi(tokens[5].c_str());
		obj = new CFullBlock(x, y, w, h, spriteId);
		break;
	}
	case OBJECT_TYPE_PIPE_TELEPORT:
	{
		float width = (float)atof(tokens[3].c_str());
		float height = (float)atof(tokens[4].c_str());
		float targetX = (float)atof(tokens[5].c_str());
		float targetY = (float)atof(tokens[6].c_str());
		obj = new CPipeTeleport(x, y, width, height, targetX, targetY);
		obj->SetObjectType(OBJECT_TYPE_PIPE_TELEPORT);
		break;
	}
	case OBJECT_TYPE_BOOMERANG_BROTHER:
		obj = new CBoomerangBrother(x, y);
		break;
	case OBJECT_TYPE_MARIO_BLOCKER:
		obj = new CBlockerWall(x, y);
		break;

	break;
	case OBJECT_TYPE_JUMPING_KOOPAS:
	{
		if (tokens.size() < 6) return;

		int pointJump = atoi(tokens[3].c_str());
		int pointWalk = atoi(tokens[4].c_str());
		int pointKick = atoi(tokens[5].c_str());

		obj = new CJumpingKoopas(x, y, pointJump, pointWalk, pointKick);
		break;
	}
	case OBJECT_TYPE_FLYING_KOOPAS:
	{
		if (tokens.size() < 6) return;

		int pointFly = atoi(tokens[3].c_str());
		int pointWalk = atoi(tokens[4].c_str());
		int pointKick = atoi(tokens[5].c_str());

		obj = new CFlyingKoopas(x, y, pointFly, pointWalk, pointKick);
		break;
	}
	case OBJECT_TYPE_ITEMBOX:
		obj = new CItemBox(x, y);
		break;
	case OBJECT_TYPE_STRETCHABLE_BLOCK:
	{
		if (tokens.size() < 13) return;

		int cellSize = atoi(tokens[3].c_str());
		int w = atoi(tokens[4].c_str());
		int h = atoi(tokens[5].c_str());

		int spriteIds[9];
		for (int i = 0; i < 9; ++i) {
			spriteIds[i] = atoi(tokens[6 + i].c_str());
		}

		obj = new CStretchableBlock(x, y, cellSize, w, h, spriteIds);
		break;
	}
	case OBJECT_TYPE_BRICKN:
	{
		if (tokens.size() < 5) {
			return;
		}

		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());

		obj = new CBrickN(x, y, w, h);
		break;
	}

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	wstring filePath(sceneFilePath);
	if (filePath.find(L"scene02.txt") != wstring::npos)
	{
		isFollowingMario = false;
		camIntroX = 0.0f;
	}
	else
	{
		isFollowingMario = true; 
	}

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	float camX, camY;
	CGame::GetInstance()->GetCamPos(camX, camY);

	float screenW = CGame::GetInstance()->GetBackBufferWidth();
	float screenH = CGame::GetInstance()->GetBackBufferHeight();
	float padding = 200.0f;

	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT obj = objects[i];

		if (dynamic_cast<CMario*>(player) && ((CMario*)player)->IsTransforming())
		{
			if (obj != player && obj->GetType() != TYPE_ITEM_POINT)
				continue;
		}

		float ox, oy;
		obj->GetPosition(ox, oy);

		if (ox >= camX - padding && ox <= camX + screenW + padding)
		{
			obj->Update(dt, &objects);
		}

		CMario* mario = dynamic_cast<CMario*>(player);
		if (mario && mario->tailAttack)
		{
			mario->tailAttack->Update(dt, &objects);
		}
	}

	if (player == NULL) return;

	CGame* game = CGame::GetInstance();
	float px, py;
	player->GetPosition(px, py);

	float cx, cy;
	float screenWidth = game->GetBackBufferWidth();
	float screenHeight = game->GetBackBufferHeight();

	// ======= SCENE 2: Intro camera scroll =========
	if (id == 2 && !isFollowingMario)
	{
		camIntroX += 0.04f * dt;

		if (camIntroX >= 1685.0f)
		{
			camIntroX = 1685.0f;
		}

		game->SetCamPos(camIntroX, 0.0f);

		float marioX, marioY;
		player->GetPosition(marioX, marioY);

		if (camIntroX >= 1685.0f && marioX >= 2000.0f)
		{
			isFollowingMario = true;
		}
	}



	else
	{
		cx = px - screenWidth / 2.0f;
		if (cx < 0) cx = 0;
		else if (cx > MAX_CAM_X) cx = MAX_CAM_X;

		float current_cx, current_cy;
		game->GetCamPos(current_cx, current_cy);

		if (((CMario*)player)->IsEnteringPipe())
		{
			cy = current_cy;
			cx = current_cx;
		}
		else
		{
			cy = py - screenHeight / 2.0f;
			if (cy > 200.0f)
			{
				current_cy = 387.0f;
			}
			else if (cy < -150)
			{
				current_cy += (cy - current_cy) * 0.1f;
			}
			else
			{
				current_cy += (0.0f - current_cy) * 0.1f;
			}
			cy = current_cy;
		}

		if (isShaking) {
			if (GetTickCount64() - shake_start <= 500) {
				float shakeOffset = rand() % 8 - 4; 
				cy += shakeOffset;
			}
			else {
				isShaking = false;
			}
		}

		if (((CMario*)player)->IsDead())
		{
			game->SetCamPos(cx, MAX_CAM_Y);
		}
		else
		{
			game->SetCamPos(cx, cy);
		}
	}

	PurgeDeletedObjects();
}


void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than	deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}
void CPlayScene::AddObject(LPGAMEOBJECT obj)
{
	objects.push_back(obj);
}
