#include "ResultScene.h"
#include "../Library//sceneManager.h"
#include "../Source/debugScreen.h"

ResultScene::ResultScene()
{
	hImage = LoadGraph("data\\texture\\Score.png");
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update()
{
	//シーンの切り替え
	if (CheckHitKey(KEY_INPUT_T))
	{
		SceneManager::ChangeScene("TitleScene");//タイトル
	}
	SceneBase::Update();
}

void ResultScene::Draw()
{
	DrawGraph(0, 0, hImage, 0);
	DrawFormatString(400, 400, GetColor(0, 0, 0),"Score:");//スコア(文字)
	SetFontSize(50);
	SceneBase::Draw();
}
