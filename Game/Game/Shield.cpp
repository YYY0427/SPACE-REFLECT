#include "Shield.h"
#include "Player.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Game/Image3D.h"
#include "../UI/Gauge.h"
#include "../UI/UIManager.h"
#include "../UI/ImageUI.h"
#include "../Input/InputManager.h"
#include "../Util/Range.h"
#include "../Util/Timer.h"
#include "../Math/MathUtil.h"
#include "../Math/Matrix.h"
#include "../Math/Vector2.h"
#include <string>

namespace
{
	// �V�[���h�摜�̃t�@�C���p�X
	const std::string img_file_path = "Data/Image/Shield1.png";	

	// �V�[���h�摜�̃T�C�Y
	constexpr float shield_width = 110.0f;	// �����T�C�Y
	constexpr float shield_height = 110.0f;	// �c���T�C�Y

	// �V�[���h�̃G�t�F�N�g�̊g�嗦
	const Math::Vector3 effect_scale = { 80.0f,  80.0f, 80.0f, };

	// �G�l���M�[�Q�[�WUI�̉摜�̃t�@�C���p�X
	const std::string energy_gage_ui_file_path = "Data/Image/EnerugyBar.png";	

	// �G�l���M�[�Q�[�WUI�̔w�i�摜�̃t�@�C���p�X
	const std::string energy_gage_ui_back_file_path = "Data/Image/StatusBase.png";

	// �G�l���M�[�Q�[�WUI�̈ʒu
	const Math::Vector2 energy_gage_ui_pos = { 215, 660 };

	// �G�l���M�[�Q�[�WUI�̃T�C�Y
	const Math::Vector2 energy_gage_ui_size = { 300, 13 };

	// �G�l���M�[�����̃t�@�C���p�X
	const std::string energy_string_file_path = "Data/Image/stamina.png";

	// �G�l���M�[�����̈ʒu
	const Math::Vector2 energy_string_pos = { 70, 620 };

	// �G�l���M�[�����̊g�嗦
	const Math::Vector2 energy_string_scale = { 0.8f, 0.8f };

	// �ő�G�l���M�[�Q�[�W��
	constexpr int max_energy_gage = 500;

	// �v���C���[����̃V�[���h�̋���
	constexpr float player_distance = 100.0f;

	// �G�l���M�[�̉񕜑��x
	constexpr int energy_recovery_speed = 3;

	// �G�l���M�[�̌��鑬�x
	constexpr int energy_decrease_speed = 1;

	// �V�[���h�̊g�k�̑��x
	constexpr float scale_speed = 0.1f;

	// �V�[���h�̊g�k�̑傫��
	constexpr float scale_size = 3.0f;
}

// �R���X�g���N�^
Shield::Shield(Player& player) :
	m_player(player),
	m_isInput(false),
	m_effectHandle(-1),
	m_enerugyGage(max_energy_gage),
	m_scale({ shield_width, shield_height }),
	m_sinFrame(0),
	m_alpha(255)
{
	// 3D�摜�̃C���X�^���X�̍쐬
	m_pImage = std::make_shared<Image3D>(img_file_path);

	// �G�l���M�[�Q�[�WUI�̃C���X�^���X�̍쐬
	m_pEnergyGage = std::make_shared<Gauge>(
		energy_gage_ui_file_path,
		energy_gage_ui_back_file_path,
		"",
		max_energy_gage,
		energy_gage_ui_pos,
		energy_gage_ui_size,
		120);

	// �G�l���M�[�Q�[�WUI�̕����̍쐬
	auto pEnergyString = std::make_shared<ImageUI>(energy_string_file_path);
	pEnergyString->SetPos(energy_string_pos);
	pEnergyString->SetScale(energy_string_scale);

	// UI�̓o�^
	UI::Manager::GetInstance()->AddUI("EnergyGage", m_pEnergyGage, 0, { 0, 1 });
	UI::Manager::GetInstance()->AddUI("EnergyString", pEnergyString, 0, { 0, 1 });

	// �V�[���h�摜�̏�����
	m_pImage->SetPos(m_player.GetPos());		 // �ʒu	
	m_pImage->SetImgWidth(m_scale.x);		 // ����
	m_pImage->SetImgHeight(m_scale.y);		 // �c��
}

// �f�X�g���N�^
Shield::~Shield()
{
}

