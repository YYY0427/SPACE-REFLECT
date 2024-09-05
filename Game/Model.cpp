#include "Model.h"
#include <cassert>

namespace
{
	// �����蔻��Ƃ��Ďg�p����t���[���̖��O
	const std::string collision_frame_name = "Collision";
}

// �t�@�C�������w�肵�ă��[�h���s��
Model::Model(const std::string& fileName) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	// ���f���̃��[�h
	m_modelHandle = MV1LoadModel(fileName.c_str());

	// ���f���̃��[�h�Ɏ��s����������~�߂�
	assert(m_modelHandle != -1);

	// �A�j���[�V�������̏�����
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);

	// ���f���̃}�e���A���̃f�B�t���[�Y�J���[��ۑ�
	for (int i = 0; i < GetMaterialNum(); i++)
	{
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);
		m_matarialColorTable.push_back(color);
	}
}

// �w�肳�ꂽ�n���h���̃��f�����R�s�[���Đ�������
Model::Model(const int modelHandle) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	// ���f���̃R�s�[
	m_modelHandle = MV1DuplicateModel(modelHandle);

	// ���f���̃R�s�[�Ɏ��s������~�߂�
	assert(m_modelHandle != -1);

	// �A�j���[�V�������̏�����
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);

	// ���f���̃}�e���A���̃f�B�t���[�Y�J���[��ۑ�
	for (int i = 0; i < GetMaterialNum(); i++)
	{
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);
		m_matarialColorTable.push_back(color);
	}
}

// �f�X�g���N�^
Model::~Model()
{
	// �����蔻������g�p���Ă�����폜
	if (m_isUpdateColision)
	{
		MV1TerminateCollInfo(m_modelHandle, -1);
		m_isUseCollision = false;
	}

	// ���f���̍폜
	MV1DeleteModel(m_modelHandle);
}

// �����蔻��̐ݒ�
void Model::SetUseCollision(const bool isUse, const bool isNeedUpdate)
{
	// �����蔻����g��Ȃ��̂ɓ����蔻��̍X�V�𖈃t���[�������Ȃ������ꍇ���͂���
	assert(isUse || !isNeedUpdate);

	// �����蔻��g�p���ɕω����������ꍇ�̂ݍX�V����
	if (m_isUpdateColision != isUse)
	{
		if (isUse)
		{
			// �g��Ȃ����g��
			// �����蔻��p�t���[��������
			m_colFrameIndex = MV1SearchFrame(m_modelHandle, collision_frame_name.c_str());

			// ������Ȃ����� or �G���[
			if (m_colFrameIndex < 0)	
			{
				m_colFrameIndex = -1;
			}

			// �����蔻������\�z����
			MV1SetupCollInfo(m_modelHandle, m_colFrameIndex, 8, 8, 8);
		}
		else
		{
			// �g�����g��Ȃ�
			MV1TerminateCollInfo(m_modelHandle, -1);
		}
	}

	// �ۑ�
	m_isUseCollision = isUse;
	m_isUpdateColision = isNeedUpdate;
}

