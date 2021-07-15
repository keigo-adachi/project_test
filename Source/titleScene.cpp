#include "titleScene.h"
#include "../Library/sceneManager.h"
#include "DebugScreen.h"
TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (CheckHitKey(KEY_INPUT_RETURN)) 	
	{
		 SceneManager::ChangeScene("PlayScene"); //プレイシーン
	}
	if (CheckHitKey(KEY_INPUT_ESCAPE))
	{
		DxLib_End();
	}
	SceneBase::Update();
}

void TitleScene::Draw()
{
	SceneBase::Draw();
}
