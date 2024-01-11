#include "GameScene.h"
#include "../Stage/Tutorial.h"

// コンストラクタ
GameScene::GameScene(SceneManager& manager, Stage stage) :
	SceneBase(manager),
	m_stage(stage)
{
	switch (m_stage)
	{
	case Stage::TUTORIAL:
		m_pStage = std::make_unique<Tutorial>();
		break;
	}
}

// デストラクタ
GameScene::~GameScene()
{
}

// 更新
void GameScene::Update()
{
	m_pStage->Update();
}

// 描画
void GameScene::Draw()
{
	m_pStage->Draw();
}
