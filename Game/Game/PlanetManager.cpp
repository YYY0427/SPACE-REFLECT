#include "PlanetManager.h"
#include "Planet.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../Util/DrawFunctions.h"
#include "../Resource/Model/ModelResourceManager.h"
#include "../Math/MathUtil.h"
#include <string>

namespace
{
	// �X�e�[�W�Z���N�g���̉�]���x
	const Math::Vector3 stage_select_rotate_speed = { 0, Math::Util::ToRadian(0.5f), 0 };

	// �v���C��ʂ̉�]���x
	const Math::Vector3 play_rotate_speed = { 0, Math::Util::ToRadian(0.01f), 0 };
}

// �R���X�g���N�^
PlanetManager::PlanetManager(const std::string& objectDataFileName)
{
	// �C���X�^���X�̎擾
	auto& dataReader = DataReaderFromUnity::GetInstance();

	// �n���̃C���X�^���X�̍쐬
	auto earthData = dataReader.GetData(objectDataFileName, "Earth");
	for (auto& earth : earthData)
	{
		m_planetData[PlanetType::EARTH].modelHandle = Resource::Model::Manager::GetInstance()->GetHandle("Earth");
		m_planetData[PlanetType::EARTH].pPlanet = std::make_shared<Planet>(m_planetData[PlanetType::EARTH].modelHandle, earth);
	}

	// ���̃C���X�^���X�̍쐬
	auto moonData = dataReader.GetData(objectDataFileName, "Moon");
	for (auto& moon : moonData)
	{
		m_planetData[PlanetType::MOON].modelHandle = Resource::Model::Manager::GetInstance()->GetHandle("Moon");
		m_planetData[PlanetType::MOON].pPlanet = std::make_shared<Planet>(m_planetData[PlanetType::MOON].modelHandle, moon);
	}
}

// �f�X�g���N�^
PlanetManager::~PlanetManager()
{
	m_planetData.clear();
}

// �v���C���̍X�V
void PlanetManager::UpdatePlay(const Math::Vector3& playerVec)
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->Update(playerVec, play_rotate_speed);
	}
}

// �X�e�[�W�Z���N�g���̍X�V
void PlanetManager::UpdateStageSelect()
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->UpdateStageSelect(stage_select_rotate_speed);
	}
}

// �`��
void PlanetManager::Draw()
{
	for (auto& planet : m_planetData)
	{
		planet.second.pPlanet->Draw();
	}
}

// �^�C�v����f���̃|�C���^���擾
const std::shared_ptr<Planet>& PlanetManager::GetPlanet(PlanetType type) const
{
	return m_planetData.at(type).pPlanet;
}