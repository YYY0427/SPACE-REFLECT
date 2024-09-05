#pragma once
#include "LaserBase.h"
#include "../Shield.h"

// �v���g�^�C�v�錾
class EnemyManager;

/// <summary>
/// ���˃��[�U�[
/// </summary>
class ReflectLaser : public LaserBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pEnemyManager">pEnemymanager�̃|�C���^</param>
	/// <param name="pShield">pShield�̃|�C���^</param>
	/// <param name="pLaser">pLaser�̃|�C���^</param>
	/// <param name="firePos">���ˈʒu</param>
	ReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
				 const std::shared_ptr<Shield>& pShield, 
				 const std::shared_ptr<LaserBase>& pLaser, 
				 const Math::Vector3& firePos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ReflectLaser();

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
	std::shared_ptr<Shield>       m_pShield;		// �V�[���h�̃|�C���^
	std::shared_ptr<LaserBase>    m_pLaser;			// ���[�U�[�̃|�C���^
	std::shared_ptr<EnemyManager> m_pEnemyManager;	// �G�̃|�C���^

	// ���[�U�[�G�t�F�N�g�̃n���h��
	int m_laserEffectHandle;	

	// ���[�U�[�̌����Ă�������̍��W
	Math::Vector3 m_directionPos;
};
