#include "SoundManager.h"
#include "../SaveData.h"
#include "../Util/FileUtil.h"
#include <cassert>

namespace
{
	// CSV�T�E���h�f�[�^�̃t�@�C���p�X
	const std::string sound_data_csv_file_path = "Data/Csv/Sound.csv";

	// �T�E���h���\�[�XSE�̃t�@�C���p�X
	const std::string se2d_resourse_file_path = "Data/Sound/SE/2D/";
	const std::string se3d_resourse_file_path = "Data/Sound/SE/3D/";

	// �T�E���h���\�[�XBGM�̃t�@�C���p�X
	const std::string bgm_resourse_file_path = "Data/Sound/BGM/";

	// �ݒ肷�鉹�ʂ����ɕ����邩
	constexpr int config_volume_num = 5;

	// �ő剹��
	constexpr int max_volume = 255;
}

namespace Sound
{
	// �R���X�g���N�^
	Manager::Manager()
	{
	}

	// �f�X�g���N�^
	Manager::~Manager()
	{
	}

	// �X�V
	void Manager::Update()
	{
		for (auto& sound : m_soundDataTable)
		{
			// �t�F�[�h����
			if (sound.second.fadeData.isFade)
			{
				// �t�F�[�h�t���[����i�߂�
				sound.second.fadeData.currentFadeFrame++;

				// �ݒ肵���t���[�����ɒB������t�F�[�h�I��
				if (sound.second.fadeData.currentFadeFrame >= sound.second.fadeData.fadeFrame)
				{
					// ������
					sound.second.fadeData.isFade = false;
					sound.second.fadeData.currentFadeFrame = 0;

					// ���ʂ�ݒ肵�����ʂɐݒ�
					SetVolume(sound.first, sound.second.fadeData.afterVolume);
				}
				// �t�F�[�h��
				else
				{
					// �t�F�[�h���̉��ʂ�ݒ�
					int volume = sound.second.fadeData.initVolume +
						(sound.second.fadeData.afterVolume - sound.second.fadeData.initVolume) *
						sound.second.fadeData.currentFadeFrame / sound.second.fadeData.fadeFrame;
					// ���ʂ�ݒ�
					SetVolume(sound.first, volume);
				}
			}
			// ���ʂ�0�ȉ��ɂȂ�����~�߂�	
			if (sound.second.volume <= 0)
			{
				StopSound(sound.first);
			}
			// �I�v�V�����Ȃǂŉ��ʂ��ύX�����\��������̂ŉ��ʂ��Đݒ�
			SetVolume(sound.first, sound.second.volume);
		}
	}

	// �I������
	void Manager::End()
	{
		// �S�ẴT�E���h���~�߂�
		StopAllSound();

		// �T�E���h�̉��
		for (auto& sound : m_soundDataTable)
		{
			DeleteSoundMem(sound.second.handle);
		}
	}

	// 2D�T�E���h�̃��[�h
	void Manager::LoadSoundFile2D(const std::string& fileName, const std::string& extension, SoundType type)
	{
		// �T�E���h�^�C�v�ɂ���Ă��ꂼ��̃t�@�C���p�X��ݒ�
		std::string path;
		if (type == SoundType::BGM)
		{
			path = bgm_resourse_file_path;
		}
		else if (type == SoundType::SE2D)
		{
			path = se2d_resourse_file_path;
		}
		else
		{
			// 2D����Ȃ��̂Ŏ~�߂�
			assert(!"�T�E���h�^�C�v�������ł�");
		}

		// �t�@�C���p�X�̍쐬
		path += fileName;
		path += extension;

		// �T�E���h�̃��[�h
		int handle = LoadSoundMem(path.c_str());
		assert(handle != -1);

		// �f�[�^�e�[�u���Ɋi�[
		m_soundDataTable[fileName].handle = handle;
	}

	// 3D�T�E���h�̃��[�h
	void Manager::LoadSoundFile3D(const std::string& fileName, const std::string& extension)
	{
		// �t�@�C���p�X�̍쐬
		std::string path = se3d_resourse_file_path;
		path += fileName;
		path += extension;

		// 3D�T�E���h�̃��[�h
		SetCreate3DSoundFlag(TRUE);
		int handle = LoadSoundMem(path.c_str());
		SetCreate3DSoundFlag(FALSE);
		assert(handle != -1);

		// �f�[�^�e�[�u���Ɋi�[
		m_soundDataTable[fileName].handle = handle;
	}

