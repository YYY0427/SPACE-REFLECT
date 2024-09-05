#include "Player.h"
#include "Shield.h"
#include "../Util/InputState.h"
#include "../Util/FileUtil.h"
#include "../Util/Range.h"
#include "../Application.h"
#include "../Math/Matrix.h"
#include "../Math/MathUtil.h"
#include "../Math/Vector2.h"
#include "../UI/Gauge.h"
#include "../UI/StatusBack.h"
#include "../UI/ImageUI.h"
#include "../UI/UIManager.h"
#include "../Model.h"
#include "../ModelHandleManager.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../MyDebug/DebugText.h"
#include "../Sound/SoundManager.h"
#include "Camera.h"
#include <algorithm>

namespace
{
	// �v���C���[�̃p�����[�^�̃t�@�C���p�X
	const std::string player_param_file_path = "Data/Csv/PlayerParam.csv";

	// �v���C���[�̈ړ���
	const Vector3 player_vec_up    = { 0, 1, 0 };
	const Vector3 player_vec_down  = { 0, -1, 0 };
	const Vector3 player_vec_right = { -1, 0, 0 };
	const Vector3 player_vec_left  = { 1, 0, 0 };

	// �v���C���[�̏����̌����Ă������
	const Vector3 init_model_direction = { 0, 0, 1 };

	// ���t���[���O�܂ňʒu����ۑ����邩
	constexpr int log_frame = 10;

	// �v���C���[��HP����
	const Math::Vector2 hp_string_pos   = { 70, 560 };	// �ʒu
	const Math::Vector2 hp_string_scale = { 0.8f, 0.8f };	// �g�嗦

	// �v���C���[��HP�o�[
	const Math::Vector2 hp_bar_pos  = { 215, 600 };		// �ʒu
	const Math::Vector2 hp_bar_size = { 300, 13 };		// �T�C�Y

	// �v���C���[��HP�o�[�̃t�@�C���p�X
	const std::string hp_bar_file_path = "Data/Image/HPBar.png";

	// �v���C���[��HP�o�[�̔w�i�摜�̃t�@�C���p�X
	const std::string hp_bar_back_file_path = "Data/Image/StatusBase.png";

	// �v���C���[��HP�����̃t�@�C���p�X
	const std::string hp_string_file_path = "Data/Image/Life.png";

	// �Q�[���I�[�o�[���o�̃t���[����
	const int game_over_frame = 60 * 5;

	// �u�[�X�g�G�t�F�N�g�̃v���C���[�Ƃ̑��Έʒu
	const Vector3 boost_effect_relative_pos = { 0, -30.0f, -30.0f };

	// �v���C���[��i��ł���悤�Ɍ����邽�߂ɌX����p�x
	constexpr float tilt_angle = 25.0f;

	// �J�������v���C���[��ǂ��z���Ă��邩�ǂ����͈̔�
	constexpr float camera_over_range = 200.0f;

	// �v���C���[�̈ړ��x�N�g���̌�����
	constexpr float move_vec_decrease_rate = 0.9f;

	// �����x�̍ő�l
	constexpr float max_opacity = 1.0f;

	// �v���C���[���X����傫��
	constexpr float tilt_size = 0.02f;

	// �Q�[���I�[�o�[���̃v���C���[��1�t���[�����Ƃɉ�]������p�x
	constexpr float game_over_rotate_angle = 5.0f;

	// �Q�[���I�[�o�[���̃v���C���[�̈ړ��x�N�g��
	const Vector3 game_over_move_vec = { 0.0f, -0.5f, 1.0f };
}

