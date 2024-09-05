#pragma once
#include <memory>
#include <list>
#include <vector>
#include <string>
#include <map>
#include "../../Math/Vector3.h"
#include "../Laser/LaserManager.h"
#include "../../StateMachine.h"
#include "../../Effect/ScreenShaker.h"

// �v���g�^�C�v�錾
class EnemyBase;
class Player;
class Warning;
namespace Effect { class ScreenShaker; }

// �G�̎��
enum class EnemyType
{
	// �ʏ�̓G
	MOSQUITO,		// ��
};

// �{�X�G�̎��
enum class BossEnemyType
{
	// �{�X�G
	MOSQUITO,		// ��
	MATRIX,			// �}�g���b�N�X
	NONE,			// �Ȃ�
};

// �G�̍s���̃f�[�^
struct EnemyActionData
{
	Vector3 goalPos;		// �ړI�n
	int idleFrame;			// �ړI�n�ɓ��B���Ă��玟�̖ړI�n�Ɍ������܂ł̑ҋ@�t���[��
	bool isLaser;			// �ړI�n�ɓ��B�����烌�[�U�[������
	LaserType laserType;	// ���[�U�[�����ꍇ�A�ǂ̃��[�U�[������
	float laserChargeFrame;	// ���[�U�[�����ꍇ�A���[�U�[�����܂ł̃`���[�W�t���[��
	int laserIdleFrame;		// ���[�U�[�����ꍇ�A�ړI�n�ɓ��B���Ă��烌�[�U�[�����܂ł̑ҋ@�t���[��
	float laserSpeed;		// ���[�U�[�����ꍇ�A���[�U�[�̈ړ����x
	int laserFireFrame;		// ���[�U�[�����t���[�����˂������邩
	bool isPlayerFollowing;	// ���[�U�[�����ꍇ�A���[�U�[���v���C���[��Ǐ]���邩�ǂ���
};

// �G�̃f�[�^
struct EnemyData
{
	Vector3 pos;	// �G�̏����ʒu
	EnemyType type;	// �G�̎��
	int hp;			// �G�̗̑�
	int attack;		// �G�̍U����
	float speed;	// �G�̈ړ����x
	float scale;	// �G�̑傫��
	std::vector<EnemyActionData> actionDataList;	// �G�̍s���f�[�^���X�g
};

// �E�F�[�u�f�[�^
struct WaveData
{
	std::vector<EnemyData> enemyDataList;	// �G�̃f�[�^���X�g
};

