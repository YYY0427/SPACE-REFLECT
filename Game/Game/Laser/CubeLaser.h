#pragma once
#include "LaserBase.h"

// �v���g�^�C�v�錾
class Player;
class Camera;

/// <summary>
/// �L���[�u���[�U�[
/// </summary>
class CubeLaser : public LaserBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="firePos">���ˈʒu</param>
	/// <param name="laserSpeed">���[�U�[�̑���</param>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="pCamera">�J�����̃|�C���^</param>
	CubeLaser(const Math::Vector3& firePos,
			  const float laserSpeed, 
			  const std::shared_ptr<Player>& pPlayer,
			  const std::shared_ptr<Camera>& pCamera);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CubeLaser();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override final;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override final;

private:
	// �|�C���^
	std::shared_ptr<Player> m_pPlayer;	// �v���C���[
	std::shared_ptr<Camera> m_pCamera;	// �J����

	// �ړ��x�N�g��
	Math::Vector3 m_moveVec;

	// 1�t���[���ɉ�]�����
	Math::Vector3 m_deltaRot;

	// �ő�X�P�[��
	Math::Vector3 m_maxScale;

	// �K�E�X�p�X�N���[��
	int m_gaussScreen;	
};