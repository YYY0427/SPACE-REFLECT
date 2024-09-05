#include "TutorialUI.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../Util/Easing.h"
#include "../Sound/SoundManager.h"
#include <DxLib.h>

namespace
{
	// �摜�t�@�C���p�X
	const std::string xbox_stick_l_up = "Data/Image/xbox_stick_l_up.png";
	const std::string xbox_stick_l_down = "Data/Image/xbox_stick_l_down.png";
	const std::string xbox_stick_l_left = "Data/Image/xbox_stick_l_left.png";
	const std::string xbox_stick_l_right = "Data/Image/xbox_stick_l_right.png";
	const std::string xbox_stick_r_up = "Data/Image/xbox_stick_r_up.png";
	const std::string xbox_stick_r_down = "Data/Image/xbox_stick_r_down.png";
	const std::string xbox_stick_r_left = "Data/Image/xbox_stick_r_left.png";
	const std::string xbox_stick_r_right = "Data/Image/xbox_stick_r_right.png";
	const std::string xbox_rb_file_path = "Data/Image/xbox_rb_double.png";
	const std::string xbox_lb_file_path = "Data/Image/xbox_lb_double.png";

	// �A���t�@�l�̑������x
	constexpr int alpha_speed = 5;

	// �`�悷��摜�̐؂�ւ��t���[��
	constexpr int change_image_frame = 120;

	// �`�悷��摜��2���ȏ゠��ꍇ�̉摜�̊Ԋu
	constexpr float image_interval = 50.0f;

	// �摜�ƕ����̊Ԋu
	constexpr float text_interval = 50.0f;

	// �`��t���[��
	constexpr int move_tutorial_frame = 600;	// �ړ�
	constexpr int shield_tutorial_frame = 600;	// �V�[���h
	constexpr int reflect_tutorial_frame = 400;	// ����
	constexpr int cube_tutorial_frame = 400;	// �L���[�u

	// �`���[�g���A���p��UI���E���Ɏc���̂��ǂ����̃t���O
	// �܂��������̂���false
	constexpr bool is_tutorial_ui_hold_test = false;
}

// �R���X�g���N�^
TutorialUI::TutorialUI() :
	m_state(TutorialState::MOVE)
{
	// ������
	m_changeImageFrame = change_image_frame;

	// ��ʃT�C�Y�̎擾
	auto& screenSize = Application::GetInstance().GetWindowSize();

	// �ړ��̃`���[�g���A���̃f�[�^�̐ݒ�
	std::vector<int> imgHandle;
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_up.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_down.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_left.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_l_right.c_str()));
	m_tutorialDataMap[TutorialState::MOVE].imgHandle.push_back(imgHandle);
	m_tutorialDataMap[TutorialState::MOVE].goalPos = { screenSize.width / 2 + 125.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::MOVE].messageId = "TutorialMove";
	m_tutorialDataMap[TutorialState::MOVE].drawFrame = move_tutorial_frame;

	imgHandle.clear();
	
	// �V�[���h�̃`���[�g���A���̃f�[�^�̐ݒ�
	imgHandle.push_back(my::MyLoadGraph(xbox_rb_file_path.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_lb_file_path.c_str()));
	m_tutorialDataMap[TutorialState::SHIELD].imgHandle.push_back(imgHandle);
	imgHandle.clear();
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_up.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_down.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_left.c_str()));
	imgHandle.push_back(my::MyLoadGraph(xbox_stick_r_right.c_str()));
	m_tutorialDataMap[TutorialState::SHIELD].imgHandle.push_back(imgHandle);
	m_tutorialDataMap[TutorialState::SHIELD].goalPos = 
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::SHIELD].messageId = "TutorialShield";
	m_tutorialDataMap[TutorialState::SHIELD].drawFrame = shield_tutorial_frame;
	imgHandle.clear();

	// ���˂̃`���[�g���A���̃f�[�^�̐ݒ�
	m_tutorialDataMap[TutorialState::REFLECT].goalPos = 
	{ screenSize.width / 2.0f - 225.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::REFLECT].messageId = "TutorialReflect";
	m_tutorialDataMap[TutorialState::REFLECT].drawFrame = reflect_tutorial_frame;

	// ����2�̃`���[�g���A���̃f�[�^�̐ݒ�
	m_tutorialDataMap[TutorialState::REFLECT2].goalPos =
	{ screenSize.width / 2.0f - 120.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::REFLECT2].messageId = "TutorialReflect2";
	m_tutorialDataMap[TutorialState::REFLECT2].drawFrame = -1;

	// �L���[�u�̃`���[�g���A���̃f�[�^�̐ݒ�
	m_tutorialDataMap[TutorialState::CUBE].goalPos = 
	{ screenSize.width / 2.0f - 240.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::CUBE].messageId = "TutorialCube";
	m_tutorialDataMap[TutorialState::CUBE].drawFrame = cube_tutorial_frame;

	// �L���[�u2�̃`���[�g���A���̃f�[�^�̐ݒ�
	m_tutorialDataMap[TutorialState::CUBE2].goalPos =
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::CUBE2].messageId = "TutorialCube2";
	m_tutorialDataMap[TutorialState::CUBE2].drawFrame = -1;

	// �v���C�̃`���[�g���A���̃f�[�^�̐ݒ�
	m_tutorialDataMap[TutorialState::PLAY].goalPos =
	{ screenSize.width / 2.0f, screenSize.height - 100.0f };
	m_tutorialDataMap[TutorialState::PLAY].messageId = "TutorialPlay";
	m_tutorialDataMap[TutorialState::PLAY].drawFrame = -1;
}