/// <summary>
/// �G�̊Ǘ��N���X
/// </summary>
class EnemyManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pPlayer">�v���C���[�̃|�C���^</param>
	/// <param name="pLaserManager">���[�U�[�Ǘ��̃|�C���^</param>
	/// <param name="pScreenShaker">��ʗh��̃|�C���^</param>
	EnemyManager(
		const std::shared_ptr<Player>& pPlayer, 
		const std::shared_ptr<LaserManager>& pLaserManager,
		const std::shared_ptr<Effect::ScreenShaker>& pScreenShaker);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EnemyManager();

	/// <summary>
	/// �X�e�[�g�̍X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �ʏ�̍X�V
	/// </summary>
	void UpdateNormal();

	/// <summary>
	/// �x�����̍X�V
	/// </summary>
	void UpdateWarning();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �E�F�[�u�̊J�n
	/// </summary>
	void StartWave();

	/// <summary>
	/// ���̃E�F�[�u�Ɉڍs
	/// </summary>
	void NextWave();

	/// <summary>
	/// �G�̒ǉ�
	/// </summary>
	/// <param name="data">�G�̃f�[�^</param>
	void AddEnemy(const EnemyData& data);

	/// <summary>
	/// �{�X�G�̒ǉ�
	/// </summary>
	/// <param name="type">�{�X�G�̎��</param>
	void AddBossEnemy(const BossEnemyType& type);

	/// <summary>
	/// �G�̃X�e�[�W�f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	void LoadEnemyStageFileData(const std::string& filePath);		

	/// <summary>
	/// �G�̃E�F�[�u�̃f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>�ǂݍ��񂾃f�[�^</returns>
	std::vector<EnemyData> LoadEnemyWaveFileData(const std::string& filePath);	

	/// <summary>
	/// �G�̍s���f�[�^��ǂݍ���
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>�ǂݍ��񂾃f�[�^</returns>
	std::vector<EnemyActionData> LoadEnemyActionFileData(const std::string& filePath);	

	/// <summary>
	/// �G���G���X�g�̎擾
	/// </summary>
	/// <returns>�G���G���X�g</returns>
	const std::list<std::shared_ptr<EnemyBase>>& GetEnemyList() const;	

	/// <summary>
	/// �{�X�G�̎擾
	/// </summary>
	/// <returns>�{�X�G</returns>
	const std::shared_ptr<EnemyBase>& GetBossEnemy() const;				

	/// <summary>
	/// �{�X�G���������Ă��邩�ǂ����̎擾
	/// </summary>
	/// <returns></returns>
	bool IsBossAlive() const;

	/// <summary>
	/// �{�X���|����A���S���o�����ǂ����̎擾
	/// �{�X���o�����Ă��Ȃ��ꍇ��false��Ԃ�
	/// </summary>
	/// <returns>true : ���S���o���Afalse : ���S���o���łȂ�</returns>
	bool IsDeadBossAnim() const;

	/// <summary>
	/// �{�X���|���ꂽ���A���S���o���I���������̎擾
	/// </summary>
	/// <returns>true : �|���ꂽ�Afalse : �܂��������Ă���</returns>
	bool IsDeadBoosEndAnim() const;

	/// <summary>
	/// ���݂̃E�F�[�u���I���������ǂ����̎擾
	/// </summary>
	/// <returns>true : �I���Afalse : �r��</returns>
	bool IsEndWave() const;												

private:
	// �X�e�[�g
	enum class State
	{
		NORMAL,
		WARNING,
	};

	// �E�F�[�u�̃C���f�b�N�X
	enum class EnemyWaveIndex
	{
		POS_X,
		POS_Y,
		POS_Z,
		TYPE,
		HP,
		ATTACK,
		SCALE,
		SPEED,
		ACTION,
	};

	// �G�̍s���̃C���f�b�N�X
	enum class EnemyActionIndex
	{
		GOAL_POS_X,
		GOAL_POS_Y,
		GOAL_POS_Z,
		IDLE_FRAME,
		IS_LASER,
		LASER_TYPE,
		LASER_CHARGE_FRAME,
		LASER_IDLE_FRAME,
		LASER_SPEED,
		LASER_FIRE_FRAME,
		IS_PLAYER_FOLLOWING,
	};

private:
	// �X�e�[�g�}�V��
	StateMachine<State> m_stateMachine;	

	// �|�C���^
	std::shared_ptr<Player> m_pPlayer;						// �v���C���[
	std::shared_ptr<LaserManager> m_pLaserManager;			// ���[�U�[�Ǘ�
	std::list<std::shared_ptr<EnemyBase>> m_pEnemyList;		// �G���G���X�g
	std::shared_ptr<EnemyBase> m_pBossEnemy;				// �{�X�G
	std::shared_ptr<Warning> m_pWarning;					// �x��
	std::shared_ptr<Effect::ScreenShaker> m_pScreenShaker;			// ��ʗh��

	// �t���O
	bool m_isDeadBoss;			// �{�X���|���ꂽ���ǂ���
	bool m_isBoss;				// �{�X���o���������ǂ���
	bool m_isNextWave;			// ���̃E�F�[�u�Ɉڍs���邩�ǂ���
	bool m_isLoadWave;			// �E�F�[�u�f�[�^��ǂݍ��񂾂��ǂ���
	bool m_isStartWave;			// �E�F�[�u���J�n�������ǂ���
	bool m_isDeadEffectSound;	// ���S���o�̃T�E���h���Đ��������ǂ���

	// �E�F�[�u
	std::vector<WaveData> m_waveTable;	// �E�F�[�u�f�[�^
	BossEnemyType m_bossType;			// �{�X�̎��
	int m_waveNow;						// ���݂̃E�F�[�u��
};