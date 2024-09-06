#pragma once
#include <memory>
#include "../Transitor/TransitorBase.h"
#include "SceneManager.h"
#include "../Transitor/Fade.h"

namespace Scene
{
	// �v���g�^�C�v�錾
	class Manager;

	/// <summary>
	/// �V�[���̊��N���X
	/// </summary>
	class Base
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W���̃C���X�^���X</param>
		Base(const std::shared_ptr<Scene::Manager>& pSceneManager) :
			m_pSceneManager(pSceneManager)
		{
		}

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~Base() {}

		/// <summary>
		/// ������
		/// �R���X�g���N�^�ł͍D���ȏꏊ�ŏ��������s���Ȃ����ߍ쐬
		/// </summary>
		virtual void Init() = 0;

		/// <summary>
		/// �I������
		/// �f�X�g���N�^�ł͍D���ȏꏊ�ŏI���������s���Ȃ����ߍ쐬
		/// </summary>
		virtual void End() = 0;

		/// <summary>
		/// �X�V
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// �`��
		/// </summary>
		virtual void Draw() = 0;

	protected:
		// �V�[���̃}�l�[�W���[
		std::shared_ptr<Scene::Manager> m_pSceneManager;

		// ��ʐ؂�ւ����o�̃C���X�^���X
		std::unique_ptr<TransitorBase> m_pTransitor;

		// �t�F�[�h
		std::unique_ptr<Fade> m_pFade;
	};
}