// �X�V
void Model::Update()
{
	UpdateAnim(m_animPrev);
	UpdateAnim(m_animNext);

	// �w��t���[�������ăA�j���[�V������ύX����
	m_animChangeFrame++;
	if (m_animChangeFrame > m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();

	// �����蔻��f�[�^�̍X�V
	if (m_isUseCollision && m_isUpdateColision)
	{
		MV1RefreshCollInfo(m_modelHandle, m_colFrameIndex);
	}
}

//  �`��
void Model::Draw()
{
	int result = MV1DrawModel(m_modelHandle);
	assert(result != -1);
}

// ���f���̕\���ʒu�̐ݒ�
void Model::SetPos(const Math::Vector3& pos)
{
	int result = MV1SetPosition(m_modelHandle, pos.ToDxLibVector3());
	assert(result != -1);
}

// ���f���̉�]��Ԃ̐ݒ�
void Model::SetRot(const Math::Vector3& rot)
{
	int result = MV1SetRotationXYZ(m_modelHandle, rot.ToDxLibVector3());
	assert(result != -1);
}

// ���f���̉�]�s��̐ݒ�
void Model::SetRotMtx(const Math::Matrix& mtx)
{
	int result = MV1SetRotationMatrix(m_modelHandle, mtx.ToDxLibMatrix());
	assert(result != -1);
}

// ���f���̊g�嗦�̐ݒ�
void Model::SetScale(const Math::Vector3& scale)
{
	int result = MV1SetScale(m_modelHandle, scale.ToDxLibVector3());
	assert(result != -1);
}

// ���f���̕s�����x�̐ݒ�
void Model::SetOpacity(const float opacity)
{
	int result = MV1SetOpacityRate(m_modelHandle, opacity);
	assert(result != -1);
}

// ���f���̎w��̃}�e���A���̕`��u�����h���[�h��ݒ�
void Model::SetMaterialDrawBlendMode(const int materialIndex, const int blendMode)
{
	int result = MV1SetMaterialDrawBlendMode(m_modelHandle, materialIndex, blendMode);
	assert(result != -1);
}

// ���f���̑S�Ẵ}�e���A���̕`�惂�[�h�̐ݒ�
void Model::SetAllMaterialDrawBlendMode(const int blendMode)
{
	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	// �}�e���A���̐��̕��A�`�惂�[�h�̐ݒ�
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDrawBlendMode(i, blendMode);
	}
}

// �w��̃}�e���A���̕`��u�����h�p�����[�^�̐ݒ�
void Model::SetMaterialDrawBlendParam(const int materialIndex, const int blendParam)
{
	int result = MV1SetMaterialDrawBlendParam(m_modelHandle, materialIndex, blendParam);
	assert(result != -1);
}

// �S�Ẵ}�e���A���̕`��u�����h�p�����[�^�̐ݒ�
void Model::SetAllMaterialDrawBlendParam(const int blendParam)
{
	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	// �}�e���A���̐��̕��A�`��u�����h�p�����[�^�̐ݒ�
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDrawBlendParam(i, blendParam);
	}
}

// �w��̃}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
void Model::SetMaterialDifColor(const int materialIndex, const COLOR_F color)
{
	int result = MV1SetMaterialDifColor(m_modelHandle, materialIndex, color);
	assert(result != -1);
}

// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�̐ݒ�
void Model::SetAllMaterialDifColor(const COLOR_F color)
{
	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	// �}�e���A���̐��̕��A�`��u�����h�p�����[�^�̐ݒ�
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDifColor(i, color);
	}
}

// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�𔽓]
void Model::InversAllMaterialDifColor()
{
	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	for (int i = 0; i < materialNum; i++)
	{
		// �}�e���A���̐F���擾
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);

		// �F�𔽓]
		color = { 1.0f - color.r, 1.0f - color.g, 1.0f - color.b, 1.0f };

		// �}�e���A���ɐF��ݒ�
		SetMaterialDifColor(i, color);
	}
}

// �S�Ẵ}�e���A���̃f�B�t���[�Y�J���[�����ɖ߂�
void Model::RestoreAllMaterialDifColor()
{
	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	// �}�e���A���̐��̕��A�f�B�t���[�Y�J���[�����ɖ߂�
	for (int i = 0; i < materialNum; i++)
	{
		COLOR_F color = m_matarialColorTable[i];
		SetMaterialDifColor(i, color);
	}
}

// �S�Ẵ}�e���A���̃A�E�g���C����ݒ肷��
void Model::SetAllMaterialOutLine(const COLOR_F color, const float size)
{

	/*int result = MV1SetMaterialOutLineWidthAll(m_modelHandle, size);
	assert(result != -1);	

	result = MV1SetMaterialOutLineColorAll(m_modelHandle, color);
	assert(result != -1);*/


	// �}�e���A���̐��̎擾
	int materialNum = GetMaterialNum();

	// �}�e���A���̐��̕��A�A�E�g���C����ݒ肷��
	for (int i = 0; i < materialNum; i++)
	{
		// �}�e���A���̌��X�̃A�E�g���C���̑������擾
		float initWidth = MV1GetMaterialOutLineDotWidth(m_modelHandle, i);

		// �A�E�g���C���̑��������X�̑��� + �w��̑����ɐݒ�
		int result = MV1SetMaterialOutLineDotWidth(m_modelHandle, i, size);
		assert(result != -1);

		// �A�E�g���C���̐F��ݒ�
		result = MV1SetMaterialOutLineColor(m_modelHandle, i, color);
		assert(result != -1);
	}
}

