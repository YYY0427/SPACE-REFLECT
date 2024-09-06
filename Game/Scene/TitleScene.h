#pragma once
#include "SceneBase.h"
#include "../Math/Vector2.h"
#include <vector>

// �v���g�^�C�v�錾
class Camera;
class SkyDome;

namespace Scene
{
	/// <summary>
	/// �^�C�g���V�[��
	/// </summary>
	class Title final : public Scene::Base
	{
	public:
		/// </summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
		Title(const std::shared_ptr<Scene::Manager>& pSceneManager);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Title();

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

		/// <summary>
		/// ���[�U�[�̐ݒ�
		/// </summary>
		/// <param name="pos1">����̈ʒu</param>
		/// <param name="pos2">�E���̈ʒu</param>
		/// <param name="color">���[�U�[�̐F</param>
		void SetLaser(const Math::Vector2& pos1, const Math::Vector2& pos2, const unsigned int color);

	private:
		// ���������ׂ�V�[������
		enum class SceneItem
		{
			GAME,	// �Q�[��
			NUM		// ���ڐ�
		};

	private:
		// ���[�U�[�̃f�[�^
		struct LaserData
		{
			Math::Vector2 pos1;
			Math::Vector2 pos2;
			Math::Vector2 startPos;
			float thickness;
			int alpha;
			unsigned int color;
		};

	private:
		// �|�C���^
		std::shared_ptr<Camera> m_pCamera;
		std::shared_ptr<SkyDome> m_pSkyDome;

		// ���[�U�[�̃f�[�^
		std::vector<LaserData> m_laserData;

		// ���ݑI�����Ă���V�[������
		int m_currentSelectSceneItem;

		// �A���t�@�l
		int m_alpha;

		// �A���t�@�l�̉��Z�l
		int m_alphaAdd;

		// �t���[��
		int m_frame;

		// �K�E�X�n���h��
		int m_gaussHandle;

		// �{�^���������ꂽ���ǂ���
		bool m_isInput;
	};
}