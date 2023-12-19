#include "DebugScene.h"
#include "DxLib.h"

DebugScene::DebugScene(SceneManager& manager) :
	SceneBase(manager)
{
}

DebugScene::~DebugScene()
{
}

void DebugScene::Update()
{
}

void DebugScene::Draw()
{
	DrawString(50, 50, "unnko", 0xffffff);
}