#include "MeteorManager.h"
#include "../Util/DrawFunctions.h"
#include "../Editor/DataReaderFromUnity.h"
#include <string>

namespace
{
	// ������覐΂̐����Ԋu
	const int small_create_interval_frame = 5;
}

// �R���X�g���N�^
MeteorManager::MeteorManager(const std::string& objectDataFileName, const std::shared_ptr<Player>& pPlayer) :
	m_smallCreateIntervalFrame(0),
	m_pPlayer(pPlayer)
{
	// �z�u�f�[�^�����݂���ꍇ�͔z�u�f�[�^����覐΂𐶐�
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Meteor2");
	for (auto& meteorData : data)
	{
		m_pMeteorList.push_back(std::make_shared<Meteor>(meteorData, m_pPlayer));
	}
}

// �f�X�g���N�^
MeteorManager::~MeteorManager()
{
	// 覐΂̍폜
	m_pMeteorList.clear();
}

// �X�^�[�g���o���̍X�V
void MeteorManager::UpdateStart()
{
	// 覐΂̍X�V
	for (auto& meteor : m_pMeteorList)
	{
		meteor->UpdateStart();
	}
}

// �X�V
void MeteorManager::Update(const Math::Vector3& cameraPos)
{
	// ���݂��Ȃ�覐΂��폜
	m_pMeteorList.remove_if([](std::shared_ptr<Meteor> meteor) { return !meteor->IsEnabled(); });

	// 覐΂̍X�V
	for (auto& meteor : m_pMeteorList)
	{
		meteor->Update(cameraPos);
	}
}

// �`��
void MeteorManager::Draw()
{
	for (auto& meteor : m_pMeteorList)
	{
		meteor->Draw();
	}
}

// ������覐΂̐���
void MeteorManager::CreateSmallMeteor(const float playerRelativePos)
{
	m_smallCreateIntervalFrame++;
	if (small_create_interval_frame < m_smallCreateIntervalFrame)
	{
		// ������覐΂̐���
		m_pMeteorList.push_back(std::make_shared<Meteor>(MeteorType::SMALL, m_pPlayer, playerRelativePos));
		m_smallCreateIntervalFrame = 0;
	}
}

// 覐΂̍폜
void MeteorManager::DeleteAllMeteor()
{
	m_pMeteorList.clear();
}

// 覐΂̎擾
const std::list<std::shared_ptr<Meteor>>& MeteorManager::GetMeteor() const
{
	return m_pMeteorList;
}
