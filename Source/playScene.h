#pragma once
#include "../Library/sceneBase.h"

/// <summary>
/// プレイシーン
/// </summary>
class PlayScene : public SceneBase {
public:
	PlayScene();
	~PlayScene();
	void Update() override;
	void Draw() override;
};