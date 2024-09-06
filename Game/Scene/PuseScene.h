#pragma once
#include "SceneBase.h"
#include "StageSelectScene.h"

namespace Scene
{
	/// <summary>
	/// �|�[�Y�V�[��
	/// </summary>
	class Puse final : public Scene::Base
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W��</param>
		Puse(const std::shared_ptr<Scene::Manager>& pSceneManager, const Stage stage);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Puse();

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

		// �|�[�Y�̏��
		enum class State
		{
			CONTINUE,		// ������
			OPTION,			// �I�v�V����
			STAGE_SELECT,	// �X�e�[�W�Z���N�g
			TITLE,			// �^�C�g��
			NUM				// ��Ԑ�
		};

	private:
		// ���݂̏��
		int m_currentSelectItem;

		// ���ڂ̕`��F�f�[�^
		std::vector<int> m_itemColorTable;

		// ���݂̃X�e�[�W
		Stage m_stage;

		// ���U�C�N�����p�X�N���[��
		int m_gaussScreen;
	};
}