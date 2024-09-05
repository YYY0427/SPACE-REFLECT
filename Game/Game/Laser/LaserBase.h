#pragma once
#include <memory>
#include "../../Math/Vector3.h"

// �v���g�^�C�v�錾
class Model;

/// <summary>
/// ���[�U�[�̊��N���X
/// </summary>
class LaserBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LaserBase();
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LaserBase();
	
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ���[�U�[���~�߂�
	/// </summary>
	/// <param name="pos">���[�U�[���~�߂�ʒu</param>
	virtual void Stop(const Math::Vector3& pos) {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="pos">���˂���ʒu</param>
	virtual void Reflect(const Math::Vector3& pos) {};

	/// <summary>
	/// ���˂����ɖ߂�
	/// </summary>
	virtual void UndoReflect() {};

	/// <summary>
	/// ���[�U�[������
	/// </summary>
	void Delete();


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���f���n���h�����擾
	/// </summary>
	/// <returns>���f���n���h��</returns>
	int GetModelHandle() const;			

	/// <summary>
	/// ���˃��[�U�[�̃L�[���擾
	/// </summary>
	/// <returns>���˃��[�U�[�̃L�[</returns>
	int GetReflectLaserKey() const;			

	/// <summary>
	/// �ʒu�����擾
	/// </summary>
	/// <returns>�ʒu���</returns>
	const Math::Vector3& GetPos() const;

	/// <summary>
	/// ���[�U�[�̏I�[�ʒu���擾
	/// </summary>
	/// <returns>���[�U�[�̏I�[�ʒu</returns>
	const Math::Vector3& GetEndPos() const;

	/// <summary>
	/// �����x�N�g�����擾
	/// </summary>
	/// <returns>�����x�N�g��</returns>
	virtual Math::Vector3 GetDirection() const;

	/// <summary>
	/// ���݃t���O�̎擾
	/// </summary>
	/// <returns>true : ���݁Afalse : ���݂��Ă��Ȃ�</returns>
	bool IsEnabled() const;

	/// <summary>
	/// ���˃t���O�̎擾
	/// </summary>
	/// <returns>true : ���ˁAfalse : ���˂��Ă��Ȃ�</returns>
	bool IsReflect() const;	


	//// �Z�b�^�[ ////

	/// <summary>
	/// �ʒu����ݒ�
	/// </summary>
	/// <param name="pos">�ʒu���</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// ���˃��[�U�[�̃L�[��ݒ�
	/// </summary>
	/// <param name="key">���˃��[�U�[�̃L�[</param>
	void SetReflectLaserKey(const int key);

protected:
	// �|�C���^
	std::shared_ptr<Model> m_pModel;

	// �ʒu���
	Math::Vector3 m_pos;

	// �J�n�ʒu
	Math::Vector3 m_startPos;

	// �I���ʒu
	Math::Vector3 m_endPos;

	// ��]���
	Math::Vector3 m_rot;

	// �g�嗦
	Math::Vector3 m_scale;

	// ���x
	float m_speed;		

	// �s�����x
	float m_opacity;	

	// ���݃t���O
	bool m_isEnabled;

	// ���˃t���O
	bool m_isReflect;

	// ���˃��[�U�[�̃L�[
	int m_reflectLaserKey;	
};

