#include "Camera.h"
#include "../Math/MathUtil.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../MyDebug/DebugText.h"
#include "../Util/Easing.h"
#include <DxLib.h>
#include <algorithm>
#include <cassert>

namespace
{
	// �J�����̎���p
	constexpr float camera_perspective = 70.0f;

	// �`�拗��(near, far)
	constexpr float near_distance = 5.0f;
	constexpr float far_distance  = 50000.0f;

	// �v���C���[����̃J�����܂ł̋���
	constexpr float camera_distance = 400.0f;

	// �J�����̈ړ����x
	constexpr float camera_move_speed = 7.5f;

	// �J�����̈ړ������͈�
	const Math::Vector2 camera_move_limit = { 150.0f * 3.0f, 100.0f * 3.0f };

	// �J�������v���C���[�̃x�N�g���ɍ��킹�ď����ړ����銄��
	constexpr float camera_move_ratio = 0.7f;

	// �Q�[���X�^�[�g�̉��o���J�n����v���C���[�̈ʒu
	constexpr float camera_start_animation_start_pos_z = 200.0f;

	// �Q�[���X�^�[�g��
	constexpr int camera_start_animation_frame = 200;								// �J�����̉��o�̃t���[����
	const Math::Vector3 camera_init_relative_player_pos        = { 500.0f, 100.0f, 2000.0f };	// �J�����̏����ʒu
	const Math::Vector3 camera_start_animation_end_pos         = { 0.0f, 50.0f, -400.0f };// �J�����̈ړ���̈ʒu	
	const Math::Vector3 camera_start_animation_start_direction = { -1.0f, 0.0f, -1.0f };	// �J�����̈ړ���̊J�n�̕���
	const Math::Vector3 camera_start_animation_end_direction   = { 1.0f, 0.0f, 1.0f };	// �J�����̈ړ���̏I���̕���

	// �Q�[���N���A��
	constexpr int camera_game_clear_target_frame = 300;		// �J�����̃^�[�Q�b�g���v���C���[�Ɍ�����t���[����
	constexpr int camera_game_clear_move_frame   = 300;		// �J�����̈ʒu���ړ�������t���[����
	const Math::Vector3 camera_game_clear_end_pos = { 200.0f, 50.0f, 200.0f };	// �J�����̈ړ���̈ʒu
	const Math::Vector3 camera_game_clear_start_direction = { 1.0f, 0.0f, 1.0f };	// �J�����̈ړ���̊J�n�̕���
	const Math::Vector3 camera_game_clear_end_direction = { -1.0f, 0.0f, -1.0f };	// �J�����̈ړ���̏I���̕���

	// �Q�[���I�[�o�[���̃J�����̃^�[�Q�b�g���v���C���[�Ɍ�����t���[����
	constexpr int camera_game_over_target_frame = 300;

	// �ړI�n�ɓ��B�������ǂ����̔���
	// �����臒l�i�K�؂Ȓl�ɒ�������K�v�j
	constexpr float camera_pos_distance_threshold    = 10.0f;	// �J�����̈ʒu
	constexpr float camera_target_distance_threshold = 10.0f;	// �J�����̒����_
}

// �R���X�g���N�^
Camera::Camera() :
	m_pos({ 0.0f, 0.0f, 0.0f }),
	m_target({ 0.0f, 0.0f, 0.0f }),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_gameClearLerpFrame(0),
	m_hermiteFrame(0),
	m_gameOverLerpFrame(0)
{
	// �J�����̐ݒ�
	SetCamera();
}

// �R���X�g���N�^
Camera::Camera(const Math::Vector3& playerPos) :
	m_pos(playerPos + camera_init_relative_player_pos),
	m_target(playerPos),
	m_perspective(camera_perspective),
	m_cameraVertical(0.0f),
	m_cameraHorizon(DX_PI_F),
	m_isStartAnimation(false),
	m_gameClearLerpFrame(0),
	m_hermiteFrame(0),
	m_gameOverLerpFrame(0)
{
	// �J�����̐ݒ�
	SetCamera();
}

