#include "OptionScene.h"
#include "SceneManager.h"
#include "StageSelectScene.h"
#include "../Util/DrawFunctions.h"
#include "../Input/InputManager.h"
#include "../SaveData.h"
#include "../String/MessageManager.h"
#include "../Application.h"
#include "../Transitor/FadeTransitor.h"
#include "../Util/StateMachine.h"
#include "../Sound/SoundManager.h"
#include "DxLib.h"
#include <cassert>
#include <string>
#include <array>

namespace
{
	// �摜�t�@�C���̃p�X
	const std::string sound_icon_img_file_path = "Data/Image/Sound.png";
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb.png";

	// ���U�C�N�̎g�p�s�N�Z����(8, 16, 32 �̉��ꂩ)
	constexpr int gauss_param_pixel = 8;

	// ���U�C�N�̋���(100�Ŗ�1�s�N�Z�����̕�)
	constexpr int gauss_param_power = 600;

	// �I������Ă��Ȃ��Ƃ��̕`��F
	constexpr unsigned int normal_color = 0x444444;

	// �I������Ă���Ƃ��̕`��F
	constexpr unsigned int choose_color = 0xffffff;

	// �\������e�L�X�g�̑S�̂̈ʒu
	constexpr int draw_text_pos_x = 200;
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 - 100;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 64;

	// ���ʂ����i�K�ɕ����邩
	constexpr int volume_division = 5;

	// ���ʃA�C�R���̑I������Ă��Ȃ��Ƃ��̕`��P�x(0 �Â� �` 255 ���邢)
	constexpr int normal_draw_bright = 70;

	// ���ʃA�C�R���ǂ����̊Ԋu
	constexpr int volume_icon_space = 70;
}

namespace Scene
{
	// �R���X�g���N�^
	Option::Option(const std::shared_ptr<Scene::Manager>& pSceneManager, const State state) :
		Base(pSceneManager),
		m_currentSelectItem(0),
		m_soundIconHandle(-1),
		m_lbButtonImgHandle(-1),
		m_rbButtonImgHandle(-1),
		m_gaussScreen(-1),
		m_state(state)
	{
	}

	// �f�X�g���N�^
	Option::~Option()
	{
	}

	// ������
	void Option::Init()
	{
		// �摜�̃��[�h
		m_soundIconHandle = my::MyLoadGraph(sound_icon_img_file_path.c_str());
		m_lbButtonImgHandle = my::MyLoadGraph(xbox_lb_file_path.c_str());
		m_rbButtonImgHandle = my::MyLoadGraph(xbox_rb_file_path.c_str());

		// �X�e�[�g�}�V���̏�����
		m_updateStateMachine.AddState(State::STAGE_SELECT, {}, [this] { UpdateStageSelect(); }, {});
		m_updateStateMachine.AddState(State::PAUSE, {}, [this] { UpdatePause(); }, {});
		m_drawStateMachine.AddState(State::STAGE_SELECT, {}, [this] { DrawStageSelect(); }, {});
		m_drawStateMachine.AddState(State::PAUSE, {}, [this] { DrawPause(); }, {});
		m_updateStateMachine.SetState(m_state);
		m_drawStateMachine.SetState(m_state);

		// ���U�C�N�����p�̃O���t�B�b�N�̍쐬
		const auto& size = Application::GetInstance()->GetWindowSize();
		m_gaussScreen = MakeScreen(size.width, size.height);

		// ���ڂ̕`��F��I������Ă��Ȃ��Ƃ��̐F�ɏ�����
		for (int i = 0; i < static_cast<int>(OptionItem::NUM); i++)
		{
			m_itemColorTable.push_back(normal_color);
		}
	}

	// �I������
	void Option::End()
	{
		// �摜�̃A�����[�h
		DeleteGraph(m_soundIconHandle);
		DeleteGraph(m_lbButtonImgHandle);
		DeleteGraph(m_rbButtonImgHandle);

		// ���U�C�N�����p�̃O���t�B�b�N�̍폜
		DeleteGraph(m_gaussScreen);
	}

