#include "SkyDome.h"
#include "../Model.h"
#include "../ModelHandleManager.h"
#include <string>

namespace
{
	// ���f���̊g�嗦
	const Math::Vector3 model_scale = { 150, 150, 150 };
}

// �R���X�g���N�^
SkyDome::SkyDome(const Math::Vector3& pos) :
	m_pos(pos)
{
	// ���f���̃C���X�^���X�̍쐬
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("SkyDome"));

	// ���f���̈ʒu��ݒ�
	m_pModel->SetPos(m_pos);

	// ���f���̉�]��ݒ�
	m_pModel->SetRot({ 0, 0, 0 });

	// ���f���̊g�嗦��ݒ�
	m_pModel->SetScale(model_scale);
}

// �f�X�g���N�^
SkyDome::~SkyDome()
{
}

// �X�V
void SkyDome::Update(const Math::Vector3& pos)
{
	m_pos = pos;
	m_pModel->SetPos(m_pos);
}

// �`��
void SkyDome::Draw()
{
	m_pModel->Draw();
}