	// �T�E���h��CSV�t�@�C����ǂݍ���
	void Manager::LoadCsvSoundFile()
	{
		// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
		auto str = FileUtil::LoadCsvFile(sound_data_csv_file_path);
		for (auto& data : str)
		{
			// csv�f�[�^����擾�����f�[�^����t�H���g�n���h���̍쐬���Ċi�[
			SoundData soundData;
			soundData.handle = -1;

			// string�^����float�^�ɕϊ����i�[
			soundData.volumeRate = std::stof(data[static_cast<int>(SoundFileDataType::VOLUM_RATE)]);

			// string�^�Ŋi�[
			soundData.extension = data[static_cast<int>(SoundFileDataType::EXTENSION)];

			// �T�E���h�^�C�v�̕ۑ�
			// �ϊ������f�[�^���t�@�C������ID�Ƃ��Ċi�[
			// �T�E���h�̃^�C�v�ɂ���Ă��ꂼ�ꃍ�[�h
			int iSoundType = std::stoi(data[static_cast<int>(SoundFileDataType::SOUND_TYPE)]);	// string�^����int�^�ɕϊ�
			SoundType soundType = static_cast<SoundType>(iSoundType);							// int�^����SoundType�^�ɕϊ�
			switch (soundType)
			{
			case SoundType::BGM:
				soundData.type = SoundType::BGM;
				m_soundDataTable[data[static_cast<int>(SoundFileDataType::FILE_NAME)]] = soundData;
				LoadSoundFile2D(data[static_cast<int>(SoundFileDataType::FILE_NAME)], soundData.extension, soundType);
				break;
			case SoundType::SE2D:
				soundData.type = SoundType::SE2D;
				m_soundDataTable[data[static_cast<int>(SoundFileDataType::FILE_NAME)]] = soundData;
				LoadSoundFile2D(data[static_cast<int>(SoundFileDataType::FILE_NAME)], soundData.extension, soundType);
				break;
			case SoundType::SE3D:
				soundData.type = SoundType::SE3D;
				m_soundDataTable[data[static_cast<int>(SoundFileDataType::FILE_NAME)]] = soundData;
				LoadSoundFile3D(data[static_cast<int>(SoundFileDataType::FILE_NAME)], soundData.extension);
				break;
			default:
				// ���蓾�Ȃ��l�Ȃ̂Ŏ~�߂�
				assert(!"�T�E���h�^�C�v�������ł�");
				break;
			}

			// ������
			soundData.volume = max_volume;
			soundData.fadeData.isFade = false;
		}
	}

	// �w���2DSE��炷
	void Manager::PlaySE(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// 2DSE�ȊO�̏ꍇ�͎~�߂�
		assert(m_soundDataTable[fileName].type == SoundType::SE2D);

		// �T�E���h�̍Đ�
		PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);

