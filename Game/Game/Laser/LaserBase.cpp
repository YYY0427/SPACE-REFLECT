#include "LaserBase.h"
#include "../../Game/Model.h"
#include "../../Effect/Effekseer3DEffectManager.h"

// �R���X�g���N�^
LaserBase::LaserBase() :
	m_isEnabled(true),
	m_isReflect(false),
	m_speed(0.0f),
	m_opacity(1.0f),
	m_reflectLaserKey(-1)
{
}

// �f�X�g���N�^
LaserBase::~LaserBase()
{
}

// ���[�U�[�̍폜
void LaserBase::Delete()
{
	m_isEnabled = false;
}

// �ʒu�̐ݒ�
void LaserBase::SetPos(const Math::Vector3& pos)
{
	m_pos = pos;
}

// ���˃��[�U�[�̃L�[�̐ݒ�
void LaserBase::SetReflectLaserKey(const int key)
{
	m_reflectLaserKey = key;
}

// ���f���n���h���̎擾
int LaserBase::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

// ���˃��[�U�[�̃L�[�̎擾
int LaserBase::GetReflectLaserKey() const
{
	return m_reflectLaserKey;
}

// �ʒu���̎擾
const Math::Vector3& LaserBase::GetPos() const
{
	return m_pos;
}

// ���f���̐�[�̈ʒu�̎擾
const Math::Vector3& LaserBase::GetEndPos() const
{
	return m_endPos;
}

// �����x�N�g���̎擾
Math::Vector3 LaserBase::GetDirection() const
{
	return { 0, 0, 0 };
}

// ���݃t���O�̎擾
bool LaserBase::IsEnabled() const
{
	return m_isEnabled;
}

// ���˃t���O�̎擾
bool LaserBase::IsReflect() const
{
	return m_isReflect;
}
