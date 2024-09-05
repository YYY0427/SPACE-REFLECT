#pragma once
#include <string>
#include <map>
#include <vector>
#include "../Math/Vector3.h"

// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^
struct UnityGameObject
{
	std::string name;
	Math::Vector3 pos;
	Math::Vector3 rot;
	Math::Vector3 scale;
};

/// <summary>
/// Unity����z�u�f�[�^��ǂݍ��ރN���X
/// </summary>
class DataReaderFromUnity
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DataReaderFromUnity();

	/// <summary>
	/// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ�� 
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	void LoadUnityGameObjectData(const std::string& fileName);

	/// <summary>
	/// �C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static DataReaderFromUnity& GetInstance();

	/// <summary>
	/// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^���擾
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="objectName">�I�u�W�F�N�g��</param>
	/// <returns>�f�[�^</returns>
	std::vector<UnityGameObject> GetData(const std::string& fileName, const std::string& objectName) const;

	/// <summary>
	/// �S�Ẵf�[�^���폜
	/// </summary>
	void DeleteAllData();

private:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	DataReaderFromUnity();

	/// <summary>
	/// �R�s�[�R���X�g���N�^�֎~
	/// </summary>
	/// <param name=""></param>
	DataReaderFromUnity(const DataReaderFromUnity&) = delete;	

	/// <summary>
	/// ������Z�q�֎~
	/// </summary>
	/// <param name=""></param>
	void operator = (const DataReaderFromUnity&) = delete;		

private:
	// �ǂݎ�����f�[�^
	// �����Ƀf�[�^���i�[����
	std::map<std::string, std::map<std::string, std::vector<UnityGameObject>>> m_data;
};