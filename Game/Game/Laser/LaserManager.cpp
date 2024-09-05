#include "LaserManager.h"
#include "LaserBase.h"
#include "NormalLaser.h"
#include "../../Util/DrawFunctions.h"
#include "../Shield.h"
#include "ReflectLaser.h"
#include "CubeLaser.h"
#include "../Player.h"
#include "../Shield.h"
#include "../../Application.h"
#include <cassert>
#include <string>

// �R���X�g���N�^
LaserManager::LaserManager(const std::shared_ptr<Player>& pPlayer, const std::shared_ptr<Camera>& pCamera) :
	m_pPlayer(pPlayer),
	m_pCamera(pCamera)
{
}

// �f�X�g���N�^
LaserManager::~LaserManager()
{
}

// �X�V
void LaserManager::Update()
{
	// ���݃t���O���������Ă���̃��[�U�[���폜
	m_pLaserList.remove_if([](const LaserData& laser) { return !laser.pLaser->IsEnabled(); });

	// ���[�U�[�̍X�V
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Update();
	}
}

// �`��
void LaserManager::Draw()
{
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Draw();
	}
}

// ���[�U�[�̒ǉ�
int LaserManager::AddLaser(LaserType type, 
						   const std::shared_ptr<EnemyBase>& pEnemy, 
						   const int laserChargeFrame, 
						   const int laserFireFrame, 
						   const float laserSpeed, 
						   const bool isPlayerFollowing)
{
	// ���[�U�[�̃f�[�^���쐬
	LaserData laserData;

	// ���[�U�[�̎�ނ�ݒ�
	laserData.type = LaserType::NORMAL;

	// ���[�U�[�̃C���X�^���X���쐬
	laserData.pLaser = std::make_shared<NormalLaser>(
		pEnemy, m_pPlayer, laserChargeFrame, laserFireFrame, laserSpeed, isPlayerFollowing);

	// Key�̐ݒ�
	laserData.key = CreateLaserKey();

	// ���[�U�[���X�g�ɒǉ�
	m_pLaserList.push_back(laserData);

	// Key��Ԃ�
	return laserData.key;
}

// ���˃��[�U�[�̒ǉ�
int LaserManager::AddReflectLaser(const std::shared_ptr<EnemyManager>& pEnemyManager, 
								  const std::shared_ptr<Shield>& pShield, 
								  const std::shared_ptr<LaserBase>& pLaser, 
								  const Math::Vector3& firePos)
{
	// ���[�U�[�̃f�[�^���쐬
	LaserData laserData;
	laserData.type = LaserType::REFLECT;

	// Key�̐ݒ�
	laserData.key = CreateLaserKey();

	// ���[�U�[�̃|�C���^��ݒ�
	laserData.pLaser = std::make_shared<ReflectLaser>(pEnemyManager, pShield, pLaser, firePos);

	// ���[�U�[���X�g�ɒǉ�
	m_pLaserList.push_back(laserData);

	return laserData.key;
}

// �L���[�u���[�U�[�̒ǉ�
int LaserManager::AddCubeLaser(const Math::Vector3& firePos, const float laserSpeed)
{
	// ���[�U�[�̃f�[�^���쐬
	LaserData laserData;
	laserData.type = LaserType::CUBE;

	// Key�̐ݒ�
	laserData.key = CreateLaserKey();

	// ���[�U�[�̃|�C���^��ݒ�
	laserData.pLaser = std::make_shared<CubeLaser>(firePos, laserSpeed, m_pPlayer, m_pCamera);

	// ���[�U�[���X�g�ɒǉ�
	m_pLaserList.push_back(laserData);

	// Key��Ԃ�
	return laserData.key;
}

// ���[�U�[�̍폜
void LaserManager::DeleteLaser(const int key)
{
	// �L�[�ƈ�v���郌�[�U�[���폜
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->Delete();
		}
	}
}

// �S�Ẵ��[�U�[�̍폜
void LaserManager::DeleteAllLaser()
{
	// ���[�U�[�̍폜
	for (auto& laser : m_pLaserList)
	{
		laser.pLaser->Delete();
	}
	m_pLaserList.clear();
}

// ���[�U�[�̎擾	
const std::list<LaserData>& LaserManager::GetLaserList() const
{
	return m_pLaserList;
}

// ���[�U�[�̎擾
LaserData LaserManager::GetLaserData(const int key) const
{
	// �L�[�ƈ�v���郌�[�U�[���擾
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			return laser;
		}
	}

	// �G���[
	assert(!"���[�U�[��������܂���ł���");
	return m_pLaserList.front();
}

// ���[�U�[�̈ʒu�̐ݒ�
void LaserManager::SetLaserPos(const int key, const Math::Vector3& pos)
{
	// �L�[�ƈ�v���郌�[�U�[�Ɉʒu��ݒ�
	for (auto& laser : m_pLaserList)
	{
		if (laser.key == key)
		{
			laser.pLaser->SetPos(pos);
		}
	}
}

// ���[�U�[�̃L�[�̍쐬
int LaserManager::CreateLaserKey()
{
	// �L�[�̏d��������邽�߂ɁA�ő�̃L�[��+1������
	int key = 0;
	for (auto& laser : m_pLaserList)
	{
		if (key <= laser.key)
		{
			key = laser.key + 1;
		}
	}
	return key;
}