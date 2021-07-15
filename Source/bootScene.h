#pragma once

#include "../Library/sceneBase.h"

class BootScene : public SceneBase {
public:
	BootScene();
	~BootScene();
	void Update() override;
	void Draw() override;
};