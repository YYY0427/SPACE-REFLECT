#pragma once
#include "SceneBase.h"

namespace Scene
{
	/// <summary>
	/// ���[�h�V�[��
	/// </summary>
	class Load final : public Scene::Base
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W���̃C���X�^���X</param>
		/// <param name="isEternal">�i���\����(�f�o�b�O�p)</param>
		Load(const std::shared_ptr<Scene::Manager>& pSceneManager, const bool isDrawEternal = false);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Load();

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
		// ���[�h��ʂ��\������Ă��鎞��
		int m_time;

		// ���[�h��ʂ��i���\����
		bool m_isDrawEternal;
	};
}