//  �R���X�g���N�^
Player::Player(const std::string& objectDataFileName) :
	m_moveVec(0, 0, 0),
	m_ultimateTimer(0),
	m_isInputLeftStick(false),
	m_playerDeadEffectHandle(-1),
	m_isPlayerDeadEffect(false),
	m_isStartAnimation(false),
	m_isReflect(false),
	m_boostEffectHandle(-1),
	m_damageEffectHandle(-1),
	m_opacity(1.0f),
	m_gameOverWaitFrame(game_over_frame),
	m_isGameOverSE(false)
{
	// �f�[�^�̓ǂݍ���
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Player");
	m_pos     = data.front().pos;
	m_rot     = data.front().rot;
	m_scale   = data.front().scale;

	// �O���t�@�C������p�����[�^��ǂݍ���
	LoadParameter(player_param_file_path);
	m_maxHp = GetParameter(DataType::PlayerParamType::MAX_HP);
	m_hp    = m_maxHp;
	m_boostEffectScale.x = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_X);
	m_boostEffectScale.y = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_Y);
	m_boostEffectScale.z = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_Z);
	m_boostEffectSpeed   = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SPEED);
	m_playerSize.x		 = GetParameter(DataType::PlayerParamType::PlAYER_WIDTH);	
	m_playerSize.y       = GetParameter(DataType::PlayerParamType::PlAYER_HEIGHT);
	m_collisionRadius	 = GetParameter(DataType::PlayerParamType::COLLISION_RADIUS);
	m_moveSpeed.x		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_XY);
	m_moveSpeed.y		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_XY);
	m_moveSpeed.z		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_Z);

	// �v���C���[���f���̃C���X�^���X�̐���
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Player"));

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);// �s�����x
	m_pModel->SetScale(m_scale);	// �g�嗦
	m_pModel->SetRot(m_rot);		// ��]
	m_pModel->SetPos(m_pos);		// �ʒu
	m_pModel->Update();				// �X�V

	// �u�[�X�g�G�t�F�N�g�̍Đ�
	Effect::Effekseer3DManager::GetInstance()->PlayEffectLoop(
		m_boostEffectHandle, 
		"PlayerBoost",
		m_pos + boost_effect_relative_pos,
		m_boostEffectScale,
		m_boostEffectSpeed,
		{ m_rot.x, 0.0f, 0.0f });
}

//  �f�X�g���N�^
Player::~Player()
{
}

// �X�^�[�g���o�̍X�V
void Player::UpdateStart(const Vector3& cameraPos)
{
	// Z�������Ɉړ�
	m_moveVec.z = GetParameter(DataType::PlayerParamType::START_MOVE_SPEED_Z);
	m_pos.z += m_moveVec.z;

	if (m_pos.z > cameraPos.z + camera_over_range)
	{
		// �X�^�[�g���o���������t���O�𗧂Ă�
		m_isStartAnimation = true;
	}

	// �v���C���[���X����
	m_rot.x = -Math::Util::ToRadian(tilt_angle);

	// �G�t�F�N�g�̐ݒ�
	const auto& effectManager = Effect::Effekseer3DManager::GetInstance();
	effectManager->SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager->SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager->SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager->SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);

	// ���f���̐ݒ�
	m_pModel->SetPos(m_pos);	// �ʒu
	m_pModel->SetRot(m_rot);	// ��]
	m_pModel->Update();			// �X�V
}