// �f�X�g���N�^
TutorialUI::~TutorialUI()
{
	// �摜�̉��
	for (auto& img : m_tutorialDataMap)
	{
		for (auto& handle : img.second.imgHandle)
		{
			for (auto& data : handle)
			{
				DeleteGraph(data);
			}
		}
		DeleteGraph(img.second.messageTextImgHandle);
	}
}

// �X�V
void TutorialUI::Update()
{
	for (auto& data : m_tutorialDataMap)
	{
		// ��Ԃ��I�����Ă���ꍇ
		if (data.second.isEnd)
		{
			// �`���[�g���A���p��UI���E���Ɏc���̂��ǂ����̃t���O�������Ă���ꍇ
			if (is_tutorial_ui_hold_test)
			{
				// �g�嗦������
				data.second.scale -= 0.01f;
				data.second.scale = (std::max)(data.second.scale, 0.6f);

				// �ړI�n�̐ݒ�
				auto& screenSize = Application::GetInstance().GetWindowSize();
				Math::Vector2 goalPos = { static_cast<float>(screenSize.width) - 200.0f,
									static_cast<float>(screenSize.height - 30.0f) };

				// �I�����Ă����Ԃ̐����J�E���g
				int count = 0;
				for (auto& data : m_tutorialDataMap)
				{
					if (data.second.isEnd)
					{
						count++;
					}
				}
				// �J�E���g�̐��ɂ���ĖړI�n��ύX
				goalPos.y -= 30.0f * count;

				// �ړI�n�փC�[�W���O���g�p���Ĉړ�
				data.second.easing++;
				data.second.imgPos.x = Easing::EaseOutCubic(data.second.easing, 300, goalPos.x, data.second.imgPos.x);
				data.second.imgPos.y = Easing::EaseOutCubic(data.second.easing, 300, goalPos.y, data.second.imgPos.y);
			}
			else
			{
				// �����x������
				data.second.imgAlpha -= alpha_speed;

				// ���X�ɏ�Ɉړ�
				data.second.imgPos.y -= 1.0f;
			}
		}
		// ��Ԃ��J�n���Ă���ꍇ
		else if (data.first == m_state && data.second.isStart)
		{
			// �t���[�����Z
			if (data.second.drawFrame > -1)
			{
				data.second.drawFrame--;
				data.second.drawFrame = (std::max)(data.second.drawFrame, 0);

				if (data.second.drawFrame <= 0)
				{
					EndState();
				}
			}

			// �����x�𑝉�
			data.second.imgAlpha += alpha_speed;
			data.second.imgAlpha = (std::min)(data.second.imgAlpha, 255);

			// ���X�ɏ�Ɉړ�
			data.second.imgPos.y -= 1.0f;
			data.second.imgPos.y = (std::max)(data.second.imgPos.y, data.second.goalPos.y);

			// ����̃t���[�����Ƃɉ摜��؂�ւ�
			m_changeImageFrame.Update(1);
			if (m_changeImageFrame.IsTimeOut())
			{
				// ������
				m_changeImageFrame.Reset();

				// �摜�̐؂�ւ�
				for (size_t i = 0; i < data.second.imgHandle.size(); i++)
				{
					data.second.imgIndex[i] = (data.second.imgIndex[i] + 1) % data.second.imgHandle[i].size();
				}
			}
		}
	}
}