		// ���ʂ��ő�ɐݒ�
		m_soundDataTable[fileName].volume = max_volume;
		SetVolume(fileName, m_soundDataTable[fileName].volume);
	}

	// �w���2DSE�����[�v�Đ�
	void Manager::PlaySELoop(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// 2DSE�ȊO�̏ꍇ�͎~�߂�
		assert(m_soundDataTable[fileName].type == SoundType::SE2D);

		// �T�E���h�̍Đ�
		PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_LOOP);

		// ���ʂ��ő�ɐݒ�
		m_soundDataTable[fileName].volume = max_volume;
		SetVolume(fileName, m_soundDataTable[fileName].volume);
	}

	// �w���3DSE��炷
	void Manager::PlaySE3D(const std::string& fileName, const Vector3& soundPos, const float soundRadius)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// 3DSE�ȊO�̏ꍇ�͎~�߂�
		assert(m_soundDataTable[fileName].type == SoundType::SE3D);

		// �Đ�����ʒu�̐ݒ�
		Set3DPositionSoundMem(soundPos.ToDxLibVector3(), m_soundDataTable[fileName].handle);

		// �Đ����锼�a�̐ݒ�
		Set3DRadiusSoundMem(soundRadius, m_soundDataTable[fileName].handle);

		// �T�E���h�̍Đ�
		PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);

		// ���ʂ��ő�ɐݒ�
		m_soundDataTable[fileName].volume = max_volume;
		SetVolume(fileName, m_soundDataTable[fileName].volume);
	}

	// �w���BGM��炷
	void Manager::PlayBGM(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// BGM�ȊO�̏ꍇ�͎~�߂�
		assert(m_soundDataTable[fileName].type == SoundType::BGM);

		// �T�E���h�̍Đ�
		PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_LOOP);

		// ���ʂ��ő�ɐݒ�
		m_soundDataTable[fileName].volume = max_volume;
		SetVolume(fileName, m_soundDataTable[fileName].volume);
	}

	// ����̃T�E���h���Đ������`�F�b�N
	bool Manager::IsPlaySound(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// �Đ������`�F�b�N
		bool sound = CheckSoundMem(m_soundDataTable[fileName].handle);
		return sound;
	}

	// ����̃T�E���h���~�߂�
	void Manager::StopSound(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// �T�E���h�̒�~
		StopSoundMem(m_soundDataTable[fileName].handle);
	}

	// ���ׂẴT�E���h���~�߂�
	void Manager::StopAllSound()
	{
		// �S�ẴT�E���h���~�߂�
		for (auto& sound : m_soundDataTable)
		{
			StopSoundMem(sound.second.handle);
		}
	}

	// BGM���~�߂�
	void Manager::StopBGM()
	{
		// BGM���~�߂�
		for (auto& sound : m_soundDataTable)
		{
			if (sound.second.type == SoundType::BGM)
			{
				StopSoundMem(sound.second.handle);
			}
		}
	}

	// ���ʒ���
	void Manager::SetVolume(const std::string& fileName, const int volume)
	{
		// �T�E���h�ɐݒ肳�ꂽ���ʒ���
		int setVolume = volume;
		m_soundDataTable[fileName].volume = setVolume;
		setVolume = static_cast<int>(volume * m_soundDataTable[fileName].volumeRate);

		// �R���t�B�O�Őݒ肵���T�E���h�̑S�̉��ʒ���
		int configWholeVolume = SaveData::GetInstance().GetSaveData().masterVolume;
		float configWholeRate = static_cast<float>(configWholeVolume) / config_volume_num;

		// �R���t�B�O�Őݒ肵���T�E���h�^�C�v�ʉ��ʒ���
		int configVolume = 0;
		if (m_soundDataTable[fileName].type == SoundType::BGM)
		{
			// BGM
			configVolume = SaveData::GetInstance().GetSaveData().bgmVolume;
		}
		else
		{
			// SE
			configVolume = SaveData::GetInstance().GetSaveData().seVolume;
		}

		// �ݒ肵�������ʂƃT�E���h�ɐݒ肳�ꂽ���ʂƃR���t�B�O�Őݒ肳�ꂽ���ʂ��狁�߂��ŏI�I�ȉ��ʂɐݒ�
		float configRate = static_cast<float>(configVolume) / config_volume_num;
		setVolume = static_cast<int>(setVolume * configRate * configWholeRate);
		ChangeVolumeSoundMem(setVolume, m_soundDataTable[fileName].handle);
	}

	// ���̃t�F�[�h�C���̐ݒ�
	void Manager::SetFadeSound(const std::string& fileName, const int fadeFrame, const int initVolume, const int afterVolume)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// ���ʂ��������ʂɐݒ�
		SetVolume(fileName, initVolume);

		// ������
		m_soundDataTable[fileName].fadeData.isFade = true;
		m_soundDataTable[fileName].fadeData.currentFadeFrame = 0;
		m_soundDataTable[fileName].fadeData.initVolume = initVolume;
		m_soundDataTable[fileName].fadeData.afterVolume = afterVolume;
		m_soundDataTable[fileName].fadeData.fadeFrame = fadeFrame;
	}

	// 3D�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu��ݒ肷��
	void Manager::Set3DSoundListenerPosAndFrontPos_UpVecY(const Vector3& pos, const Vector3& angle)
	{
		DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(pos.ToDxLibVector3(), (pos + angle).ToDxLibVector3());
	}

	// �T�E���h�̃��[�v�͈͂�ݒ�
	void Manager::SetLoopAreaTimePos(const std::string& fileName, LONGLONG startTime, LONGLONG endTime)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		//// �T�E���h�̑��Đ����Ԃ��擾
		//LONGLONG time = GetSoundTotalTime(m_soundDataTable[fileName].handle);

		//// ���Đ����Ԃ���starTime(0.0�`1.0)��endTime(0.0�`1.0)��ݒ�
		//startTime = time * startTime;
		//endTime = time * endTime;

		// ���[�v�͈͂�ݒ�
		int result = SetLoopAreaTimePosSoundMem(startTime, endTime, m_soundDataTable[fileName].handle);
		assert(result != -1 && "�T�E���h�̃��[�v�͈͂̐ݒ�Ɏ��s���܂���");
	}

	// �T�E���h�̍Đ����Ԃ��擾
	LONGLONG Manager::GetSoundTotalTime(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// �Đ����Ԃ��擾
		LONGLONG time = DxLib::GetSoundTotalTime(m_soundDataTable[fileName].handle);
		return time;
	}

	// �T�E���h�̉��ʂ��擾
	int Manager::GetSoundVolume(const std::string& fileName)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

		// ���ʂ��擾
		return m_soundDataTable[fileName].volume;
	}

	// �Đ�����BGM�̃t�@�C�����̎擾
	std::string Manager::GetPlayBGMFileName() const
	{
		// �Đ�����BGM�̃t�@�C�������擾
		for (auto& sound : m_soundDataTable)
		{
			if (CheckSoundMem(sound.second.handle))
			{
				if (sound.second.type == SoundType::BGM)
				{
					return sound.first;
				}
			}
		}
		return "";
	}

	// BGM���Đ�����
	bool Manager::IsPlayBGM() const
	{
		// BGM���Đ�����
		for (auto& sound : m_soundDataTable)
		{
			if (CheckSoundMem(sound.second.handle))
			{
				if (sound.second.type == SoundType::BGM)
				{
					return true;
				}
			}
		}
		return false;
	}

	// �ő�̉��ʂ��擾
	int Manager::GetMaxVolume() const
	{
		return max_volume;
	}

}
