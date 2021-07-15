#pragma once
#include "../Library//gameObject.h"

class Camera :public GameObject{
public:
	Camera(SceneBase*scene);
	~Camera();
	void Update()override;
	void Draw()override;
	void SetPosition(VECTOR pos) { position = pos; }
	void SetTarget(VECTOR tgt) { target = tgt; }

private:
	
	VECTOR position; 
	VECTOR target; 
};