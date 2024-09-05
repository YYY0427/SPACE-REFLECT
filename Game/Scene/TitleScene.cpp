#include "TitleScene.h"
#include "DebugScene.h"
#include "StageSelectScene.h"
#include "SceneManager.h"
#include "../Game/Camera.h"
#include "../Game/SkyDome.h"
#include "../Transitor/Fade.h"
#include "../Util/InputState.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../Sound/SoundManager.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include <DxLib.h>

namespace
{
	// �\������e�L�X�g�̑S�̂̈ʒu
	const int draw_text_pos_y = Application::GetInstance()->GetWindowSize().height / 2 + 200;

	// �e�L�X�g�̕�����
	constexpr int text_space_y = 50;

	// �����̓_�ł̑���
	constexpr int string_alpha_add = 2;

	// ���[�U�[�̈ʒu�͈�
	constexpr int laser_height = 25;
}

// �R���X�g���N�^
TitleScene::TitleScene(SceneManager& manager) :
	SceneBase(manager),
	m_currentSelectSceneItem(0),
	m_alpha(200),
	m_frame(0),
	m_isInput(false),
	m_alphaAdd(string_alpha_add),
	m_gaussHandle(-1)
{
}

// �f�X�g���N�^
TitleScene::~TitleScene()
{
}

// ������
void TitleScene::Init()
{
	// �t�F�[�h�C���̉��o
	m_pFade = std::make_unique<Fade>();
	m_pFade->StartFadeIn(0);

	// �C���X�^���X�̍쐬
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->SetCamera(Math::Vector3(0, 0, 0), Math::Vector3(0, 0, 1));
	m_pSkyDome = std::make_shared<SkyDome>(Math::Vector3(0, 0, 0));

	// �^�C�g����ʂ�BGM���Đ�
	Sound::Manager::GetInstance()->PlayBGM("TitleBgm");
	Sound::Manager::GetInstance()->SetFadeSound("TitleBgm", 120, 0, 255);

	// ���[�U�[�̔z�u
	SetLaser({ 0, 210 }, { 1280, 210 }, 0xff0000);
	SetLaser({ 0, 390 }, { 1280, 390 }, 0x0000ff);

	// �K�E�X�n���h���̍쐬
	auto& size = Application::GetInstance()->GetWindowSize();
	m_gaussHandle = MakeScreen(size.width, size.height);
}

// �I������
void TitleScene::End()
{
}

// �X�V
void TitleScene::Update()
{
	// �ǂ����̃{�^���������ꂽ�玟�̃V�[���ɑJ��
	if (InputState::IsTriggered(InputType::ANY_BUTTON))
	{
		// �t�F�[�h�A�E�g�̉��o�̊J�n
		m_pFade->StartFadeOut(255);

		// PRESS ANY BUTTON�̓_�ł��~�߂�
		m_alphaAdd = 0;
		m_alpha = 255;

		if (!m_isInput)
		{
			// �^�C�g����ʂ�BGM���t�F�[�h�A�E�g
			const auto& soundManager = Sound::Manager::GetInstance();
			soundManager->SetFadeSound("TitleBgm", 40, soundManager->GetSoundVolume("TitleBgm"), 0);

			// �^�C�g����ʂ̌��艹���Đ�
			soundManager->PlaySE("Enter");
		}

		// �t���O�𗧂Ă�
		m_isInput = true;
	}

	// �X�J�C�h�[���̍X�V
	m_pSkyDome->Update(Math::Vector3(0, 0, 0));

	// ���[�U�[�̍X�V
	for (auto& laser : m_laserData)
	{
		laser.alpha = GetRand(100);
		laser.thickness = GetRand(9) + 1;
		int rand = GetRand(laser_height * 2) - laser_height;
		laser.pos1.y = laser.startPos.x + rand;
		laser.pos2.y = laser.startPos.y + rand;
	}

	// PRESS ANY BUTTON�̓_�ŏ���
	// 255��50���s�����藈���肷��
	m_alpha += m_alphaAdd;
	if (m_alpha >= 255 || m_alpha <= 50)
	{
		m_alphaAdd *= -1;
	}

	// �{�^����������ăt�F�[�h�A�E�g���I�������玟�̃V�[���ɑJ��
	if (!m_pFade->IsFading() && m_isInput)
	{
		switch (static_cast<SceneItem>(m_currentSelectSceneItem))
		{
		case SceneItem::GAME:
			m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
			return;
		}
	}

	// �t�F�[�h�̍X�V
	m_pFade->Update();
}

void TitleScene::Draw()
{
	// ��ʂ��N���A
	ClearDrawScreen();
	
	// �C���X�^���X�擾
	auto& size = Application::GetInstance()->GetWindowSize();	
	const auto& messageManager = String::MessageManager::GetInstance();

	// �`��X�N���[����؂�ւ�
	SetDrawScreen(m_gaussHandle);

	// �ؑ֌�̕`��X�N���[�����N���A
	ClearDrawScreen();

	// ���[�U�[�̕`��
	for (auto& laser : m_laserData)
	{
		// ���[�U�[�̔����Ƃ���̕`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
		DrawLine(laser.pos1.x, laser.startPos.x, laser.pos2.x, laser.startPos.y, 0xffffff, 14);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���[�U�[�̊O���̐F�̕`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, laser.alpha);
		DrawLine(laser.pos1.x, laser.pos1.y, laser.pos2.x, laser.pos2.y, laser.color, laser.thickness);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// �^�C�g�����S�̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	messageManager->DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff, 0x000000);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �K�E�X�t�B���^�[�̓K�p
	GraphFilter(m_gaussHandle, DX_GRAPH_FILTER_GAUSS, 32, 1400);

	// �`��X�N���[����߂�
	SetDrawScreen(DX_SCREEN_BACK);

	// ���Z�u�����h�ŃK�E�X�摜�����񂩏d�˂ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	for (int i = 0; i < 3; i++)
	{
		// �������쐬�����K�E�X�摜��`��
		DrawGraph(0, 0, m_gaussHandle, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); 

	// �X�J�C�h�[���̕`��
	m_pSkyDome->Draw();

	// �^�C�g�����S��������x�`��
	messageManager->DrawStringCenter("TitleLogo", size.width / 2, 300, 0xffffff, GetColor(130, 130, 130));

	// ���ڂ̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	messageManager->DrawStringCenter("TitleItemStart", size.width / 2, 
		draw_text_pos_y + text_space_y * static_cast<int>(SceneItem::GAME), 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// �t�F�[�h�̕`��
	m_pFade->DrawFade(true);
}

// ���[�U�[�̕`��
void TitleScene::SetLaser(const Math::Vector2& pos1, const Math::Vector2& pos2, const unsigned int color)
{
	for (int i = 0; i < 10; i++)
	{
		// ���[�U�[�̃f�[�^���쐬
		LaserData data{};
		data.alpha = GetRand(100);
		data.color = color;
		data.thickness = GetRand(9) + 1;
		data.pos1.x = pos1.x;
		data.pos2.x = pos2.x;
		data.startPos = { pos1.y, pos2.y };

		int rand = GetRand(laser_height * 2) - laser_height;
		data.pos1.y = pos1.y + rand;
		data.pos2.y = pos2.y + rand;

		m_laserData.push_back(data);
	}
}
