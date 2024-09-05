#pragma once
#include "../DataType/PlayerParamDataType.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Util/Timer.h"
#include <deque>
#include <memory>
#include <string>
#include <map>

// �v���g�^�C�v�錾
class Model;
class Shield;
class Gauge;
class StatusBack;
class Camera;

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objectDataFileName">�I�u�W�F�N�g�f�[�^�̃t�@�C����</param>
	Player(const std::string& objectDataFileName);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Player();

	/// <summary>
	/// �Q�[���J�n���̍X�V
	/// </summary>
	/// <param name="cameraPos">�J�����̈ʒu</param>
	void UpdateStart(const Vector3& cameraPos);

	/// <summary>
	/// �Q�[�����̍X�V
	/// </summary>
	/// <param name="cameraHorizon">�J�����̐����p�x</param>
	void UpdatePlay(const float cameraHorizon);

	/// <summary>
	/// �Q�[���N���A���̍X�V
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// �Q�[���I�[�o�[���̍X�V
	/// </summary>
	/// <returns>�Q�[���I�[�o�[���I��������</returns>
	bool UpdateGameOver();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �V�[���h�̕`��
	/// </summary>
	void DrawShield();

	/// <summary>
	/// �O���t�@�C������p�����[�^��ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	void LoadParameter(const std::string& fileName);

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="damage">�_���[�W��</param>
	void OnDamage(const int damage);

	/// <summary>
	/// �V�[���h�����˂������̏���
	/// </summary>
	void OnReflect();

	/// <summary>
	/// �v���C���[�����X�ɓ����ɂ���
	/// </summary>
	void FadeOut();


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���f���n���h���̎擾
	/// </summary>
	/// <returns>���f���n���h��</returns>
	int GetModelHandle() const;		

	/// <summary>
	/// �����蔻��̔��a�̎擾
	/// </summary>
	/// <returns>�����蔼�a</returns>
	float GetCollisionRadius() const;	

	/// <summary>
	/// �X�^�[�g���o���������t���O�̎擾
	/// </summary>
	/// <returns></returns>
	bool IsStartAnimation() const;

	/// <summary>
	/// �v���C���[�����݂��Ă��邩�̎擾
	/// </summary>
	/// <returns>�v���C���[�����݂��Ă��邩</returns>
	bool IsEnabled() const;					

	/// <summary>
	/// �ʒu���̎擾
	/// </summary>
	/// <returns>�ʒu���</returns>
	const Vector3& GetPos() const;	

	/// <summary>
	/// �ړ��x�N�g���̎擾
	/// </summary>
	/// <returns>�ړ��x�N�g��</returns>
	const Vector3& GetMoveVec() const;		

	/// <summary>
	/// �V�[���h�̃|�C���^�̎擾
	/// </summary>
	/// <returns>�V�[���h�̃|�C���^</returns>
	const std::shared_ptr<Shield>& GetShield() const;

	/// <summary>
	/// �v���C���[�̈ʒu����ۑ����Ă���e�[�u���̎擾
	/// </summary>
	/// <returns>�ʒu����ۑ����Ă���e�[�u��</returns>
	const std::deque<Vector3>& GetPosLogTable() const;

	/// <summary>
	/// �p�����[�^�̎擾
	/// </summary>
	/// <param name="type">�p�����[�^�̎��</param>
	/// <returns>�p�����[�^</returns>
	float GetParameter(const DataType::PlayerParamType type);


	//// �Z�b�^�[ ////

	/// <summary>
	/// �J�����̃|�C���^�[�̐ݒ�
	/// </summary>
	/// <param name="pCamera">�J�����̃|�C���^</param>
	void SetCameraPointer(const std::shared_ptr<Camera>& pCamera);

private:
	// �|�C���^
	std::shared_ptr<Model> m_pModel;		// ���f��
	std::shared_ptr<Shield> m_pShield;		// �V�[���h
	std::shared_ptr<Gauge> m_pHPbar;		// HP�o�[
	std::shared_ptr<StatusBack> m_pBackUI;	// �X�e�[�^�X�o�b�NUI
	std::shared_ptr<Camera> m_pCamera;		// �J����

	// �p�����[�^�[
	Vector3 m_pos;				// �ʒu
	Vector3 m_rot;				// ��]
	Vector3 m_scale;			// �g�嗦
	Vector3 m_moveVec;			// �ړ��x�N�g��
	Vector3 m_moveSpeed;		// �ړ����x
	Math::Vector2 m_playerSize;		// �v���C���[�̃T�C�Y

	float   m_startMoveSpeedZ;	// �X�^�[�g���̑��x
	float   m_opacity;			// �����x
	float   m_collisionRadius;	// �����蔻��̔��a

	// HP
	int m_hp;
	int m_maxHp;

	// �t���O
	bool m_isInputLeftStick;	// ���X�e�B�b�N�̓��̓t���O
	bool m_isReflect;			// ���˒���
	bool m_isStartAnimation;	// �X�^�[�g���o���������t���O
	bool m_isGameOverSE;		// �Q�[���I�[�o�[SE���Đ��������t���O

	// �G�t�F�N�g
	int		m_boostEffectHandle;		// �u�[�X�g�G�t�F�N�g�n���h��
	Vector3 m_boostEffectScale;			// �u�[�X�g�G�t�F�N�g�̊g�嗦
	float	m_boostEffectSpeed;			// �u�[�X�g�G�t�F�N�g�̑��x
	int		m_damageEffectHandle;		// �_���[�W�G�t�F�N�g�n���h��
	int		m_playerDeadEffectHandle;	// �v���C���[���S�G�t�F�N�g�n���h��
	bool	m_isPlayerDeadEffect;		// �v���C���[���S�G�t�F�N�g�̍Đ��t���O

	// �^�C�}�[
	int		   m_gameOverWaitFrame;			// �ҋ@�t���[��
	int		   m_onDamageSEWaitFrame;		// �_���[�WSE�̍Đ��҂��t���[��
	int		   m_ultimateTimer;				// ���G���Ԃ̃^�C�}�[
	Timer<int> m_waitTimer;					// �ҋ@���Ԃ̃^�C�}�[

	// �O���t�@�C������ǂݍ��񂾃p�����[�^
	std::map<std::string, float> m_parameterTable;

	// ���߂�ꂽ�t���[���O�܂ňʒu����ۑ����Ă���e�[�u��
	std::deque<Vector3> m_posLogTable;
};