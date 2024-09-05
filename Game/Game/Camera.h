#pragma once
#include "../Math/Vector3.h"

/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	Camera(const Math::Vector3& playerPos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// �X�^�[�g���o���̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void UpdateStart(const Math::Vector3& playerPos);

	/// <summary>
	/// �v���C���̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <param name="playerVec">�v���C���[�̃x�N�g��</param>
	void UpdatePlay(const Math::Vector3& playerPos, const Math::Vector3& playerVec);

	/// <summary>
	/// �Q�[���N���A���̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <returns>�J�������o���I��������</returns>
	bool UpdateGameClear(const Math::Vector3& playerPos);

	/// <summary>
	/// �Q�[���I�[�o�[���̍X�V
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void UpdateGameOver(const Math::Vector3& playerPos);


	//// �Z�b�^�[ ////

	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	void SetCamera();		

	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	/// <param name="pos">�J�����̈ʒu</param>
	/// <param name="target">�J�����̒����_</param>
	void SetCamera(const Math::Vector3& pos, const Math::Vector3& target);


	//// �Q�b�^�[ ////

	/// <summary>
	/// �J�����̍��W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const Math::Vector3& GetPos() const;

	/// <summary>
	/// �J�����̒����_�̎擾
	/// </summary>
	/// <returns>�J�����̒����_</returns>
	const Math::Vector3& GetTarget() const;

	/// <summary>
	/// �J�����̐��������̎擾
	/// </summary>
	/// <returns>�J�����̐�������</returns>
	float GetCameraHorizon() const;	

	/// <summary>
	/// �X�^�[�g���o���������t���O�̎擾
	/// </summary>
	/// <returns>�X�^�[�g���o���������t���O</returns>
	bool  IsStartAnimation() const;	

	/// <summary>
	/// �J������X���̈ړ��͈͐����𒴂�����
	/// </summary>
	/// <returns>��������</returns>
	bool IsOverMoveRangeX() const;

	/// <summary>
	/// �J������Y���̈ړ��͈͐����𒴂�����
	/// </summary>
	/// <returns>��������</returns>
	bool IsOverMoveRangeY() const;

	/// <summary>
	/// �J�����̈ړ����x�̎擾
	/// </summary>
	/// <returns>�J�����̈ړ����x</returns>
	float GetCameraMoveSpeed() const;

	/// <summary>
	/// �J�������v���C���[�̃x�N�g���ɍ��킹�ď����ړ����銄�� 
	/// </summary>
	/// <returns></returns>
	float GetCameraMoveRate() const;

private:
	// �J�����̍��W
	Math::Vector3 m_pos;

	// �J�����̒����_
	Math::Vector3 m_target;

	// ����p
	float m_perspective;

	// �J�����̐�������
	float m_cameraVertical;

	// �J�����̐�������
	float m_cameraHorizon;	

	// �t���[��
	int m_hermiteFrame;			// �G���~�l�[�g�⊮�̃t���[��
	int m_gameClearLerpFrame;	// ���[�v���o�̃t���[��
	int m_gameOverLerpFrame;	// ���[�v���o�̃t���[��

	// �X�^�[�g���o���������t���O
	bool m_isStartAnimation;	
};