// �X�V
void Player::UpdatePlay(const float cameraHorizon)
{
	if (!m_pBackUI)
	{
		const auto& soundManager = Sound::Manager::GetInstance();
		soundManager->PlaySE("GageRecovery");

		// �v���C���[UI�̔w�i�摜�̃C���X�^���X�̐���
		m_pBackUI = std::make_shared<StatusBack>();

		// UI�̓o�^
		UIManager::GetInstance().AddUI("PlayerStatusBack", m_pBackUI, 0, { -2, 0 });
	}

	// �X�^�[�g���o�����Ă��Ȃ��ꍇ
	if (!m_pBackUI->IsStartAnimation())
	{
		// �X�^�[�g���o
		m_pBackUI->UpdateStartAnimation();
	}
	// �X�^�[�g���o���I��������񂾂�
	else if(!m_pShield && !m_pHPbar)
	{
		// �V�[���h�̃C���X�^���X�̐���
		m_pShield = std::make_shared<Shield>(*this);

		// HP�o�[�̃C���X�^���X�̐���
		m_pHPbar = std::make_shared<Gauge>(
			hp_bar_file_path,
			hp_bar_back_file_path,
			"",
			m_maxHp,
			hp_bar_pos,
			hp_bar_size,
			120);

		// HP�����̃C���X�^���X�̐���
		auto pHpString = std::make_shared<ImageUI>(hp_string_file_path);
		pHpString->SetPos(hp_string_pos);
		pHpString->SetScale(hp_string_scale);

		// UI�̓o�^
		UIManager::GetInstance().AddUI("HPBar", m_pHPbar, 0, { -2, 0 });
		UIManager::GetInstance().AddUI("HPString", pHpString, 0, { -2, 0 });
	}

	const auto& effectManager = Effect::Effekseer3DManager::GetInstance();

	// ���X�e�B�b�N�̓��͏��̎擾
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	// �J�����̉�]�ɍ��킹�Ĉړ��x�N�g�����쐬
	Vector3 moveUp    = Vector3::Transform(player_vec_up, Math::Matrix::GetRotationY(cameraHorizon));
	Vector3 moveDown  = Vector3::Transform(player_vec_down, Math::Matrix::GetRotationY(cameraHorizon));
	Vector3 moveRight = Vector3::Transform(player_vec_right, Math::Matrix::GetRotationY(cameraHorizon));
	Vector3 moveLeft  = Vector3::Transform(player_vec_left, Math::Matrix::GetRotationY(cameraHorizon));

	// �ړ����̏�����
	m_isInputLeftStick = false;
	m_moveVec.z = 0;
	Vector3 moveVecX = { 0, 0, 0 };
	Vector3 moveVecY = { 0, 0, 0 };

	// �ړ��x�N�g���̌���
	m_moveVec *= move_vec_decrease_rate;

	// �X�e�B�b�N�����͂���Ă�����ړ��x�N�g���ɃX�e�B�b�N���X���Ă�������̃x�N�g������
	// �X�e�B�b�N�̌X�����킢�ɂ���ăx�N�g�����傫���Ȃ�
	if (up > 0)
	{
		moveVecY = moveUp * up;
		m_isInputLeftStick = true;
	}
	if (left > 0)
	{
		moveVecX = moveLeft * left;
		m_isInputLeftStick = true;
	}
	if (down > 0)
	{
		moveVecY = moveDown * down;
		m_isInputLeftStick = true;
	}
	if (right > 0)
	{
		moveVecX = moveRight * right;
		m_isInputLeftStick = true;
	}

	// �X�e�B�b�N�����͂���Ă���ꍇ�݈̂ړ�
	if (m_isInputLeftStick)
	{
		// �v���C���[���猩��x������z�����̃x�N�g���𑫂��Ĉړ��x�N�g�����쐬����
		m_moveVec = moveVecY + moveVecX;

		Vector3 moveSpeed;
		moveSpeed = m_moveSpeed;

		// �J�����̈ړ��͈͂𒴂��Ă���ꍇ
		// �ړ��X�s�[�h�����炷
		// (�J�����������Ȃ����߁A�J�����������Ă���Ƃ��ƈႤ�����Ɍ����邽��)
		if (m_pCamera->IsOverMoveRangeX())
		{
			moveSpeed.x = m_moveSpeed.x * (1.0f - m_pCamera->GetCameraMoveRate());
		}
		if (m_pCamera->IsOverMoveRangeY())
		{
			moveSpeed.y = m_moveSpeed.y * (1.0f - m_pCamera->GetCameraMoveRate());
		}

		// �v���C���[�̃X�s�[�h���|����
		m_moveVec = m_moveVec * moveSpeed;

		Debug::Text::AddLog("PlayerMoveSpeed", { moveSpeed.x, moveSpeed.y, moveSpeed.z });
	}

	// �쐬�����ړ��x�N�g���ō��W�̈ړ�
	Vector3 tempPos = m_pos + m_moveVec;

	// ���[���h���W���X�N���[�����W�ɕϊ�
	Vector3 screenPos = Vector3::FromDxLibVector3(
		ConvWorldPosToScreenPos(tempPos.ToDxLibVector3()));

	// ��ʊO�ɏo�Ȃ��悤�ɂ���
	Size size = Application::GetInstance().GetWindowSize();
	if (screenPos.x > size.width - m_playerSize.x)
	{
		screenPos.x = size.width - m_playerSize.x;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.x < 0 + m_playerSize.x)
	{
		screenPos.x = 0 + m_playerSize.x;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.y > size.height - m_playerSize.y)
	{
		screenPos.y = size.height - m_playerSize.y;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.y < 0 + m_playerSize.y)
	{
		screenPos.y = 0 + m_playerSize.y;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else
	{
		m_pos = tempPos;
	}

	// ���Z�������Ɉړ�
	m_moveVec.z = m_moveSpeed.z;
	m_pos.z += m_moveVec.z;

	// ���O�ɒǉ�
	m_posLogTable.push_front(m_pos);

	// �ۑ��\�Ȑ��𒴂��Ă����疖���������
	if (m_posLogTable.size() > log_frame)
	{
		m_posLogTable.pop_back();
	}

	// ���G���Ԃ̃^�C�}�[�̍X�V
	// 0�ȉ��ɂ͂Ȃ�Ȃ�
	m_ultimateTimer = (std::max)(--m_ultimateTimer, 0);

	Vector3 moveVec = m_moveVec;
	Vector3 moveSpeed = m_moveSpeed;
	if (m_pCamera->IsOverMoveRangeX())
	{
		moveSpeed.x = m_moveSpeed.x * (1.0f - m_pCamera->GetCameraMoveRate());
		moveVec.x /= moveSpeed.x;
		moveVec.x *= m_moveSpeed.x;
	}
	if (m_pCamera->IsOverMoveRangeY())
	{
		moveSpeed.y = m_moveSpeed.y * (1.0f - m_pCamera->GetCameraMoveRate());
		moveVec.y /= moveSpeed.y;
		moveVec.y *= m_moveSpeed.y;
	}

	// �ړ��x�N�g���̑傫������v���C���[�̌X������Z�o
	// X����]�͐i��ł���悤�Ɍ�����悤�X����
	m_rot = { Math::Util::ToRadian(tilt_angle) + (-moveVec.y * tilt_size), 0.0f, -moveVec.x * tilt_size };

	// �s�����x�����ɖ߂�
//	m_opacity = max_opacity;

	// �G�t�F�N�g�̐ݒ�
	effectManager->SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager->SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, 0.0f, -m_rot.z });
	effectManager->SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager->SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->SetRot(m_rot);			// ��]
	m_pModel->Update();					// �X�V

	// �V�[���h������ꍇ
	if (m_pShield)
	{
		// �V�[���h�̍X�V
		m_pShield->Update();
	}
}

