#pragma once
#include "../Editor/DataReaderFromUnity.h"
#include <memory>
#include <vector>
#include <map>

// �v���g�^�C�v�錾
 class Planet;

 // �f���̎��
 enum class PlanetType
 {
	 MOON,
	 EARTH,
	 MARS,
	 JUPITER,
	 SATURN,
	 URANUS,
	 NEPTUNE,
	 PLUTO,
	 NUM
 };	

/// <summary>
/// �f�����Ǘ�����N���X
/// </summary>
class PlanetManager
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objectDataFileName">�I�u�W�F�N�g�f�[�^�̃t�@�C����</param>
	PlanetManager(const std::string& objectDataFileName);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlanetManager();

	/// <summary>
	/// �Q�[���v���C���̍X�V
	/// </summary>
	/// <param name="playerVec">�v���C���[�̃x�N�g��</param>
	void UpdatePlay(const Math::Vector3& playerVec);

	/// <summary>
	/// �X�e�[�W�Z���N�g���̍X�V
	/// </summary>
	void UpdateStageSelect();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �f���̎擾
	/// </summary>
	/// <param name="type">�f���̎��</param>
	/// <returns>�f���̃|�C���^</returns>
	const std::shared_ptr<Planet>& GetPlanet(PlanetType type) const;

private:
	// �f���̃f�[�^
	struct PlanetData
	{
		// ���f���n���h��
		int modelHandle = -1;

		// �|�C���^
		std::shared_ptr<Planet> pPlanet;
	};

private:
	// �f���̃f�[�^
	std::map<PlanetType, PlanetData> m_planetData;
};