	// �X�V
	void Option::Update()
	{
		// �J���[�̏�����
		for (auto& itemColor : m_itemColorTable)
		{
			itemColor = normal_color;
		}

		// �I�������񂷏���
		int itemTotalValue = static_cast<int>(OptionItem::NUM);
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

		// �I������Ă��鍀�ڂ��ǂꂩ
		switch (static_cast<OptionItem>(m_currentSelectItem))
		{
			// �E�B���h�E���[�h�̐ݒ�
		case OptionItem::WINDOW_MODE:
			SaveData::GetInstance().SetWindowMode();
			break;

			// �S�̉��ʂ̒���
		case OptionItem::MASTER_VOLUME:
			SaveData::GetInstance().SetMasterVolume(volume_division);
			break;

			// BGM���ʂ̒���
		case OptionItem::BGM_VOLUME:
			SaveData::GetInstance().SetBgmVolume(volume_division);
			break;

			// SE���ʂ̒���
		case OptionItem::SE_VOLUME:
			SaveData::GetInstance().SetSeVolume(volume_division);
			break;
		default:
			// ���肦�Ȃ��̂Ŏ~�߂�
			assert(false);
		}

		// �X�e�[�g�}�V���̍X�V
		m_updateStateMachine.Update();
	}

	// �X�e�[�W�Z���N�g�̍X�V
	void Option::UpdateStageSelect()
	{
		// ����̃{�^���������ꂽ�Ƃ�
		if (Input::Manager::IsTriggered(Input::Type::LEFT_SHOULDER))
		{
			// �I��
			Sound::Manager::GetInstance()->PlaySE("Select");
			m_pSceneManager->PopScene();
			return;
		}
	}

	// �|�[�Y�̍X�V
	void Option::UpdatePause()
	{
		// �߂�{�^���������ꂽ�Ƃ�
		if (Input::Manager::IsTriggered(Input::Type::BACK) || Input::Manager::IsTriggered(Input::Type::PAUSE))
		{
			// �I��
			m_pSceneManager->PopScene();
			return;
		}
	}

	// �`��
	void Option::Draw()
	{
		// �X�e�[�g�}�V���̍X�V
		m_drawStateMachine.Update();

		// �E�B���h�E�T�C�Y�̎擾
		const auto& size = Application::GetInstance()->GetWindowSize();

		// �C���X�^���X�̎擾
		const auto& messageManager = String::MessageManager::GetInstance();

		// ���ڂ̕`��
		int windowMode = static_cast<int>(OptionItem::WINDOW_MODE);
		messageManager->DrawString("OptionItemWindowMode", draw_text_pos_x,
			draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]);

		int masterVolume = static_cast<int>(OptionItem::MASTER_VOLUME);
		messageManager->DrawString("OptionItemMasterVolume", draw_text_pos_x,
			draw_text_pos_y + text_space_y * masterVolume, m_itemColorTable[masterVolume]);

		int bgmVolume = static_cast<int>(OptionItem::BGM_VOLUME);
		messageManager->DrawString("OptionItemBgmVolume", draw_text_pos_x,
			draw_text_pos_y + text_space_y * bgmVolume, m_itemColorTable[bgmVolume]);

		int seVolume = static_cast<int>(OptionItem::SE_VOLUME);
		messageManager->DrawString("OptionItemSeVolume", draw_text_pos_x,
			draw_text_pos_y + text_space_y * seVolume, m_itemColorTable[seVolume]);

