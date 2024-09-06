#pragma once
#include "SceneBase.h"
#include <string>
#include <vector>

// �v���g�^�C�v�錾
class Warning;
class ResultWindow;

namespace Scene
{
	/// <summary>
	/// �e�X�g�p�V�[��
	/// �F�X�����Ƃ��Ɏg��
	/// </summary>
	class TestScene final : public Scene::Base
	{
	public:
		/// </summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
		TestScene(const std::shared_ptr<Scene::Manager>& pSceneManager);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~TestScene();

		/// <summary>
		/// ������
		/// </summary>
		void Init() override final;

		/// <summary>
		/// �I������
		/// </summary>
		void End() override final;

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override final;

		/// <summary>
		/// �`��
		/// </summary>
		void Draw() override final;

	private:
		std::unique_ptr<Warning> m_pWarning;
		std::unique_ptr<ResultWindow> m_pResultWindow;
		std::vector<std::vector<std::string>> m_fileNames;
	};
}