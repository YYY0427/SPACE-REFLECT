#include "EnemyBase.h"
#include "../../Effect/Effekseer3DEffectManager.h"
#include "../../Model.h"
#include "../../Score/Score.h"
#include "../../Sound/SoundManager.h"

namespace
{
	// ���S�G�t�F�N�g�̍Đ����x
	constexpr float dead_effect_play_speed = 0.5f;

	// ���S�G�t�F�N�g�̊g�嗦
	const Math::Vector3 dead_effect_scale = { 200.0f, 200.0f, 200.0f };
}

// �R���X�g���N�^
EnemyBase::EnemyBase() :
	m_isEnabled(true),
	m_pos({ 0, 0, 0 }),
	m_rot({ 0, 0, 0 }),
	m_hp(0),
	m_opacity(0.0f),
	m_attackPower(0),
	m_moveSpeed(0.0f),
	m_deadEffectHandle(-1),
	m_movePointIndex(0),
	m_isGoal(false),
	m_collisionRadius(0.0f)
{
}

// �f�X�g���N�^
EnemyBase::~EnemyBase()
{
}

// �_���[�W����
void EnemyBase::OnDamage(const int damage, const Math::Vector3& pos)
{
	// HP�����炷
	m_hp -= damage;

	// HP��0�ȉ��ɂȂ����玀�S
	if (m_hp <= 0)
	{
		// �X�R�A�̉��Z
		Score::GetInstance().AddScore(ScoreType::ENEMY);

		// ���S���̍Đ�
		Sound::Manager::GetInstance()->PlaySE("NormalEnemyDie");

		// ���S�G�t�F�N�g�̍Đ�
		Effect::Effekseer3DManager::GetInstance()->PlayEffect(
			m_deadEffectHandle,
			"EnemyDied",
			m_pos,
			dead_effect_scale,
			dead_effect_play_speed);

		// ���S�t���O�𗧂Ă�
		m_isEnabled = false;
	}
	else
	{
		// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�𔽓]
		m_pModel->InversAllMaterialDifColor();
	}
}

// �ʒu�̎擾
const Math::Vector3& EnemyBase::GetPos() const
{
	return m_pos;
}

// ���[�U�[�̔��ˈʒu�̎擾
const Math::Vector3& EnemyBase::GetLaserFirePos() const
{
	return m_laserFirePos;
}

// ���݃t���O�̎擾
bool EnemyBase::IsEnabled() const
{
	return m_isEnabled;
}

// �{�X�̎��S���o����
bool EnemyBase::IsDeadAnim()
{
	return false;
}

// �����蔻��̔��a�̎擾
float EnemyBase::GetCollisionRadius() const
{
	return m_collisionRadius;
}
