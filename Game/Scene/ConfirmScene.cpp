#include "ConfirmScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "../Sound/SoundManager.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "../MyDebug/DebugText.h"
#include "../String/MessageManager.h"

namespace
{
	// ���ڂ̑I������Ă��Ȃ��Ƃ��̕`��F
	constexpr unsigned int normal_color = 0x444444;

	// ���ڂ̑I������Ă���Ƃ��̕`��F
	constexpr unsigned int choose_color = 0xffffff;
}

// �R���X�g���N�^
ConfirmScene::ConfirmScene(SceneManager& manager, PuseScene::State state) :
	SceneBase(manager),
	m_currentSelectItem(0),
	m_gaussScreen(-1),
	m_state(state)
{
}

// �f�X�g���N�^
ConfirmScene::~ConfirmScene()
{
}

// ������
void ConfirmScene::Init()
{
	// ���U�C�N�����p�̃O���t�B�b�N�̍쐬
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

	// ���ڂ̕`��F��I������Ă��Ȃ��Ƃ��̐F�ɏ�����
	for (int i = 0; i < static_cast<int>(State::NUM); i++)
	{
		m_itemColorTable.push_back(normal_color);
	}
}

// �I������
void ConfirmScene::End()
{
	// ���U�C�N�����p�̃O���t�B�b�N�̍폜
	DeleteGraph(m_gaussScreen);
}

// �X�V
void ConfirmScene::Update()
{
	// �J���[�̏�����
	for (auto& itemColor : m_itemColorTable)
	{
		itemColor = normal_color;
	}

	// �I�������񂷏���
	int itemTotalValue = static_cast<int>(ConfirmScene::State::NUM);
	if (InputState::IsTriggered(InputType::LEFT))
	{
		m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
		Sound::Manager::GetInstance()->PlaySE("Select");
	}
	else if (InputState::IsTriggered(InputType::RIGHT))
	{
		m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
		Sound::Manager::GetInstance()->PlaySE("Select");
	}

	// �I������Ă��鍀�ڂ̐F��ς���
	m_itemColorTable[m_currentSelectItem] = choose_color;

	// ����
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// �ǂ��̃V�[���ɖ߂邩
		switch (static_cast<ConfirmScene::State>(m_currentSelectItem))
		{
		case State::YES:
			switch (m_state)
			{
			case PuseScene::State::STAGE_SELECT:
				m_manager.ChangeAndClearScene(std::make_shared<StageSelectScene>(m_manager));
				break;

			case PuseScene::State::TITLE:
				m_manager.ChangeAndClearScene(std::make_shared<TitleScene>(m_manager));
				break;
			}
			break;

		case State::NO:
			m_manager.PopScene();
			break;
		}
	}

	// �߂�
	if (InputState::IsTriggered(InputType::BACK))
	{
		m_manager.PopScene();
		return;
	}
}

// �`��
void ConfirmScene::Draw()
{
	Debug::Text::AddLog("currentSelectItem", {m_currentSelectItem});

	// ���U�C�N����
	const auto& size = Application::GetInstance().GetWindowSize();
	GetDrawScreenGraph(0, 0, size.width, size.height, m_gaussScreen);
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, 8, 600);
	DrawGraph(0, 0, m_gaussScreen, true);

	// ��ʂ��Â����鏈��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0, 0, size.width, size.height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �E�B���h�E�̕`��
	DrawBox(0, size.height / 2.0f - 200, size.width, size.height / 2.0f + 200, 0x555555, true);

	// ���b�Z�[�W�̕`��
	String::MessageManager::GetInstance()->DrawStringCenter("ConfirmBack", size.width / 2.0f, size.height / 2.0f - 100, 0xffffff);
	String::MessageManager::GetInstance()->DrawStringCenter("ConfirmYes", size.width / 2.0f - 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(ConfirmScene::State::YES)]);
	String::MessageManager::GetInstance()->DrawStringCenter("ConfirmNo", size.width / 2.0f + 100, size.height / 2.0f + 50, m_itemColorTable[static_cast<int>(ConfirmScene::State::NO)]);
}
