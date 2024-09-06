#include "PuseScene.h"
#include "SceneManager.h"
#include "OptionScene.h"
#include "StageSelectScene.h"
#include "TitleScene.h"
#include "ConfirmScene.h"
#include "../Input/InputManager.h"
#include "../Sound/SoundManager.h"
#include "../MyDebug/DebugText.h"
#include "../Application.h"
#include "../String/MessageManager.h"

namespace
{ 
	// ���U�C�N�̎g�p�s�N�Z����(8, 16, 32 �̉��ꂩ)
	constexpr int gauss_param_pixel = 8;

	// ���U�C�N�̋���(100�Ŗ�1�s�N�Z�����̕�)
	constexpr int gauss_param_power = 600;

	// ��ʂ��Â����鋭��(�ő�255)
	constexpr int dark_param_power = 200;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 64;

	// �I������Ă��Ȃ��Ƃ��̕`��F
	constexpr unsigned int normal_color = 0x444444;

	// �I������Ă���Ƃ��̕`��F
	constexpr unsigned int choose_color = 0xffffff;

	// �\������e�L�X�g�̑S�̂̈ʒu
	const int draw_text_pos_x = Application::GetInstance()->GetWindowSize().width / 2;
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 - 50;
}

namespace Scene
{
	// �R���X�g���N�^	
	Puse::Puse(const std::shared_ptr<Scene::Manager>& pSceneManager, const Stage stage) :
		Base(pSceneManager),
		m_currentSelectItem(0),
		m_stage(stage),
		m_gaussScreen(-1)
	{
	}

	// �f�X�g���N�^
	Puse::~Puse()
	{
	}

	// ������
	void Puse::Init()
	{
		// ���U�C�N�����p�̃O���t�B�b�N�̍쐬
		auto& screenSize = Application::GetInstance()->GetWindowSize();
		m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

		// ���ڂ̕`��F��I������Ă��Ȃ��Ƃ��̐F�ɏ�����
		for (int i = 0; i < static_cast<int>(State::NUM); i++)
		{
			m_itemColorTable.push_back(normal_color);
		}
	}

	// �I������
	void Puse::End()
	{
		// ���U�C�N�����p�̃O���t�B�b�N�̍폜
		DeleteGraph(m_gaussScreen);
	}

	// �X�V
	void Puse::Update()
	{
		// �J���[�̏�����
		for (auto& itemColor : m_itemColorTable)
		{
			itemColor = normal_color;
		}

		// �I�������񂷏���
		int itemTotalValue = static_cast<int>(State::NUM);
		if (Input::Manager::IsTriggered(Input::Type::UP))
		{
			m_currentSelectItem = ((m_currentSelectItem - 1) + itemTotalValue) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}
		else if (Input::Manager::IsTriggered(Input::Type::DOWN))
		{
			m_currentSelectItem = (m_currentSelectItem + 1) % itemTotalValue;
			Sound::Manager::GetInstance()->PlaySE("Select");
		}

		// �I������Ă��鍀�ڂ̐F��ς���
		m_itemColorTable[m_currentSelectItem] = choose_color;

		// ����
		if (Input::Manager::IsTriggered(Input::Type::DECISION))
		{
			// �I������Ă��鍀�ڂ��ǂꂩ
			switch (static_cast<State>(m_currentSelectItem))
			{
				// ������
			case State::CONTINUE:
				m_pSceneManager->PopScene();
				break;

				// �I�v�V����
			case State::OPTION:
				m_pSceneManager->PushScene(std::make_shared<Scene::Option>(m_pSceneManager, Scene::Option::State::PAUSE));
				break;

				// �X�e�[�W�Z���N�g�ɖ߂�
			case State::STAGE_SELECT:
				m_pSceneManager->PushScene(std::make_shared<Scene::Confirm>(m_pSceneManager, State::STAGE_SELECT));
				break;

				// �^�C�g���ɖ߂�
			case State::TITLE:
				m_pSceneManager->PushScene(std::make_shared<Scene::Confirm>(m_pSceneManager, State::TITLE));
				break;
			}
		}

		// �|�[�Y����
		if (Input::Manager::IsTriggered(Input::Type::PAUSE) ||
			Input::Manager::IsTriggered(Input::Type::BACK))
		{
			m_pSceneManager->PopScene();
			return;
		}
	}

	// �`��
	void Puse::Draw()
	{
		// �f�o�b�O�p���O
		Debug::Text::AddLog("currentSelectState", { m_currentSelectItem });

		// ��ʃT�C�Y�̎擾
		auto& screenSize = Application::GetInstance()->GetWindowSize();

		// ���U�C�N����
		GetDrawScreenGraph(0, 0, screenSize.width, screenSize.height, m_gaussScreen);
		GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_param_pixel, gauss_param_power);
		DrawGraph(0, 0, m_gaussScreen, true);

		// ��ʂ��Â�����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, dark_param_power);
		DrawBox(0, 0, screenSize.width, screenSize.height, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���b�Z�[�W�}�l�[�W���̎擾
		const auto& messageManager = String::MessageManager::GetInstance();

		// ���j���[�̕`��
		messageManager->DrawStringCenter("PauseMenu", screenSize.width / 2.0f, 150, 0xffffff, 0xffffff);

		// ���ڂ̕`��
		int continueItem = static_cast<int>(State::CONTINUE);
		messageManager->DrawStringCenter("PauseContinue",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * continueItem,
			m_itemColorTable[continueItem]);

		int optionItem = static_cast<int>(State::OPTION);
		messageManager->DrawStringCenter("PauseOption",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * optionItem,
			m_itemColorTable[optionItem]);

		int stageSelectItem = static_cast<int>(State::STAGE_SELECT);
		messageManager->DrawStringCenter("PauseStageSelect",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * stageSelectItem,
			m_itemColorTable[stageSelectItem]);

		int restartItem = static_cast<int>(State::TITLE);
		messageManager->DrawStringCenter("PauseTitle",
			draw_text_pos_x,
			draw_text_pos_y + text_space_y * restartItem,
			m_itemColorTable[restartItem]);
	}
}