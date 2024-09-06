#include "Image3D.h"
#include "../Util/DrawFunctions.h"
#include <cassert>

// �R���X�g���N�^
Image3D::Image3D(const std::string& filePath) :
	m_pos({}),
	m_rot({}),
	m_imgWidth(0),
	m_imgHeight(0),
	m_vertex({}),
	m_alpha(255)
{
	// �摜�̓ǂݍ���
	m_imgHandle = my::MyLoadGraph(filePath.c_str());
}

// �R���X�g���N�^
Image3D::Image3D(const int imgHandle) :
	m_imgHandle(imgHandle),
	m_pos({}),
	m_rot({}),
	m_imgWidth(0),
	m_imgHeight(0),
	m_vertex({}),
	m_alpha(255)
{
	// �摜�n���h���������Ȃ�~�߂�
	assert(m_imgHandle != -1);
}

// �f�X�g���N�^
Image3D::~Image3D()
{
	// �摜�̍폜
	DeleteGraph(m_imgHandle);
}

// �X�V
void Image3D::Update()
{
	// �e���_�̏���ݒ�
	m_vertex[0].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[0].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[0].u = 0.0f;
	m_vertex[0].v = 0.0f;
	m_vertex[0].su = 0.0f;
	m_vertex[0].sv = 0.0f;

	m_vertex[1].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[1].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[1].u = 1.0f;
	m_vertex[1].v = 0.0f;
	m_vertex[1].su = 0.0f;
	m_vertex[1].sv = 0.0f;

	m_vertex[2].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[2].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[2].u = 0.0f;
	m_vertex[2].v = 1.0f;
	m_vertex[2].su = 0.0f;
	m_vertex[2].sv = 0.0f;

	m_vertex[3].dif = GetColorU8(255, 255, 255, 255);
	m_vertex[3].spc = GetColorU8(0, 0, 0, 0);
	m_vertex[3].u = 1.0f;
	m_vertex[3].v = 1.0f;
	m_vertex[3].su = 0.0f;
	m_vertex[3].sv = 0.0f;

	m_vertex[4] = m_vertex[2];
	m_vertex[5] = m_vertex[1];

	// ��]( x, y, z����]�̏� )�{���W�ړ��s��̍쐬
	MATRIX transformMatrix;
	transformMatrix = MGetRotX(m_rot.x);
	transformMatrix = MMult(transformMatrix, MGetRotY(m_rot.y));
	transformMatrix = MMult(transformMatrix, MGetRotZ(m_rot.z));
	transformMatrix = MMult(transformMatrix, MGetTranslate(VGet(m_pos.x, m_pos.y, m_pos.z)));

	// �s����g���ă��[���h���W���Z�o
	m_vertex[0].pos = VTransform(VGet(-m_imgWidth, m_imgHeight, 0.0f), transformMatrix);	// ����
	m_vertex[1].pos = VTransform(VGet(m_imgWidth, m_imgHeight, 0.0f), transformMatrix);		// �E��
	m_vertex[2].pos = VTransform(VGet(-m_imgWidth, -m_imgHeight, 0.0f), transformMatrix);	// ����
	m_vertex[3].pos = VTransform(VGet(m_imgWidth, -m_imgHeight, 0.0f), transformMatrix);	// �E��

	m_vertex[4].pos = m_vertex[2].pos;
	m_vertex[5].pos = m_vertex[1].pos;

	// �s����g���Ė@�����Z�o
	m_vertex[0].norm = VTransformSR(VGet(0.0f, 0.0f, -1.0f), transformMatrix);
	m_vertex[1].norm = m_vertex[0].norm;
	m_vertex[2].norm = m_vertex[0].norm;
	m_vertex[3].norm = m_vertex[0].norm;
	m_vertex[4].norm = m_vertex[0].norm;
	m_vertex[5].norm = m_vertex[0].norm;
}

// �`��
void Image3D::Draw()
{
	// �|���S���̗������`�悷��
	SetUseBackCulling(FALSE);

	// ���C�e�B���O�͍s��Ȃ�
	SetUseLighting(FALSE);

	// �����x��ݒ�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);

	// �Q�|���S���̕`��
	DrawPolygon3D(m_vertex.data(), 2, m_imgHandle, true);

	// �����x��߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// ���C�e�B���O�͍s��Ȃ�
	SetUseLighting(TRUE);

	// �|���S���̗����͕`�悵�Ȃ�
	SetUseBackCulling(TRUE);
}

// �摜�n���h���̎擾
int Image3D::GetImageHandle() const
{
	return m_imgHandle;
}

// �摜�̒��_���̎擾
const std::array<VERTEX3D, 6>& Image3D::GetVertex() const
{
	return m_vertex;
}

// �ʒu�̐ݒ�
void Image3D::SetPos(const Math::Vector3& pos)
{
	m_pos = pos;
}

// ��]�̐ݒ�
void Image3D::SetRot(const Math::Vector3& rot)
{
	m_rot = rot;
}

// �摜�̉����̐ݒ�
void Image3D::SetImgWidth(const float width)
{
	m_imgWidth = width;
}

// �摜�̏c���̐ݒ�
void Image3D::SetImgHeight(const float height)
{
	m_imgHeight = height;
}

// �摜�̓����x�̐ݒ�
void Image3D::SetAlpha(const int alpha)
{
	m_alpha = alpha;
}

// �摜�̉����Ɨ����̊g�嗦�����̉摜�̃T�C�Y�ɖ߂�
void Image3D::SetImgDafualtScale()
{
	// �摜�̑傫���ǂ���摜��`�悷��
	GetGraphSizeF(m_imgHandle, &m_imgWidth, &m_imgHeight);
}