// �A�j���[�V������ݒ肷��(�ς��Ɛ؂�ւ���)
void Model::SetAnimation(const int animNo, const bool isLoop, const bool isForceChange, const float animPlaySpeed)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (m_animNext.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		InitAnimData(m_animPrev);
	}
	if (m_animNext.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animNext.attachNo);
		InitAnimData(m_animNext);
	}

	// �V�����A�j���[�V������ݒ�
	m_animNext.animNo = animNo;
	m_animNext.playSpeed = animPlaySpeed;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

// �A�j���[�V������ω�������(���t���[�������Đ؂�ւ���)
void Model::ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame, const float animPlaySpeed)
{
	if (!isForceChange)
	{
		// ���łɍĐ�����Ă���A�j���͍Đ����Ȃ�
		if (m_animNext.animNo == animNo)	return;
	}

	// �ȑO�̃A�j���[�V�������c���Ă���ΏI��
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		InitAnimData(m_animPrev);
	}

	// ���ݍĐ����Ă���A�j���[�V�������Â��A�j���Ƃ���
	m_animPrev = m_animNext;

	// �V�����A�j���[�V������ݒ�
	m_animNext.animNo = animNo;
	m_animNext.playSpeed = animPlaySpeed;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	// �ύX�ɂ�����t���[�������o���Ă���
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// �A�j���[�V�����̃u�����h����ݒ肷��
	UpdateAnimBlendRate();
}

// ���݂̃A�j���[�V�������I�����Ă��邩�ǂ������擾����
bool Model::IsAnimEnd()
{
	// Loopp�A�j���̏ꍇ��false��Ԃ�
	if (m_animNext.isLoop)
	{
		return false;
	}

	// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ��擾����
	float time = MV1GetAttachAnimTime(m_modelHandle, m_animNext.attachNo);

	// �A�j���[�V�������Đ��������Ԃ��A�j���[�V�����̑����Ԃ𒴂��Ă�����
	// �A�j���[�V�������I�����Ă���̂�true��Ԃ�
	if (time >= m_animNext.totalTime)
	{
		return true;
	}
	return false;
}

void Model::StopAnim()
{
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);
}

// ���f���̃n���h���̎擾
int Model::GetModelHandle() const
{
	return m_modelHandle;		
}

// �����蔻��Ɏg�p����t���[���C���f�b�N�X���擾����
int Model::GetColFrameIndex() const
{
	return m_colFrameIndex;
}

int Model::GetMaterialNum() const
{
	int result = MV1GetMaterialNum(m_modelHandle);
	return result;
}

// �A�j���[�V�����f�[�^�̏�����
void Model::InitAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.playSpeed = 1.0f;
	anim.isLoop = false;
}

// �A�j���[�V�����̍X�V
void Model::UpdateAnim(const AnimData& anim)
{
	// �A�j���[�V�������ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (anim.attachNo == -1) return;

	// �A�j���[�V�����̍X�V
	float time = MV1GetAttachAnimTime(m_modelHandle, anim.attachNo);
	time += anim.playSpeed;

	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			// �A�j���[�V�����̃��[�v
			time -= anim.totalTime;
		}
		else
		{
			// �Ō�̏�ԂŎ~�܂�
			time = anim.totalTime;
		}
	}
	// �A�^�b�`���Ă���A�j���[�V�����̍Đ����Ԃ�ݒ肷��
	MV1SetAttachAnimTime(m_modelHandle, anim.attachNo, time);
}

// ���݂̃A�j���[�V�����؂�ւ���񂩂�A�j���[�V�����̃u�����h����ݒ肷��
void Model::UpdateAnimBlendRate()
{
	// �A�j���[�V�����ω��̃t���[�����ɉ������u�����h����ݒ肷��
	float rate = static_cast<float> (m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animPrev.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animNext.attachNo, rate);
}