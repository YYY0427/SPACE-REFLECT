#pragma once
#include <map>
#include <string>
#include "SceneBase.h"

namespace Scene
{
	/// <summary>
	/// �V�[���Ԃ�J�ڂ��邽�߂̃N���X
	/// �f�o�b�O�@�\
	/// </summary>
	class Transporter final : public Scene::Base
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W���[�̃C���X�^���X</param>
		Transporter(const std::shared_ptr<Scene::Manager>& pSceneManager);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Transporter();

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
		// �V�[���̎��
		enum class SceneItem
		{
			TITLE,			// �^�C�g��
			STAGE_SELECT,	// �X�e�[�W�Z���N�g
			PAUSE,			// �|�[�Y
			OPTION,			// �I�v�V����
			TEST,			// �e�X�g
			EXIT,			// �I��
			NUM				// ���ڐ�
		};

	private:
		// �V�[���̎�ނƖ��O��R�Â��Ă���V�[���̃e�[�u��
		// first : �V�[���̎��
		// second : �V�[���̖��O
		std::map<SceneItem, std::string> m_pSceneTable;

		// �I�𒆂̃C���f�b�N�X
		int m_selectIndex;
	};
}