// �X�V
void Shield::Update()
{
	// HP�������Ă������o���Ȃ�Ȃɂ����Ȃ�
	if (!m_pEnergyGage->IsEndEntranceAnim()) return;

	// �v���C���[�������Ă��Ȃ��Ȃ�Ȃɂ����Ȃ�
	if (!m_player.IsEnabled()) return;

	// ������
	const auto& effectManager = Effect::Effekseer3DManager::GetInstance();
	effectManager->DeleteEffect(m_effectHandle);

	m_isInput = false;
	const Range<int> enerugyGageRange(0, max_energy_gage);

	// ���͂���Ă��邩
	if (Input::Manager::IsPressed(Input::Type::SHIELD))
	{
		// ���͂���Ă���
		m_isInput = true;
	}

	// �E�X�e�B�b�N�̓��͏��̎擾
	int up = Input::Manager::IsPadStick(Input::PadLR::RIGHT, Input::PadStickInputType::UP);
	int down = Input::Manager::IsPadStick(Input::PadLR::RIGHT, Input::PadStickInputType::DOWN);
	int right = Input::Manager::IsPadStick(Input::PadLR::RIGHT, Input::PadStickInputType::RIGHT);
	int left = Input::Manager::IsPadStick(Input::PadLR::RIGHT, Input::PadStickInputType::LEFT);

	// �V�[���h�̈ʒu�̌v�Z
	Math::Vector3 tempVec = { (right + -left) * 10.0f, (up + -down) * 10.0f, player_distance };

	// �v���C���[�̕��s�ړ��s��̎擾
	auto playerMtx = Math::Matrix::GetTranslate(m_player.GetPos());

	// �V�[���h�̑��Έʒu�ƃv���C���[�̕��s�s�񂩂�ʒu�����쐬
	m_pos = Math::Vector3::Transform(tempVec, playerMtx);

	// �x�N�g������p�x�����߂�
	m_rot = -Math::Matrix::ToEulerAngle(Math::Matrix::GetRotationMatrix({ 0, 0, 1 }, tempVec));

	// ���͂���Ă�����
	if (m_isInput)
	{
		// �G�l���M�[�Q�[�W���c���Ă�����
		if (m_enerugyGage > 0)
		{
			// �V�[���h���o���Ă���Ԃ͏�ɃG�l���M�[�Q�[�W�����炷
			m_enerugyGage -= energy_decrease_speed;

			// �G�l���M�[�Q�[�W�̎c�ʂ�3����؂�����
			if (m_enerugyGage < max_energy_gage * 0.35f)
			{
				// �V�[���h��_��
				m_alpha = (0.5f * sinf(m_sinFrame * 0.5f) + 0.5f) * 255.0f;
			}
			else
			{
				m_alpha = 255;
			}
		}
	}
	else
	{
		// ���͂���Ă��Ȃ��Ȃ�G�l���M�[�Q�[�W���񕜂�����
		m_enerugyGage += energy_recovery_speed;
	}

	// �G�l���M�[�Q�[�W�͈̔͂𐧌�
	m_enerugyGage = enerugyGageRange.Clamp(m_enerugyGage);

	// �V�[���h�̊g�k
	m_sinFrame++;
	m_scale.x = shield_width + (sinf(m_sinFrame * scale_speed) * scale_size);
	m_scale.y = shield_height + (sinf(m_sinFrame * scale_speed) * scale_size);

	// �摜�̐ݒ�
	m_pImage->SetAlpha(m_alpha);			 // �����x
	m_pImage->SetImgWidth(m_scale.x);		 // ����
	m_pImage->SetImgHeight(m_scale.y);		 // �c��
	m_pImage->SetPos(m_pos); // �ʒu
	m_pImage->SetRot(m_rot); // ��]
	m_pImage->Update();		 // �X�V

	// �G�l���M�[�Q�[�W�̐ݒ�
	m_pEnergyGage->SetValue(m_enerugyGage);
}

// �`��
void Shield::Draw()
{
	// �v���C���[�������Ă�����
	// �V�[���h���o���Ă�����
	if (IsShield() && m_player.IsEnabled())
	{
		m_pImage->Draw();
	}
}

// �ʒu�̎擾
const Math::Vector3& Shield::GetPos() const
{
	return m_pos;
}

// �V�[���h���g�p���Ă��邩
bool Shield::IsShield() const
{
	return (m_enerugyGage > 0) && (m_isInput);
}

// �V�[���h�̒��_���̎擾
std::array<VERTEX3D, 6> Shield::GetVertex() const
{
	return m_pImage->GetVertex();
}