// �f�X�g���N�^
Camera::~Camera()
{
}

// �v���C���̍X�V
void Camera::UpdatePlay(const Math::Vector3& playerPos, const Math::Vector3& playerVec)
{
#if true
	// �J�������v���C���[�̃x�N�g���ɍ��킹�ď����ړ�
	m_pos.x += playerVec.x * camera_move_ratio;
	m_pos.y += playerVec.y * camera_move_ratio;

	// �J�����̃^�[�Q�b�g����ɐ��ʂɌ�����
	m_target.x = m_pos.x;
	m_target.y = m_pos.y - camera_start_animation_end_pos.y;

	// �J�����̃^�[�Q�b�g�̐���
	m_target.x = std::clamp(m_target.x, -camera_move_limit.x, camera_move_limit.x);
	m_target.y = std::clamp(m_target.y, 
							-camera_move_limit.y - camera_start_animation_end_pos.y, 
							 camera_move_limit.y - camera_start_animation_end_pos.y);

	// �J�����̈ړ�����
	m_pos.x = std::clamp(m_pos.x, -camera_move_limit.x, camera_move_limit.x);
	m_pos.y = std::clamp(m_pos.y, -camera_move_limit.y, camera_move_limit.y);
#endif

	// �J�����ƃv���C���[�̍���
	Math::Vector3 direction = playerPos - m_pos;

	// Y���AX���𖳎�
	direction.y = 0.0f;
	direction.x = 0.0f;

	// �J�����ƃv���C���[�̋��������ȏ㗣��Ă�����
	if (direction.Length() > camera_distance)
	{
		// �J�����ƒ����_�̈ړ�
		direction.Normalize();
		m_pos    += (direction * camera_move_speed);
		m_target += (direction * camera_move_speed);
	}

	// �J�����̐ݒ�
	SetCamera();

	// �f�o�b�O���O�̒ǉ�
	Debug::Text::AddLog("CameraPos",    { m_pos.x, m_pos.y, m_pos.z });
	Debug::Text::AddLog("CameraTarget", { m_target.x, m_target.y, m_target.z });
}

// �X�^�[�g���o���̍X�V
void Camera::UpdateStart(const Math::Vector3& playerPos)
{
	// �����_�̍X�V
	m_target = playerPos;

	// �v���C���[�̈ʒu���J�����̈ʒu���O�ɗ�����
	if (playerPos.z > m_pos.z + camera_start_animation_start_pos_z)
	{
		// �G���~�[�g�Ȑ��̒l�𑝂₷
		m_hermiteFrame     = (std::min)(++m_hermiteFrame, camera_start_animation_frame);
		float hermiteValue = static_cast<float>(m_hermiteFrame) / static_cast<float>(camera_start_animation_frame);

		// �J�����̈ʒu���G���~�[�g�Ȑ��ňړ�������
		m_pos = Math::Vector3::Hermite
			(
				m_pos,
				camera_start_animation_start_direction,
				playerPos + camera_start_animation_end_pos,
				camera_start_animation_end_direction,
				hermiteValue
			);

		// �G���~�[�g�Ȑ��̒l��1.0f�𒴂�����
		if (m_hermiteFrame >= camera_start_animation_frame)
		{
			m_isStartAnimation = true;
			m_hermiteFrame = 0;
		}
	}

	// �J�����̐ݒ�
	SetCamera();
}

