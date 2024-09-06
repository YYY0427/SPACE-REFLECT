#include "LoadScene.h"
#include "../MyDebug/DebugText.h"
#include "../Input/InputManager.h"
#include <DxLib.h>

namespace
{
	// ���[�h��ʂ��\�������Œ᎞��
	// ���[�h��ʂ���u�����\�������̂�h������
	constexpr int lowest_time_load_scene = 120;
}

namespace Scene
{
	// �R���X�g���N�^
	Load::Load(const std::shared_ptr<Scene::Manager>& pSceneManager, const bool isEternal) :
		Scene::Base(pSceneManager),
		m_isDrawEternal(isEternal),
		m_time(0)
	{
	}

	// �f�X�g���N�^
	Load::~Load()
	{
	}

	// ������
	void Load::Init()
	{
	}

	// �I������
	void Load::End()
	{
	}

	// �X�V
	void Load::Update()
	{
		// ���݂̃V�[������\��
		Debug::Text::AddLog("LoadScene");

		// �i���\���łȂ��ꍇ
		if (!m_isDrawEternal)
		{
			// �o�ߎ��Ԃ�\��
			Debug::Text::AddLog("Time", { m_time });

			// �񓯊���ǂݍ��݂��s���Ă��Ȃ��A
			// ���[�h��ʂ��\�������Œ᎞�Ԃ��߂�����
			if (GetASyncLoadNum() <= 0 &&
				m_time++ >= lowest_time_load_scene)
			{
				// ���[�h��ʂ��폜���邱�Ƃ�ʒm
				m_pSceneManager->SetLoadSceneFlag(false);

				// ���[�h��ʂ��폜
				m_pSceneManager->PopScene();
			}
		}
		// �i���\���̏ꍇ
		else
		{
			// �߂�{�^���������ꂽ��
			if (Input::Manager::IsTriggered(Input::Type::BACK))
			{
				// ���[�h��ʂ��폜
				m_pSceneManager->PopScene();
			}
		}
	}

	// �`��
	void Load::Draw()
	{
		DrawBox(0, 0, 1920, 1080, 0x000000, true);
	}
}
