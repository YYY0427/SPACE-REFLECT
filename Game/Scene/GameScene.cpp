#include "GameScene.h"
#include "OptionScene.h"
#include "../Application.h"
#include "../Stage/Tutorial.h"
#include "../Stage/Stage1.h"
#include "../Input/InputManager.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/TileTransitor.h"
#include "../Scene/SceneManager.h"
#include "../Scene/PuseScene.h"

// �R���X�g���N�^
GameScene::GameScene(SceneManager& manager, const Stage stage) :
	SceneBase(manager),
	m_stage(stage)
{
}

// �f�X�g���N�^
GameScene::~GameScene()
{
}

// ������
void GameScene::Init()
{
	// �I�΂ꂽ�X�e�[�W�̍쐬
	switch (m_stage)
	{
	case Stage::TUTORIAL:
		m_pStage = std::make_unique<Tutorial>(m_manager);
		break;
	case Stage::STAGE_1:
		m_pStage = std::make_unique<Stage1>(m_manager);
		break;
	}
}

// �I������
void GameScene::End()
{
}

// �X�V
void GameScene::Update()
{
	// �X�e�[�W�̍X�V
	m_pStage->Update();

	// �|�[�Y
	if (Input::Manager::IsTriggered(Input::Type::PAUSE))
	{
		m_manager.PushScene(std::make_shared<PuseScene>(m_manager, m_stage));
		return;
	}
}

// �`��
void GameScene::Draw()
{
	// �X�e�[�W�̕`��
	m_pStage->Draw();
}