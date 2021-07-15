#include "playScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"
#include "axis.h"
#include "player.h"
#include "enemy.h"
#include "field.h"
#include "camera.h"
PlayScene::PlayScene()
{
	CreateGameObject<Axis>();	//方角を表示
	CreateGameObject<Player>(); //プレイヤーの生成
	CreateGameObject<Enemy>();	//エネミーの生成
	CreateGameObject<Field>();	//フィールドの生成
	Camera *pCam = CreateGameObject<Camera>(); //カメラの生成
	SetDrawOrder(pCam, 1);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	//シーンの切り替え
	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TitleScene");//タイトル
	}
	if (CheckHitKey(KEY_INPUT_R)) 
	{
		SceneManager::ChangeScene("ResultScene");//リザルト
	}
	SceneBase::Update();
}

void PlayScene::Draw()
{
	SceneBase::Draw();
}
