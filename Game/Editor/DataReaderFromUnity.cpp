#include "DataReaderFromUnity.h"
#include <cassert>
#include "../Math/MathUtil.h"
#include <DxLib.h>

namespace
{
	// �t�@�C���̊K�w
	const std::string file_hierarchy = "Data/Stage/";	

	// �g���q
	const std::string extension = ".dat";
}

// �R���X�g���N�^
DataReaderFromUnity::DataReaderFromUnity()
{
}

// �f�X�g���N�^
DataReaderFromUnity::~DataReaderFromUnity()
{
}

// Unity�Ŕz�u�����I�u�W�F�N�g�̃f�[�^��ǂݎ��
void DataReaderFromUnity::LoadUnityGameObjectData(const std::string& fileName)
{
	// �񓯊��ǂݍ��݂��s��Ȃ�
	// �����œǂݍ��񂾃f�[�^�������g������
	SetUseASyncLoadFlag(false);

	// �t�@�C���p�X�̍쐬
	std::string filePath = file_hierarchy + fileName + extension;

	// �t�@�C���ɃA�N�Z�X
	auto dataHandle = FileRead_open(filePath.c_str());

	// �f�[�^���𓾂�
	int dataNum = 0;
	int result = FileRead_read(&dataNum, sizeof(dataNum), dataHandle);
	assert(result != -1);

	// �f�[�^�̓ǂݎ��
	for (int i = 0; i < dataNum; i++)
	{
		UnityGameObject data;

		// ���O�̕����񐔂̓ǂݎ��
		uint8_t nameSize = 0;
		result = FileRead_read(&nameSize, sizeof(nameSize), dataHandle);
		assert(result != -1);

		// ���O���̂��̂�ǂݎ��
		data.name.resize(nameSize);
		result = FileRead_read(data.name.data(), sizeof(char) * nameSize, dataHandle);
		assert(result != -1);

		// ���W�f�[�^xyz��ǂ�
		result = FileRead_read(&data.pos, sizeof(data.pos), dataHandle);
		assert(result != -1);

		// ��]�f�[�^xyz��ǂ�
		result = FileRead_read(&data.rot, sizeof(data.rot), dataHandle);
		assert(result != -1);

		// �x���@���ʓx�@�ɕϊ�
		data.rot.x = Math::Util::ToRadian(data.rot.x);
		data.rot.y = Math::Util::ToRadian(data.rot.y);
		data.rot.z = Math::Util::ToRadian(data.rot.z);

		// Unity��DxLib�ł͉�]������邽�ߍ��킹��
		data.rot.y += DX_PI_F;

		// �g��f�[�^xyz��ǂ�
		result = FileRead_read(&data.scale, sizeof(data.scale), dataHandle);
		assert(result != -1);

		// �t�@�C�������Ƃɕۑ�
		// �I�u�W�F�N�g�̖��O���Ƃɕۑ�
		m_data[fileName][data.name].push_back(data);
	}

	// �t�@�C�������
	FileRead_close(dataHandle);

	// �񓯊��ǂݍ��݂��s��
	SetUseASyncLoadFlag(true);
}

// �C���X�^���X�̎擾
DataReaderFromUnity& DataReaderFromUnity::GetInstance()
{
	// �B��̃C���X�^���X��Ԃ�
	static DataReaderFromUnity instance;
	return instance;
}

// �I�u�W�F�N�g�̖��O����f�[�^���擾
std::vector<UnityGameObject> DataReaderFromUnity::GetData(const std::string& fileName, const std::string& objectName) const
{
	// �t�@�C��������f�[�^���擾
	auto it = m_data.find(fileName);
	assert(it != m_data.end() && "�t�@�C���f�[�^���Ȃ��ł�");

	// �I�u�W�F�N�g�̖��O����f�[�^���擾
	auto it2 = it->second.find(objectName);
	if (it2 == it->second.end())
	{
		// �f�[�^���Ȃ��ꍇ�͋�̃f�[�^��Ԃ�
		static std::vector<UnityGameObject> empty;
		return empty;
	}

	// �f�[�^��Ԃ�
	return it2->second;
}

// ���ׂẴf�[�^�̍폜
void DataReaderFromUnity::DeleteAllData()
{
	m_data.clear();
}