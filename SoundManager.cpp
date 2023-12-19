#include "SoundManager.h"
#include "SaveData.h"
#include "StringManager.h"
#include <cassert>
#include <fstream>
#include <sstream>

namespace
{
	// サウンドのファイルパス
	const std::string data_file_path = "Data/Sound/";

	// 設定する音量を何個に分けるか
	constexpr int config_volume_num = 5;
}

// コンストラクタ
SoundManager::SoundManager()
{
}

// デストラクタ
SoundManager::~SoundManager()
{
}

// 唯一のインスタンスを返す
SoundManager& SoundManager::GetInstance()
{
	// 唯一の実態
	static SoundManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// 2Dサウンドのロード
void SoundManager::LoadSoundFile2D(std::string fileName, std::string extension)
{
	std::string path = data_file_path;
	path += fileName;
	path += extension;
	int handle = LoadSoundMem(path.c_str());
	assert(handle != -1);
	m_soundDataTable[fileName].handle = handle;
}

// 3Dサウンドのロード
void SoundManager::LoadSoundFile3D(std::string fileName, std::string extension)
{
	std::string path = data_file_path;
	path += fileName;
	path += extension;
	SetCreate3DSoundFlag(TRUE);
	int handle = LoadSoundMem(path.c_str());
	SetCreate3DSoundFlag(FALSE);
	assert(handle != -1);
	m_soundDataTable[fileName].handle = handle;
}

// ファイルからサウンドのデータを読み取ってデータテーブルに格納
void SoundManager::LoadAndStoreSoundFileData()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	std::ifstream ifs("Data/Csv/Sound.csv");
	assert(ifs);

	// csvデータを1行ずつ読み取る
	bool isFirst = false;
	std::string line;
	while (getline(ifs, line))
	{
		// 1行目は読み込まない
		// 1行目には項目が書いてあるため
		if (!isFirst)
		{
			isFirst = true;
			continue;
		}

		// csvデータ１行を','で複数の文字列に変換
		std::vector<std::string> strvec = StringManager::GetInstance().SplitString(line, ',');
		
		// 文字列を適切なデータ型に変換して格納
		SoundData data;
		data.handle = -1;	// 初期化
		data.volumeRate = std::stof(strvec[static_cast<int>(SoundDataType::VOLUM_RATE)]);	// string型からfloat型に変換し格納
		data.extension = strvec[static_cast<int>(SoundDataType::EXTENSION)];				// string型で格納
		
		// サウンドタイプの保存
		// 変換したデータをファイル名をIDとして格納
		// サウンドのタイプによってそれぞれロード
		int iSoundType = std::stoi(strvec[static_cast<int>(SoundDataType::SOUND_TYPE)]);	// string型からint型に変換
		SoundType soundType = static_cast<SoundType>(iSoundType);							// int型からSoundType型に変換
		switch (soundType)
		{
		case SoundType::BGM:
			data.type = SoundType::BGM;
			m_soundDataTable[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile2D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		case SoundType::SE2D:
			data.type = SoundType::SE2D;
			m_soundDataTable[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile2D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		case SoundType::SE3D:
			data.type = SoundType::SE3D;
			m_soundDataTable[strvec[static_cast<int>(SoundDataType::FILE_NAME)]] = data;
			LoadSoundFile3D(strvec[static_cast<int>(SoundDataType::FILE_NAME)], data.extension);
			break;
		default:
			// あり得ない値なので止める
			assert(0);
			break;
		}
	}
}

// 指定の2DSEを鳴らす
void SoundManager::Play(std::string fileName)
{
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	// ロードしていない場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::SE2D);					// 2DSE以外の場合は止める
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

// 指定の3DSEを鳴らす
void SoundManager::Play3D(std::string fileName, VECTOR soundPos, float soundRadius)
{
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	// ロードしていない場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::SE3D);					// 3DSE以外の場合は止める
	Set3DPositionSoundMem(soundPos, m_soundDataTable[fileName].handle);
	Set3DRadiusSoundMem(soundRadius, m_soundDataTable[fileName].handle);
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);
	SetVolume(fileName, 255);
}

// 指定のBGMを鳴らす
void SoundManager::PlayBGM(std::string fileName)
{
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	// ロードしていない場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::BGM);					// BGM以外の場合は止める
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_LOOP);
	SetVolume(fileName, 255);
}

// 特定のサウンドが再生中かチェック
bool SoundManager::PlayingCheckSound(std::string fileName)
{
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	// ロードしていない場合は止める
	bool sound = CheckSoundMem(m_soundDataTable[fileName].handle);
	return sound;
}

// 特定のサウンドを止める
void SoundManager::StopSound(std::string fileName)
{
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	// ロードしていない場合は止める
	StopSoundMem(m_soundDataTable[fileName].handle);
}

// すべてのサウンドを止める
void SoundManager::StopAllSound()
{
	for (auto& sound : m_soundDataTable)
	{
		StopSoundMem(sound.second.handle);
	}
}

// 音量調節
void SoundManager::SetVolume(std::string fileName, int volume)
{
	// サウンドに設定された音量調節
	int setVolume = volume;
	setVolume = static_cast<int>(volume * m_soundDataTable[fileName].volumeRate);

	// コンフィグで設定したサウンドの全体音量調節
	int configWholeVolume = SaveData::GetInstance().GetSaveData().masterVolume;
	float configWholeRate = static_cast<float>(configWholeVolume) / config_volume_num;

	// コンフィグで設定したサウンドタイプ別音量調節
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

	// 設定したい音量とサウンドに設定された音量とコンフィグで設定された音量から求めた最終的な音量に設定
	float configRate = static_cast<float>(configVolume) / config_volume_num;
	setVolume = static_cast<int>(setVolume * configRate * configWholeRate);
	ChangeVolumeSoundMem(setVolume, m_soundDataTable[fileName].handle);
}

// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
void SoundManager::Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle)
{
	DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(pos, VAdd(pos, angle));
}