// �Q�[���N���A�̍X�V
void Player::UpdateGameClear()
{
	// �V�[���h�̍폜
	m_pShield.reset();

	// ���Z�������Ɉړ�
	m_moveVec.z = m_moveSpeed.z;
	m_pos.z += m_moveVec.z;

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->SetPos(m_pos);			// �ʒu
	m_pModel->SetRot(m_rot);			// ��]
	m_pModel->Update();					// �X�V

	// �G�t�F�N�g�̐ݒ�
	const auto& effectManager = Effect::Effekseer3DManager::GetInstance();
	effectManager->SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager->SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager->SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager->SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);
}

// �Q�[���I�[�o�[�̍X�V
bool Player::UpdateGameOver()
{
	// �G�t�F�N�g�̒�~
	Effect::Effekseer3DManager::GetInstance()->DeleteEffect(m_boostEffectHandle);

	// �ړ��x�N�g���쐬
	m_moveVec = game_over_move_vec;
	m_moveVec = m_moveVec.Normalized();
	m_moveVec *= 2.0f;

	// �v���C���[����]
	m_rot += { 0, 0, Math::Util::ToRadian(game_over_rotate_angle) };

	// �쐬�����ړ��x�N�g���ō��W�̈ړ�
	m_pos = m_pos + m_moveVec;


	if (m_gameOverWaitFrame <= game_over_frame * 0.9)
	{
		if (!m_isGameOverSE)
		{
			// �v���C���[�����񂾉��̍Đ�
			Sound::Manager::GetInstance()->PlaySE("GameOver");
			m_isGameOverSE = true;
		}
	}

	if(m_gameOverWaitFrame-- <= 0)
	{
		// �܂��Đ����Ă��Ȃ��ꍇ
		if (!m_isPlayerDeadEffect)
		{
			// �t���O�𗧂Ă�
			m_isPlayerDeadEffect = true;

			// �ł��������G�t�F�N�g���Đ�
			Effect::Effekseer3DManager::GetInstance()->PlayEffect(
				m_playerDeadEffectHandle,
				"PlayerDied",
				m_pos,
				{ 50.0f, 50.0f, 50.0f },
				0.5f);

			// �傫���������̍Đ�
			Sound::Manager::GetInstance()->PlaySE("NormalEnemyDie");
		}
		else
		{
			// �ł��������G�t�F�N�g�̍Đ����I��������
			if (!Effect::Effekseer3DManager::GetInstance()->IsPlayingEffect(m_playerDeadEffectHandle))
			{
				return true;
			}
		}
	}

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->SetPos(m_pos);	// �ʒu
	m_pModel->SetRot(m_rot);	// ��]
	m_pModel->Update();			// �X�V

	return false;
}

// �`��
void Player::Draw()
{
	// ����ł��Ȃ��ꍇ
	if (!m_isPlayerDeadEffect)
	{
		// �v���C���[���f���̕`��
		m_pModel->Draw();

#ifdef _DEBUG
		// �v���C���[�̓����蔻��̕`��
		DrawSphere3D(m_pos.ToDxLibVector3(), m_collisionRadius, 8, 0xff0000, 0xff0000, false);

		// �v���C���[�̈ʒu���̕`��
		Debug::Text::AddLog("PlayerPos", { m_pos.x, m_pos.y, m_pos.z});
#endif 
	}
}