// �`��
void TutorialUI::Draw()
{
	// �`���[�g���A���p��UI���E���Ɏc���̂��ǂ����̃t���O�������Ă���ꍇ
	if (is_tutorial_ui_hold_test)
	{
		for (auto& data : m_tutorialDataMap)
		{
			// ��Ԃ��X�^�[�g���Ă��Ȃ��ꍇ�͕`�悵�Ȃ�	
			if (!data.second.isStart) return;

			// ��������
			SetDrawScreen(data.second.messageTextImgHandle);
			ClearDrawScreen();
			auto& screenSize = Application::GetInstance().GetWindowSize();
			String::MessageManager::GetInstance()->DrawStringCenter(data.second.messageId,
				screenSize.width / 2.0f, screenSize.height / 2.0f, 0xffffff);
			SetDrawScreen(DX_SCREEN_BACK);

			// �����x�̐ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, data.second.imgAlpha);

			// ���b�Z�[�W�摜�̕`��
			DrawRotaGraph(data.second.imgPos.x + 130.0f, data.second.imgPos.y,
				data.second.scale, 0.0f, data.second.messageTextImgHandle, true);

			// �摜�̕`��
		/*	DrawRotaGraph(data.second.imgPos.x, data.second.imgPos.y,
				data.second.scale, 0.0,
				data.second.imgHandle[data.second.imgIndex], TRUE);*/

			// �����x�����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	else
	{
		// ��Ԃ��X�^�[�g���Ă��Ȃ��ꍇ�͕`�悵�Ȃ�
		if(m_tutorialDataMap[m_state].isStart == false) return;

		// �����x�̐ݒ�
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_tutorialDataMap[m_state].imgAlpha);

		// �`��ʒu�̕ۑ�
		Math::Vector2 savePos = m_tutorialDataMap[m_state].imgPos;

		// �摜�̕`��
		for (size_t i = 0; i < m_tutorialDataMap[m_state].imgHandle.size(); i++)
		{
			// �摜�̊Ԋu��ݒ�
			if (i >= 1)
			{
				//	�摜�̃T�C�Y���擾
				float width = 0.0f, height = 0.0f;
				GetGraphSizeF(m_tutorialDataMap[m_state].imgHandle[i - 1][m_tutorialDataMap[m_state].imgIndex[i -1]],
					&width, &height);

				// �摜�̈ʒu�����炷
				m_tutorialDataMap[m_state].imgPos.x += (width + image_interval);
			}

			// �摜�̕`��
			DrawRotaGraph(m_tutorialDataMap[m_state].imgPos.x, m_tutorialDataMap[m_state].imgPos.y,
						  1.0, 0.0,
						  m_tutorialDataMap[m_state].imgHandle[i][m_tutorialDataMap[m_state].imgIndex[i]], TRUE);

			// �摜�Ɖ摜�̊ԂɃe�L�X�g��}��
			if (m_tutorialDataMap[m_state].imgHandle.size() >= 2 && 
				i != m_tutorialDataMap[m_state].imgHandle.size() - 1)
			{
				//	�摜�̃T�C�Y���擾
				float width = 0.0f, height = 0.0f;
				GetGraphSizeF(m_tutorialDataMap[m_state].imgHandle[i][m_tutorialDataMap[m_state].imgIndex[i]],
					&width, &height);

				// �e�L�X�g�̕`��
				String::MessageManager::GetInstance()->DrawStringCenter(
					"TutorialUI+", 
					m_tutorialDataMap[m_state].imgPos.x + ((width + image_interval) / 2.0f),
					m_tutorialDataMap[m_state].imgPos.y, 
					0xffffff);
			}

		}

		// �e�L�X�g�̕`��
		auto messageHeight = String::MessageManager::GetInstance()->GetMessageHeight(m_tutorialDataMap[m_state].messageId);
		String::MessageManager::GetInstance()->DrawString(m_tutorialDataMap[m_state].messageId,
					  m_tutorialDataMap[m_state].imgPos.x + text_interval,
					  m_tutorialDataMap[m_state].imgPos.y - (messageHeight / 2.0f), 0xffffff);
		
		// �`��ʒu�̕���
		m_tutorialDataMap[m_state].imgPos.x = savePos.x;

		// �����x�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

// ��Ԃ̐ݒ�
void TutorialUI::StartState(const TutorialState state)
{
	// ���łɃX�^�[�g���Ă���ꍇ�͉������Ȃ�
	if (m_tutorialDataMap[state].isStart) return;

	// ���݂̏�Ԃ��I�����Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_tutorialDataMap[m_state].imgAlpha > 0) return;
	if (m_tutorialDataMap[m_state].isStart && !m_tutorialDataMap[m_state].isEnd) return;

	Sound::Manager::GetInstance()->PlaySE("TutorialUI");

	// ��Ԃ̐ݒ�
	m_state = state;
	m_tutorialDataMap[m_state].isStart = true;
	m_tutorialDataMap[m_state].isEnd = false;

	// �ʒu�̐ݒ�
	m_tutorialDataMap[m_state].imgPos = 
		{ m_tutorialDataMap[m_state].goalPos.x, m_tutorialDataMap[m_state].goalPos.y + 20.0f };

	// �摜�̃C���f�b�N�X�̏�����
	m_tutorialDataMap[m_state].imgIndex.resize(m_tutorialDataMap[m_state].imgHandle.size(), 0);

	// �`���[�g���A���p��UI���E���Ɏc���̂��ǂ����̃t���O�������Ă���ꍇ
	if (is_tutorial_ui_hold_test)
	{
		// ���b�Z�[�W�摜�p�̃X�N���[���̍쐬
		const auto& messageManager = String::MessageManager::GetInstance();
		m_tutorialDataMap[m_state].messageTextImgHandle =
			MakeScreen(messageManager->GetMessageWidth(m_tutorialDataMap[m_state].messageId),
				messageManager->GetMessageHeight(m_tutorialDataMap[m_state].messageId));
	}
}

// ���݂̏�Ԃ��I��
void TutorialUI::EndState()
{
	m_tutorialDataMap[m_state].isEnd = true;
}

// ��Ԃ̎擾
TutorialState TutorialUI::GetState() const
{
	return m_state;
}

// ��Ԃ��I�����Ă��邩
bool TutorialUI::IsEndState(const TutorialState state)
{
	return m_tutorialDataMap[state].isEnd;
}