// �Q�[���N���A���̍X�V
bool Camera::UpdateGameClear(const Math::Vector3& playerPos)
{
	// �t���[�����𑝂₷
	m_gameClearLerpFrame = (std::min)(++m_gameClearLerpFrame, camera_game_clear_target_frame);

	// �t���[�������烉�[�v�̒l���Z�o
	float lerp  = static_cast<float>(m_gameClearLerpFrame) / static_cast<float>(camera_game_clear_target_frame);

	// �J�����̃^�[�Q�b�g���v���C���[�̈ʒu�ɏ��X�ɕύX
	m_target = Math::Vector3::Lerp(m_target, playerPos, lerp);

	// �t���[�����𑝂₷
	m_hermiteFrame = (std::min)(++m_hermiteFrame, camera_game_clear_move_frame);

	// �G���~�[�g�Ȑ��̒l���Z�o
	float hermiteValue = static_cast<float>(m_hermiteFrame) / static_cast<float>(camera_game_clear_move_frame);

	// �J�����̈ړ���̈ʒu
	Math::Vector3 endPos = playerPos + camera_game_clear_end_pos;

	// �J�����̈ʒu���G���~�[�g�Ȑ��ňړ�
	m_pos = Math::Vector3::Hermite
	(
		m_pos,
		camera_game_clear_start_direction,
		endPos,
		camera_game_clear_end_direction,
		hermiteValue
	);

	// �J�����̐ݒ�
	SetCamera();

	// �ڕW�ʒu�ɓ��B������
	if (endPos.Distance(m_pos) < camera_pos_distance_threshold && 
		m_target.Distance(playerPos) < camera_target_distance_threshold)
	{
		// �J�����̉��o���I��
		return true;
	}

	// �J�����̉��o���I�����Ă��Ȃ�
	return false;
}

// �Q�[���I�[�o�[���̍X�V
void Camera::UpdateGameOver(const Math::Vector3& playerPos)
{
	// �J�����̃^�[�Q�b�g���v���C���[�̈ʒu�ɏ��X�ɕύX
	m_gameOverLerpFrame = (std::min)(++m_gameOverLerpFrame, camera_game_over_target_frame);
	float lerp = static_cast<float>(m_gameOverLerpFrame) / static_cast<float>(camera_game_over_target_frame);
	m_target = Math::Vector3::Lerp(m_target, playerPos, lerp);

	// �J�����̐ݒ�
	SetCamera();
}

// �J�����̐ݒ�
void Camera::SetCamera()
{
	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(near_distance, far_distance);

	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(Math::Util::ToRadian(m_perspective));

	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTarget_UpVecY(
		m_pos.ToDxLibVector3(),
		m_target.ToDxLibVector3());
}

// �J�����̐ݒ�
void Camera::SetCamera(const Math::Vector3& pos, const Math::Vector3& target)
{
	// �J�����̍��W�ƒ����_�̍X�V
	m_pos    = pos;
	m_target = target;

	// �J�����̐ݒ�
	SetCamera();
}

// �J�����̈ʒu�̎擾
const Math::Vector3& Camera::GetPos() const
{
	return m_pos;
}

// �J�����̒����_�̎擾
const Math::Vector3& Camera::GetTarget() const
{
	return m_target;
}

// �J�����̐����������擾
float Camera::GetCameraHorizon() const
{
	return m_cameraHorizon;
}

// �X�^�[�g���o���������t���O�̎擾
bool Camera::IsStartAnimation() const
{
	return m_isStartAnimation;
}

// �J������X���̈ړ��͈͐����𒴂�����
bool Camera::IsOverMoveRangeX() const
{
	// �ړ��͈͂𒴂�����
	if (m_pos.x >= camera_move_limit.x || m_pos.x <= -camera_move_limit.x)
	{
		// �ړ��͈͂𒴂��Ă���
		return true;
	}

	// �ړ��͈͂𒴂��Ă��Ȃ�
	return false;
}

// �J������Y���̈ړ��͈͐����𒴂�����
bool Camera::IsOverMoveRangeY() const
{
	// �ړ��͈͂𒴂�����
	if (m_pos.y >= camera_move_limit.y || m_pos.y <= -camera_move_limit.y)
	{
		// �ړ��͈͂𒴂��Ă���
		return true;
	}

	// �ړ��͈͂𒴂��Ă��Ȃ�
	return false;
}

// �J�����̈ړ����x�̎擾
float Camera::GetCameraMoveSpeed() const
{
	return camera_move_speed;
}

// �J�������v���C���[�̃x�N�g���ɍ��킹�ď����ړ����銄�� 
float Camera::GetCameraMoveRate() const
{
	return camera_move_ratio;
}