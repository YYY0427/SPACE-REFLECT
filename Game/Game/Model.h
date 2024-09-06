#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include "../Math/Vector3.h"
#include "../Math/Matrix.h"

// TODO : �����蔻��֌W�̊֐��̒ǉ�

/// <summary>
/// ���f���̊Ǘ��N���X
/// </summary>
class Model
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// �t�@�C�������w�肵�ă��[�h���s��
	/// </summary>
	/// <param name="fileName">���f���n���h���̃t�@�C����</param>
	Model(const std::string& fileName);		

	/// <summary>
	/// �R���X�g���N�^
	/// �w�肳�ꂽ�n���h���̃��f�����R�s�[���Đ�������
	/// </summary>
	/// <param name="orgModel">���f���n���h��</param>
	Model(const int modelHandle);		

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Model();

	/// <summary>
	/// �����蔻��ݒ�
	/// </summary>
	/// <param name="isUse">�����蔻������悤���邩</param>
	/// <param name="isNeedUpdate">�X�V���K�v���ǂ���</param>
	void SetUseCollision(const bool isUse, const bool isNeedUpdate = true);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
	/// </summary>
	/// <param name="animNo">�ύX��A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
	///	<param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
	void SetAnimation(const int animNo, const bool isLoop, const bool isForceChange, const float animPlaySpeed = 1.0f);

	/// <summary>
	/// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">�A�j���[�V���������[�v�����邩</param>
	/// <param name="isForceChange">���łɎw�肳�ꂽ�A�j�����Đ�����Ă���ꍇ���ύX���邩</param>
	/// <param name="changeFrame">���t���[�������ăA�j���[�V������ύX�����邩</param>
	void ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame, const float animPlaySpeed = 1.0f);

	/// <summary>
	/// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����
	/// Loop�A�j���̏ꍇ�͎擾�ł��Ȃ�(false��Ԃ�)
	/// </summary>
	/// <returns>true : �I���Afalse : �Đ���</returns>
	bool IsAnimEnd();

	/// <summary>
	/// �A�j���[�V�������~�߂� 
	/// </summary>
	void StopAnim();


	//// �Q�b�^�[ ////

	/// <summary>
	/// ���f���n���h���̎擾
	/// </summary>
	/// <returns>���f���n���h��</returns>
	int GetModelHandle() const;		

	/// <summary>
	/// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾
	/// </summary>
	/// <returns>�t���[���C���f�b�N�X</returns>
	int GetColFrameIndex() const;	

	/// <summary>
	/// ���f���̃}�e���A���̐��̎擾
	/// </summary>
	/// <returns>�}�e���A���̐�</returns>
	int GetMaterialNum() const;		


	//// �Z�b�^�[ ////

	/// <summary>
	/// ���f���̈ʒu��ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	void SetPos(const Math::Vector3& pos);

	/// <summary>
	/// ���f���̉�]��ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRot(const Math::Vector3& rot);

	/// <summary>
	/// ���f���̊g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetRotMtx(const Math::Matrix& mtx);		

	/// <summary>
	/// ���f���̊g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Math::Vector3& scale);

	/// <summary>
	/// ���f���̕s�����x�̐ݒ�(0.0~1.0)
	/// </summary>
	/// <param name="opacity">�s�����x</param>
	void SetOpacity(const float opacity);	


	//// �`�惂�[�h�ݒ� ////

	/// <summary>
	/// �w��̃}�e���A���̕`�惂�[�h�̐ݒ�
	/// </summary>
	/// <param name="materialIndex">�}�e���A���̃C���f�b�N�X</param>
	/// <param name="blendMode">�u�����h���[�h</param>
	void SetMaterialDrawBlendMode(const int materialIndex, const int blendMode);

	/// <summary>
	/// �S�Ẵ}�e���A���̕`�惂�[�h�̐ݒ�
	/// </summary>
	/// <param name="blendMode">�u�����h���[�h</param>
	void SetAllMaterialDrawBlendMode(const int blendMode);					

	/// <summary>
	/// �w��̃}�e���A���̕`��u�����h�p�����[�^�̐ݒ�
	/// </summary>
	/// <param name="materialIndex">�}�e���A���̃C���f�b�N�X</param>
	/// <param name="blendParam">�u�����h�̃p�����[�^�[</param>
	void SetMaterialDrawBlendParam(const int materialIndex, const int blendParam);	

	/// <summary>
	/// �S�Ẵ}�e���A���̕`��u�����h�p�����[�^�̐ݒ�
	/// </summary>
	/// <param name="blendParam">�u�����h�̃p�����[�^�[</param>
	void SetAllMaterialDrawBlendParam(const int blendParam);				


	//// �f�B�t���[�Y�J���[�ݒ� ////

	/// <summary>
	/// �w��̃}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
	/// </summary>
	/// <param name="materialIndex">�}�e���A���̃C���f�b�N�X</param>
	/// <param name="color">�f�B�t���[�Y�J���[</param>
	void SetMaterialDifColor(const int materialIndex, const COLOR_F color);	

	/// <summary>
	/// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
	/// </summary>
	/// <param name="color">�f�B�t���[�Y�J���[</param>
	void SetAllMaterialDifColor(const COLOR_F color);					

	/// <summary>
	/// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�𔽓]
	/// </summary>
	void InversAllMaterialDifColor();				
	
	/// <summary>
	/// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�����ɖ߂�
	/// </summary>
	void RestoreAllMaterialDifColor();					 

	/// <summary>
	/// �S�Ẵ}�e���A���̃A�E�g���C����ݒ肷�� 
	/// </summary>
	/// <param name="color">�A�E�g���C���̐F</param>
	/// <param name="size">�A�E�g���C���̑���</param>
	void SetAllMaterialOutLine(const COLOR_F color, const float size);	

private:
	// �A�j���[�V�������
	struct AnimData
	{
		int animNo;			// �A�j���[�V�����ԍ�
		int attachNo;		// �A�^�b�`�ԍ�
		float totalTime;	// �A�j���[�V�����̑��Đ�����
		float playSpeed;	// �A�j���[�V�����̍Đ����x
		bool isLoop;		// �A�j���[�V���������[�v���邩
	};

private:
	/// <summary>
	/// �A�j���[�V�����f�[�^�̏�����
	/// </summary>
	/// <param name="anim">�A�j���[�V�����̃A�^�b�`�ԍ�</param>
	void InitAnimData(AnimData& anim);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="anim">�A�j���[�V�����̃A�^�b�`�ԍ�</param>
	void UpdateAnim(const AnimData& anim);

	/// <summary>
	/// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷�� 
	/// </summary>
	void UpdateAnimBlendRate();

private:
	// ���f���̃n���h��
	int m_modelHandle;

	// �����蔻������g�p����
	bool m_isUseCollision;

	// �����蔻����𖈃t���[���X�V����
	bool m_isUpdateColision;

	// �����蔻��Ƃ��Ďg�p����t���[���̃C���f�b�N�X
	int m_colFrameIndex;

	// �A�j���[�V�����̃A�^�b�`�ԍ�
	AnimData m_animPrev;		// �ύX�O�A�j���[�V�������
	AnimData m_animNext;		// �ύX��A�j���[�V�������

	// �A�j���[�V�����̐؂�ւ����
	int m_animChangeFrame;			// ���݂̐؂�ւ��t���[����
	int m_animChangeFrameTotal;		// �؂�ւ��ɂ����鑍�t���[����

	// �S�Ă̌��̃}�e���A���̃f�B�t���[�Y�J���[��ۑ����Ă���e�[�u��
	std::vector<COLOR_F> m_matarialColorTable;
};