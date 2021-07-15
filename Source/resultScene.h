#pragma once
#include "../Library/sceneBase.h"

/// <summary>
/// リザルトシーン
/// 
/// リザルトを表示して、キーを押したらタイトルに移行する。
/// </summary>
class ResultScene : public SceneBase {
public:
	ResultScene();
	~ResultScene();
	void Update() override;
	void Draw() override;
private:
	int hImage;//リザルト画像のハンドル
};
