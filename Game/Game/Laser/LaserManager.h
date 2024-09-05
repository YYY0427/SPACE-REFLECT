#pragma once
#include <memory>
#include <list>
#include <map>
#include "../../Math/Vector3.h"

// �v���g�^�C�v�錾
class LaserBase;
class Player;
class EnemyBase;
class EnemyManager;
class Shield;
class Camera;

// ���[�U�[�̎��
enum class LaserType
{
	NORMAL,		// �ʏ탌�[�U�[
	REFLECT,	// ���˃��[�U�[
	CUBE,		// �L���[�u���[�U�[
	NUM
};

// ���[�U�[�̃f�[�^
struct LaserData
{
	int key;		// ���[�U�[�̃L�[
	LaserType type;	// ���[�U�[�̎��
	std::shared_ptr<LaserBase> pLaser;	// ���[�U�[�̃|�C���^
};

/// <summary>
/// ���[�U�[�̊Ǘ��N���X
/// </summary>
class LaserManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="pCamera">�J�����̃|�C���^</param>
	LaserManager(const std::shared_ptr<Player>& pPlayer, const std::shared_ptr<Camera>& pCamera);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~LaserManager();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���[�U�[�̒ǉ�
	/// </summary>
	/// <param name="type">���[�U�[�̎��</param>
	/// <param name="pEnemy">���ˌ��̃|�C���^</param>
	/// <param name="laserChargeFrame">���[�U�[�̃`���[�W�t���[��</param>
	/// <param name="laserFireFrame">���[�U�[�̔��ˎ��ԃt���[��</param>
	/// <param name="laserSpeed">���[�U�[�̈ړ����x</param>
	/// <param name="isPlayerFollowing">�v���C���[��Ǐ]���邩</param>
	int AddLaser(LaserType type, 
				 const std::shared_ptr<EnemyBase>& pEnemy, 
				 const int laserChargeFrame, 
				 const int laserFireFrame, 
				 const float laserSpeed, 
				 const bool isPlayerFollowing);

	/// <summary>
	/// ���˃��[�U�[�̒ǉ�
	/// </summary>
	/// <param name="pEnemyManager">�G�l�~�[�Ǘ��N���X�̃|�C���^</param>
	/// <param name="pShield">�V�[���h�̃|�C���^</param>
	/// <param name="pLaser">���[�U�[�̃|�C���^</param>
	/// <param name="firePos">���ˈʒu</param>
	/// <returns>���[�U�[�̃L�[</returns>
	int AddReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager,
						const std::shared_ptr<Shield>& pShield,
						const std::shared_ptr<LaserBase>& pLaser, 
					    const Math::Vector3& firePos);

	/// <summary>
	/// �L���[�u���[�U�[�̒ǉ�
	/// </summary>
	/// <param name="firePos">���ˈʒu</param>
	/// <param name="laserSpeed">���[�U�[�̈ړ����x</param>
	/// <returns>���[�U�[�̃L�[</returns>
	int AddCubeLaser(const Math::Vector3& firePos, const float laserSpeed);

	/// <summary>
	/// ���[�U�[�̍폜
	/// </summary>
	/// <param name="key">�폜���郌�[�U�[�̃L�[</param>
	void DeleteLaser(const int key);

	/// <summary>
	/// �S�Ẵ��[�U�[�̍폜
	/// </summary>
	void DeleteAllLaser();


	//// �Q�b�^�[ /////

	/// <summary>
	/// ���[�U�[���X�g�̎擾
	/// </summary>
	/// <returns></returns>
	const std::list<LaserData>& GetLaserList() const;	// ���[�U�[���X�g

	/// <summary>
	/// ���[�U�[�f�[�^�̎擾
	/// </summary>
	/// <param name="key">���[�U�[�̃L�[</param>
	/// <returns>���[�U�[�f�[�^</returns>
	LaserData GetLaserData(const int key) const;

	///// �Z�b�^�[ /////

	/// <summary>
	/// ���[�U�[�̈ʒu��ݒ�
	/// </summary>
	/// <param name="key">���[�U�[�̃L�[</param>
	/// <param name="pos">�ʒu���</param>
	void SetLaserPos(const int key, const Math::Vector3& pos);

private:
	/// <summary>
	/// ���[�U�[�L�[�̍쐬
	/// </summary>
	/// <returns>���[�U�[�L�[</returns>
	int CreateLaserKey();	

private:
	// �|�C���^
	std::shared_ptr<Player> m_pPlayer;   	// �v���C���[�̃|�C���^
	std::shared_ptr<Camera> m_pCamera;		// �J�����̃|�C���^

	// ���[�U�[���X�g
	std::list<LaserData>    m_pLaserList;	
};

