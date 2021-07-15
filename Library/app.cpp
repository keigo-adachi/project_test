#include "App.h"
#include "sceneManager.h"

void AppInit()
{
	SceneManager::Start();
}

void AppUpdate()
{
	SceneManager::Update();
}

void AppDraw()
{
	SceneManager::Draw();
}

void AppRelease()
{
	SceneManager::Release();
}