		// �E�B���h�E���[�h�̏�Ԃ̕\��
		auto& saveData = SaveData::GetInstance();
		(saveData.GetSaveData().windowMode) ?
			messageManager->DrawStringCenter("OptionItemWindowModeOff", size.width / 2 + 120,
				draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]) :
			messageManager->DrawStringCenter("OptionItemWindowModeOn", size.width / 2 + 120,
				draw_text_pos_y + text_space_y * windowMode, m_itemColorTable[windowMode]);

		// ���ʂ̕\��
		int masterVolumeValue = saveData.GetSaveData().masterVolume;
		int bgmVolumeValue = saveData.GetSaveData().bgmVolume;
		int seVolumeValue = saveData.GetSaveData().seVolume;

		// ���ʂ̔z��
		std::array<int, 3> volumeValue =
		{ masterVolumeValue, bgmVolumeValue, seVolumeValue };

		// ���ʂ̍���
		int volumeItem = static_cast<int>(OptionItem::MASTER_VOLUME);

		// ���ʂ̕`��
		for (int i = 0; i < volumeValue.size(); i++)
		{
			for (int j = 0; j < volumeValue[i]; j++)
			{
				// �`��P�x��ݒ�
				// �Â��`��
				SetDrawBright(normal_draw_bright, normal_draw_bright, normal_draw_bright);

				// �I������Ă��鍀�ڂ̏ꍇ�͕`��P�x��߂�
				if (m_currentSelectItem == volumeItem) SetDrawBright(255, 255, 255);

				// ���ʂ̊Ԋu���v�Z
				int textSpaceX = j * volume_icon_space;

				// ���ʂ̃A�C�R����`��
				DrawRotaGraph(620 + textSpaceX,
					draw_text_pos_y + text_space_y * volumeItem + 10, 0.2, 0.0, m_soundIconHandle, true);
			}
			volumeItem++;
		}
		// �`��P�x�����Ƃɖ߂�
		SetDrawBright(255, 255, 255);
	}

	// �X�e�[�W�Z���N�g�̕`��
	void Option::DrawStageSelect()
	{
		// �w�i�̕`��
		auto& screenSize = Application::GetInstance()->GetWindowSize();
		DrawBox(0, 0, screenSize.width, screenSize.height, 0x222222, true);

		// �X�e�[�W�Z���N�g�^�C�g���̕`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawRoundRectAA((screenSize.width / 2.0f) - 325, 50, (screenSize.width / 2.0f) - 50, 110, 5, 5, 8, 0xffffff, true);
		String::MessageManager::GetInstance()->DrawStringCenter("MissionTitle", (screenSize.width / 2.0f) - 187, 80, 0x000000);

		// RB�{�^���̕`��
		DrawRotaGraph((screenSize.width / 2.0f) + 375, 95, 1.0f, 0.0f, m_rbButtonImgHandle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �I�v�V�����^�C�g���̕`��
		DrawRoundRectAA((screenSize.width / 2.0f) + 325, 50, (screenSize.width / 2.0f) + 50, 110, 5, 5, 8, 0xffffff, true);
		String::MessageManager::GetInstance()->DrawStringCenter("OptionTitle", (screenSize.width / 2.0f) + 187, 80, 0x000000);

		// LB�{�^���̕`��
		DrawRotaGraph((screenSize.width / 2.0f) - 375, 95, 1.0f, 0.0f, m_lbButtonImgHandle, true);

		// ���̕`��
		DrawLineAA(0 + 100, 120, screenSize.width - 100, 120, 0xffffff, 3.0f);
		DrawLineAA(450, 170, 450, screenSize.height - 100, 0xffffff, 3.0f);
	}

	// �|�[�Y�̕`��
	void Option::DrawPause()
	{
		// ���U�C�N����
		const auto& size = Application::GetInstance()->GetWindowSize();
		GetDrawScreenGraph(0, 0, size.width, size.height, m_gaussScreen);
		GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_param_pixel, gauss_param_power);
		DrawGraph(0, 0, m_gaussScreen, true);

		// ��ʂ��Â�����
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 0, size.width, size.height, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �V�[���^�C�g���̕`��
		String::MessageManager::GetInstance()->DrawStringCenter("PauseOptionTitle", size.width / 2, 100, 0xffffff);
	}

}