// �V�[���h�̕`��
void Player::DrawShield()
{
	// �V�[���h������ꍇ
	// �v���C���[������ł���ꍇ�͕`�悵�Ȃ�
	if (m_pShield && !m_isPlayerDeadEffect)
	{
		// �V�[���h�̕`��
		SetWriteZBuffer3D(false);
		m_pShield->Draw();
		SetWriteZBuffer3D(true);
	}
}

// �O���t�@�C������p�����[�^��ǂݍ���
void Player::LoadParameter(const std::string& fileName)
{
	// �t�@�C���̓ǂݍ���
	auto data = FileUtil::LoadCsvFile(fileName);

	// �l�̊i�[
	for (auto& line : data)
	{
		std::string key;
		for (size_t i = 0; i < line.size(); i++)
		{
			// 1��ڂ͐����������Ă���̂Ŕ�΂�
			if (i == 0) continue;

			if (i == 1)
			{
				// �L�[�̐ݒ�
				key = line[i];
			}
			else
			{
				// �p�����[�^�̐ݒ�
				m_parameterTable[key] = std::stof(line[i]);
			}
		}
	}
}

// �_���[�W����
void Player::OnDamage(const int damage)
{
	// HP�����炷
	// 0�ȉ��ɂ͂Ȃ�Ȃ�
	m_hp = (std::max)(m_hp - damage, 0);

	// HP�o�[�̒l�̐ݒ�
	m_pHPbar->SetValue(m_hp);

	if (m_onDamageSEWaitFrame++ % 3 == 0)
	{
		// �_���[�W���̍Đ�
		Sound::Manager::GetInstance()->PlaySE("PlayerDamage");

		m_onDamageSEWaitFrame = 0;
	}

	// �G�t�F�N�g�̍Đ�
	Effect::Effekseer3DManager::GetInstance()->PlayEffect(
		m_damageEffectHandle,
		"EnemyAttackHitEffect",
		{ m_pos.x, m_pos.y, m_pos.z },
		{ 25.0f, 25.0f, 25.0f },
		1.0f);
}

// �V�[���h�����˂������̏���
void Player::OnReflect()
{
	// ���[�U�[�������₷���悤�ɕs�����x��������
	m_opacity = 0.1f;
	m_pModel->SetOpacity(m_opacity);	
}

// �v���C���[�����X�ɓ����ɂ���
void Player::FadeOut()
{
	// �s�����x��������
	m_opacity -= 0.01f;
	m_opacity = (std::max)(m_opacity, 0.0f);

	// ���f���̐ݒ�
	m_pModel->SetOpacity(m_opacity);	// �s�����x
	m_pModel->Update();					// �X�V
}

// �v���C���[�������Ă��邩
bool Player::IsEnabled() const
{
	return (m_hp > 0) ? true : false;
}

// �ʒu���̎擾
const Vector3& Player::GetPos() const
{
	return m_pos;
}

// �ړ��x�N�g���̎擾
const Vector3& Player::GetMoveVec() const
{
	return m_moveVec;
}

// �v���C���[�̓����蔻��̔��a�̎擾
float Player::GetCollisionRadius() const
{
	return m_collisionRadius;
}

// �v���C���[���f���̃n���h���̎擾
int Player::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

 // �X�^�[�g���o���������t���O�̎擾
bool Player::IsStartAnimation() const
{
	return m_isStartAnimation;
}

// �V�[���h�̃C���X�^���X�̎擾
const std::shared_ptr<Shield>& Player::GetShield() const
{
	return m_pShield;
}

// ���߂�ꂽ�t���[���̐������ʒu����ۑ�����e�[�u���̎擾
const std::deque<Vector3>& Player::GetPosLogTable() const
{
	return m_posLogTable;
}

// �p�����[�^�[�̎擾
float Player::GetParameter(const DataType::PlayerParamType type)
{
	// �f�[�^����L�[���擾
	auto& key = DataType::PlayerParamKey[static_cast<int>(type)];

	// �L�[�����݂��邩�m�F
	if (m_parameterTable.find(key) != m_parameterTable.end())
	{
		// ���݂���ꍇ�͒l��Ԃ�
		return m_parameterTable.at(key);
	}

	// �L�[�����݂��Ȃ��ꍇ
	// �G���[���b�Z�[�W���o��
	assert(!"Player�N���X�̃p�����[�^��key�����݂��܂���");
	return -1;
}

// �J�����̃|�C���^�̐ݒ�
void Player::SetCameraPointer(const std::shared_ptr<Camera>& pCamera)
{
	m_pCamera = pCamera;
}
