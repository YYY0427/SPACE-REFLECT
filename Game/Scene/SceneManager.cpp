#include "SceneManager.h"
#include "SceneBase.h"
#include "LoadScene.h"
#include "../Application.h"

namespace
{
	// FPS�̕\�����X�V����Ԋu(�t���[����)
	constexpr int fps_update_interval = 10;
}

namespace Scene
{
	// �R���X�g���N�^
	Manager::Manager() :
		m_updateTime(0),
		m_drawTime(0),
		m_prevFps(0.0f),
		m_isLoadScene(false)
	{
	}

	// �f�X�g���N�^
	Manager::~Manager()
	{
	}

	// �X�V
	void Manager::Update()
	{
		// �~���b�P�ʂ̐��x�����J�E���^�̌��݂̒l�𓾂�
		LONGLONG start = GetNowHiPerformanceCount();

		// ���X�g������ۂ������牽�����Ȃ�
		if (!m_pSceneTable.empty())
		{
			// �擪�̂ݎ��s
			m_pSceneTable.front()->Update();
		}

		// �X�V���Ԃ��v��
		m_updateTime = GetNowHiPerformanceCount() - start;
	}

	// �`��
	void Manager::Draw()
	{
		// �~���b�P�ʂ̐��x�����J�E���^�̌��݂̒l�𓾂�
		LONGLONG start = GetNowHiPerformanceCount();

		// �t���ɕ`��(�Ō�ɐς񂾂��̂��ŏ��ɕ`�悳���)
		for (auto rit = m_pSceneTable.rbegin(); rit != m_pSceneTable.rend(); rit++)
		{
			// �`��
			rit->get()->Draw();
		}

		// �`�掞�Ԃ��v��
		m_drawTime = GetNowHiPerformanceCount() - start;

		// FPS�̕`��
		DrawFps();

		// �X�V���Ԃ̕`��
		DrawUpdateTime();

		// �`�掞�Ԃ̕`��
		DrwaDrawTime();
	}

	// �V�[���̐؂�ւ�
	void Manager::ChangeScene(const std::shared_ptr<Scene::Base>& nextScene)
	{
		// ���X�g������ۂ����������ւ���̂ł͂Ȃ�
		if (m_pSceneTable.empty())
		{
			// �V�[���̒ǉ�
			PushScene(nextScene);
		}
		else
		{
			// �V�[���̏I������
			m_pSceneTable.front()->End();

			// ���ɂP�ȏ゠������擪�̂��̂����ւ���
			m_pSceneTable.front() = nextScene;

			// �V�[���̏�����
			m_pSceneTable.front()->Init();

			// �񓯊��ǂݍ��݂̃`�F�b�N
			CheckAsyncLoad();
		}
	}

	// �V�[���̐؂�ւ��ƑO�̃V�[�����폜
	void Manager::ChangeAndClearScene(const std::shared_ptr<Scene::Base>& nextScene)
	{
		// �V�[���̑S�폜
		ClearScene();

		// �V�[���̐؂�ւ�
		ChangeScene(nextScene);
	}

	// �񓯊��ǂݍ��݂̃`�F�b�N 
	void Manager::CheckAsyncLoad()
	{
		// �񓯊��ǂݍ��ݒ��̐����擾
		if (GetASyncLoadNum() > 0 && !m_isLoadScene)
		{
			// �t���O�𗧂Ă�
			m_isLoadScene = true;

			// �񓯊��ǂݍ��݂��������ꍇ���[�h��ʂɔ��
			PushScene(std::make_shared<Scene::Load>(shared_from_this()));
		}
	}

	// ���݂̃V�[���̏�ɃV�[����ς�(�|�[�Y)
	void Manager::PushScene(const std::shared_ptr<Scene::Base>& scene)
	{
		// �V�[���̒ǉ�
		m_pSceneTable.push_front(scene);

		// �V�[���̏�����
		m_pSceneTable.front()->Init();

		// �񓯊��ǂݍ��݂̃`�F�b�N
		CheckAsyncLoad();
	}

	// ��ԏ�̃V�[�����폜
	void Manager::PopScene()
	{
		// �V�[���̏I������
		m_pSceneTable.front()->End();

		// �V�[���̍폜
		m_pSceneTable.pop_front();
	}

	// �V�[���̑S�폜
	void Manager::ClearScene()
	{
		// �V�[���̏I������
		for (auto& scene : m_pSceneTable)
		{
			scene->End();
		}

		// �V�[���̑S�폜
		m_pSceneTable.clear();
	}

	// FPS�̕`��
	void Manager::DrawFps()
	{
#ifdef _DEBUG
		// ��ʃT�C�Y�̎擾
		Size size = Application::GetInstance()->GetWindowSize();

		// fps�̎擾
		float nowFps = GetFPS();

		// fps�̍X�V
		static int count = 0;
		if (count++ % fps_update_interval == 0)
		{
			m_prevFps = nowFps;
		}

		// fps��`��
		DrawFormatString(300, size.height - 40, 0xffffff, "FPS : %f", m_prevFps);
#endif
	}

	// �X�V���Ԃ̕`��
	void Manager::DrawUpdateTime()
	{
#ifdef _DEBUG
		// ��ʃT�C�Y�̎擾
		const auto& size = Application::GetInstance()->GetWindowSize();

		// fps�̎擾
		float nowFps = GetFPS();

		// 1�t���[���ɂ��������`�掞�Ԃ�\��
		float rate = static_cast<float>(m_updateTime + m_drawTime) / static_cast<long long>(1000000 / nowFps);
		int width = static_cast<int>(size.width * rate);
		DrawBox(16, size.height - 40, 32, size.height - 24, 0xff0000, true);
		DrawString(40, size.height - 40, "DrawTime", 0xffffff);
		DrawBox(0, size.height - 16, width, size.height, 0xff0000, true);
#endif
	}

	// �`�掞�Ԃ̕`��
	void Manager::DrwaDrawTime()
	{
#ifdef _DEBUG
		// ��ʃT�C�Y�̎擾
		Size size = Application::GetInstance()->GetWindowSize();

		// fps�̎擾
		float nowFps = GetFPS();

		// 1�t���[���ɂ��������X�V���Ԃ�\��
		float rate = static_cast<float>(m_updateTime + m_drawTime) / static_cast<long long>(1000000 / nowFps);
		int width = static_cast<int>(size.width * rate);
		rate = static_cast<float>(m_updateTime) / static_cast<long long>(1000000 / nowFps);
		width = static_cast<int>(size.width * rate);
		DrawBox(150, size.height - 40, 166, size.height - 24, 0x0000ff, true);
		DrawString(174, size.height - 40, "UpdateTime", 0xffffff);
		DrawBox(0, size.height - 16, width, size.height, 0x0000ff, true);
#endif
	}
}