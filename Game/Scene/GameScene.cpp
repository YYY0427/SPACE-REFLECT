#include "GameScene.h"
#include "OptionScene.h"
#include "../Application.h"
#include "../Stage/Tutorial.h"
#include "../Stage/Stage1.h"
#include "../Util/InputState.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/TileTransitor.h"
#include "../Scene/SceneManager.h"
#include "../Scene/PuseScene.h"

// コンストラクタ
GameScene::GameScene(SceneManager& manager, Stage stage) :
	SceneBase(manager),
	m_stage(stage)
{
	// 選ばれたステージの作成
	switch (m_stage)
	{
	case Stage::TUTORIAL:
		m_pStage = std::make_unique<Tutorial>(manager);
		break;
	case Stage::STAGE_1:
		m_pStage = std::make_unique<Stage1>(manager);
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
	// ステージの更新
	m_pStage->Update();

	// ポーズ
	if (InputState::IsTriggered(InputType::PAUSE))
	{
	//	m_manager.PushScene(std::make_shared<OptionScene>(m_manager, OptionScene::State::PAUSE));
		m_manager.PushScene(std::make_shared<PuseScene>(m_manager, m_stage));
		return;
	}
}

// 描画
void GameScene::Draw()
{
	// ステージの描画
	m_pStage->Draw();
}