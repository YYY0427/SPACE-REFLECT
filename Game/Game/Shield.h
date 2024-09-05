#pragma once
#include <memory>
#include <DxLib.h>
#include <array>
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

// �v���g�^�C�v�錾
class Player;
class Image3D;
class Gauge;

/// <summary>
/// �V�[���h�N���X
/// </summary>
class Shield
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	Shield(Player& player);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Shield();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ʒu�̎擾
	/// </summary>
	/// <returns>�ʒu</returns>
	const Vector3& GetPos() const;

	/// <summary>
	/// �V�[���h���g�p���Ă��邩�̎擾
	/// </summary>
	/// <returns></returns>
	bool IsShield() const;

	/// <summary>
	/// �V�[���h�̒��_���̎擾 
	/// </summary>
	/// <returns>���_���</returns>
	std::array<VERTEX3D, 6> GetVertex() const;

private:
	// �|�C���^
	std::shared_ptr<Image3D> m_pImage;
	std::shared_ptr<Gauge> m_pEnergyGage;

	// �v���C���[�̎Q��
	Player& m_player;

	// �V�[���h�̃G�t�F�N�g�n���h��
	int m_effectHandle;

	// �G�l���M�[�Q�[�W
	int m_enerugyGage;

	// �V�[���h�̈ʒu
	Vector3 m_pos;

	// �V�[���h�̉�]
	Vector3 m_rot;

	// �V�[���h�̊g�嗦
	Math::Vector2 m_scale;

	// �v���C���[����̑��Έʒu
	Vector3 m_relativePosToPlayer;

	// ���͂��ꂽ��
	bool m_isInput;

	// sin�J�[�u�̃t���[��
	int m_sinFrame;

	// �A���t�@�l
	int m_alpha;
};