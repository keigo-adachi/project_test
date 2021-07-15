#pragma once
#include "../Library/gameObject.h"
class Field : public GameObject{
public:
	Field(SceneBase* scene);
	~Field();
	void Update() override;
	void Draw() override;
	bool CollisionLine(VECTOR* hit, VECTOR form, VECTOR to);
private:
	int hModel;
	int hSkyModel;
	float frame;
};