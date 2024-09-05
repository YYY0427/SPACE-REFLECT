#include "DebugScene.h"
#include "TitleScene.h"
#include "TestScene.h"
#include "OptionScene.h"
#include "StageSelectScene.h"
#include "SceneManager.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "DxLib.h"

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	const int draw_text_pos_x = Application::GetInstance()->GetWindowSize().width / 2 - 100;
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 32;
}

// �R���X�g���N�^
DebugScene::DebugScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0)
{
}

// �f�X�g���N�^
DebugScene::~DebugScene()
{
}

// ������
void DebugScene::Init()
{
}

// �I������
void DebugScene::End()
{
}

// �X�V
void DebugScene::Update()
{
	// �I�������񂷏���
	int sceneItemTotalValue = static_cast<int>(SceneItem::NUM);
	if (InputState::IsTriggered(InputType::UP))
	{
		m_currentSelectSceneItem = ((m_currentSelectSceneItem - 1) + sceneItemTotalValue) % sceneItemTotalValue;
	}
	else if (InputState::IsTriggered(InputType::DOWN))
	{
		m_currentSelectSceneItem = (m_currentSelectSceneItem + 1) % sceneItemTotalValue;
	}

	// ����{�^���������ꂽ��V�[���J��
	if (InputState::IsTriggered(InputType::DECISION))
	{
		switch(static_cast<SceneItem>(m_currentSelectSceneItem))
		{
		case SceneItem::TITLE:
			m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
			return;
		case SceneItem::STAGE_SELECT:
			m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
			return;
		case SceneItem::PAUSE:
			return;
		case SceneItem::OPTION:
			m_manager.PushScene(std::make_shared<OptionScene>(m_manager, OptionScene::State::PAUSE));
			return;
		case SceneItem::TEST:
			m_manager.ChangeScene(std::make_shared<TestScene>(m_manager));
			return;
		case SceneItem::EXIT:
			Application::GetInstance()->Exit();
			return;
		}
	}
}

// �`��
void DebugScene::Draw()
{
	// ��ʂ��N���A
	ClearDrawScreen();

	// �w�i�̕`��
	DrawBox(0, 0, Application::GetInstance()->GetWindowSize().width, Application::GetInstance()->GetWindowSize().height, 0x000000, true);

	// �f�o�b�O�V�[�������ׂ�V�[���̍��ڂ̃e�L�X�g�\��
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TITLE), "TITLE", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::STAGE_SELECT), "STAGE_SELECT", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::PAUSE), "PAUSE", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::OPTION), "OPTION", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::TEST), "TEST", 0xffffff, true);
	DrawString(draw_text_pos_x, draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::EXIT), "EXIT", 0xffffff, true);

	// ���ݑI�𒆂̍��ڂ̉��Ɂ���\��
	DrawString(draw_text_pos_x - text_space_y, draw_text_pos_y + text_space_y * m_currentSelectSceneItem, "��", 0xff0000);
}