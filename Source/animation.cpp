#include "animation.h"
#include "DxLib.h"
Animation::Animation(int _hModel)
{
	hModel = _hModel;
	hAnim = -1;
	prevhAnim = -1;
	frameTime = 0.0f;
	frameTimeMax = 0.0f;
}


Animation::~Animation()
{
}

void Animation::Update()
{
	frameTime += 0.5f;
	if (isLoop)
	{
		if (frameTime >= frameTimeMax)
			frameTime -= frameTimeMax;
	}
	else
	{
		if (frameTime >= frameTimeMax)
			frameTime = frameTimeMax;
	}
	
	MV1SetAttachAnimTime(hModel, hAnim, frameTime);
}

void Animation::Start(int _hAnim, bool loop)
{
	
	if (prevhAnim == _hAnim)
	{
		return;
	}
	if (hAnim >= 0) //今アニメーションがある
	{
		MV1DetachAnim(hModel, 0);
	}
	prevhAnim = _hAnim;
	isLoop = loop;
	hAnim = MV1AttachAnim(hModel, _hAnim, -1, FALSE);
	frameTimeMax = MV1GetAttachAnimTotalTime(hModel,hAnim);
	frameTime = 0.0f;
}

bool Animation::IsStart()
{
	return (frameTime == 0);
}


bool Animation::IsFinish()
{
	return(frameTime >= frameTimeMax);
}