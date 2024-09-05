#include "Warning.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../Sound/SoundManager.h"
#include <string>
#include <DxLib.h>

namespace
{
	// �摜�t�@�C���p�X
	const std::string center_img_file_path = "Data/Image/Warning.png";
	const std::string up_img_file_path =     "Data/Image/WarningUp.png";
	const std::string down_img_file_path =   "Data/Image/WarningDown.png";

	// �x�������̃t�F�[�h�C���E�A�E�g�̑��x
	constexpr float string_fade_speed = 0.07f;

	// �㉺�̉摜�̃X�N���[�����x
	constexpr int scroll_speed = 10;

	// �S�̂̃t�F�[�h�C���E�A�E�g�̑��x
	constexpr float all_fade_speed = 5.0f;
}

// �R���X�g���N�^
Warning::Warning(const int drawFrame) :
	m_isEnd(false),
	m_scroll(0),
	m_drawFrame(drawFrame),
	m_imgAlphaParam(0),
	m_addImgAlphaParam(all_fade_speed),
	m_stringAlphaParam(0),
	m_ImgAlphaParamRange(0, 255)
{
	// �摜�̃��[�h
	m_uiDataTable[static_cast<int>(ID::CENTER)].imgHandle = my::MyLoadGraph(center_img_file_path.c_str());
	m_uiDataTable[static_cast<int>(ID::UP)].imgHandle = my::MyLoadGraph(up_img_file_path.c_str());
	m_uiDataTable[static_cast<int>(ID::DOWN)].imgHandle = my::MyLoadGraph(down_img_file_path.c_str());

	// �摜�̃T�C�Y���擾
	for (auto& data : m_uiDataTable)
	{
		GetGraphSize(data.imgHandle, &data.imgWidth, &data.imgHeight);
	}

	// �摜�̍��W��ݒ�
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_uiDataTable[static_cast<int>(ID::CENTER)].pos.x = screenSize.width / 2.0f;
	m_uiDataTable[static_cast<int>(ID::CENTER)].pos.y = screenSize.height / 2.0f;
	m_uiDataTable[static_cast<int>(ID::UP)].pos.y = (screenSize.height / 2.0f) - 300;
	m_uiDataTable[static_cast<int>(ID::DOWN)].pos.y = (screenSize.height / 2.0f) + 300;

	// �摜�̃X�N���[��������ݒ�
	m_uiDataTable[static_cast<int>(ID::UP)].scrollDirection = 1;
	m_uiDataTable[static_cast<int>(ID::DOWN)].scrollDirection = -1;
	m_uiDataTable[static_cast<int>(ID::CENTER)].scrollDirection = 0;

	// �ʏ��BGM�̃t�@�C�����̎擾
	const auto& soundManager = Sound::Manager::GetInstance();
	auto fileName = soundManager->GetPlayBGMFileName();

	// �Đ�����BGM����������
	if (fileName != "")
	{
		// �ʏ��BGM�̃t�F�[�h�A�E�g�̐ݒ�
		soundManager->SetFadeSound(fileName, 120, soundManager->GetSoundVolume(fileName), 0);
	}

	// �x����SE�̍Đ�
	soundManager->PlaySE("Warning");

	// �x����SE�̃t�F�[�h�C���̐ݒ�
	soundManager->SetFadeSound("Warning", 20, 0, 255);
}

// �f�X�g���N�^
Warning::~Warning()
{
	// �摜�̃A�����[�h
	for (auto& data : m_uiDataTable)
	{
		DeleteGraph(data.imgHandle);
	}
}

// �X�V
void Warning::Update()
{
	// �`�掞�Ԃ��I��������
	if (m_drawFrame-- <= 0 && !m_isEnd)
	{
		// �I���t���O�𗧂Ă�
		m_isEnd = true;

		// �x����SE�̃t�F�[�h�A�E�g�̐ݒ�
		const auto& soundManager = Sound::Manager::GetInstance();
		soundManager->SetFadeSound("Warning", 30, soundManager->GetSoundVolume("Warning"), 0);

		// �x���摜�̃A���t�@�l�����炷
		m_addImgAlphaParam *= -1;
	}

	// �X�N���[��
	m_scroll += scroll_speed;

	// �x�������̃t�F�[�h
	if (m_isEnd)
	{
		// �t�F�[�h�A�E�g
		m_stringAlphaParam -= all_fade_speed;
	}
	else
	{
		// �t�F�[�h�C���E�A�E�g���J��Ԃ�
		m_stringAlphaParam = (0.5f * sinf(m_drawFrame * string_fade_speed) + 0.5f) * 255.0f;
	}

	// �x���摜�̃A���t�@�l���X�V
	m_imgAlphaParam += m_addImgAlphaParam;
	m_imgAlphaParam = m_ImgAlphaParamRange.Clamp(m_imgAlphaParam);
}

// �`��
void Warning::Draw()
{
	for (auto& data : m_uiDataTable)
	{
		// �X�N���[���̊������v�Z
		int scroll = (m_scroll * data.scrollDirection) % data.imgWidth;

		// �X�N���[������`��
		if (data.scrollDirection != 0)
		{
			// �摜�̃A���t�@�l��ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_imgAlphaParam);

			// �X�N���[������̂�2���`��
			DrawRotaGraph((data.imgWidth * 0.5f) + scroll, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);
			DrawRotaGraph((data.imgWidth * (1 * -data.scrollDirection + 0.5f)) + scroll, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);

			// ���ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// �X�N���[���Ȃ��`��
		else
		{
			// �摜�̃A���t�@�l��ݒ�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_imgAlphaParam);

			// �摜�̕`��
			DrawRotaGraph(data.pos.x, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);

			// ���ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	// �x�������̕`��
	auto& screenSize = Application::GetInstance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_stringAlphaParam);
	String::MessageManager::GetInstance()->DrawStringCenter("WarningUI", screenSize.width / 2.0f, screenSize.height / 2.0f, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// �I�����Ă��邩�ǂ���
bool Warning::IsEnd() const
{
	return (m_isEnd) && (m_imgAlphaParam <= 0);
}
