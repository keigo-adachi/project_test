#include "axis.h"

void Axis::Draw()
{
	DrawLine3D(VGet(-200, 0, 0), VGet(200, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(VGet(0,-200, 0), VGet(0, 200, 0), GetColor(0, 255, 0));
	DrawLine3D(VGet(0,0, -200), VGet(0, 0, 200), GetColor(0, 0, 255));
	DrawCone3D(VGet(200,0,0),VGet(180,0,0),5.0f,20,GetColor(255,0,0),GetColor(255,0,0),TRUE);
	DrawCone3D(VGet(0,200,0),VGet(0,180,0),5.0f,20,GetColor(0,255,0),GetColor(0,255,0),TRUE);
	DrawCone3D(VGet(0,0,200),VGet(0,0,180),5.0f,20,GetColor(0,0,255),GetColor(0,0,255),TRUE);
}
