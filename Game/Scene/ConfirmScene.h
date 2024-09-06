#pragma once
#include "SceneBase.h"
#include "PuseScene.h"

namespace Scene
{
	/// <summary>
	/// �m�F���
	/// </summary>
	class Confirm final : public Scene::Base
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W��</param>
		/// <param name="state">�|�[�Y�V�[���̏��</param>
		Confirm(const std::shared_ptr<Scene::Manager>& pSceneManager, const Puse::State state);

		/// <summary>
		/// �f�X�g���N�^ko
		/// </summary>
		~Confirm();

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
		// �m�F�̏��
		enum class State
		{
			NO,		// ������
			YES,	// �͂�
			NUM		// ��Ԑ�
		};

	private:
		// ���ڂ̕`��F�f�[�^
		std::vector<unsigned int> m_itemColorTable;

		// ���݂̑I������
		int m_currentSelectItem;

		// �ǂ��ɖ߂�̂����m�F����|�[�Y�V�[���̏��
		Puse::State m_state;

		// ���U�C�N�����p�X�N���[��
		int m_gaussScreen;
	};
}