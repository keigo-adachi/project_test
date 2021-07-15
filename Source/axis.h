#pragma once
#include "../Library/gameObject.h"

class Axis :public GameObject{
public:
	Axis(SceneBase*scene):GameObject(scene){}
	Axis(){}
	void Draw() override;
};