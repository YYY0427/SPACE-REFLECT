#include "SoundManager.h"
#include "SaveData.h"
#include "Util/FileUtil.h"
#include <cassert>

namespace
{
	// CSVサウンドデータのファイルパス
	const std::string sound_data_csv_file_path = "Data/Csv/Sound.csv";

	// サウンドリソースSEのファイルパス
	const std::string se2d_resourse_file_path = "Data/Sound/SE/2D/";
	const std::string se3d_resourse_file_path = "Data/Sound/SE/3D/";

	// サウンドリソースBGMのファイルパス
	const std::string bgm_resourse_file_path = "Data/Sound/BGM/";

	// 設定する音量を何個に分けるか
	constexpr int config_volume_num = 5;

	// 最大音量
	constexpr int max_volume = 255;
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
	// 唯一の実態参照を返す
	static SoundManager instance;
	return instance;
}

// 更新
void SoundManager::Update()
{
	for (auto& sound : m_soundDataTable)
	{
		// フェード中か
		if (sound.second.fadeData.isFade)
		{
			// フェードフレームを進める
			sound.second.fadeData.currentFadeFrame++;

			// 設定したフレーム数に達したらフェード終了
			if (sound.second.fadeData.currentFadeFrame >= sound.second.fadeData.fadeFrame)
			{
				// 初期化
				sound.second.fadeData.isFade = false;
				sound.second.fadeData.currentFadeFrame = 0;

				// 音量を設定した音量に設定
				SetVolume(sound.first, sound.second.fadeData.afterVolume);
			}
			// フェード中
			else
			{
				// フェード中の音量を設定
				int volume = sound.second.fadeData.initVolume + 
							 (sound.second.fadeData.afterVolume - sound.second.fadeData.initVolume) * 
							 sound.second.fadeData.currentFadeFrame / sound.second.fadeData.fadeFrame;
				// 音量を設定
				SetVolume(sound.first, volume);
			}
		}
		// 音量が0以下になったら止める	
		if (sound.second.volume <= 0)
		{
			StopSound(sound.first);
		}
		// オプションなどで音量が変更される可能性があるので音量を再設定
		SetVolume(sound.first, sound.second.volume);
	}
}

// 終了処理
void SoundManager::End()
{
	// 全てのサウンドを止める
	StopAllSound();

	// サウンドの解放
	for (auto& sound : m_soundDataTable)
	{
		DeleteSoundMem(sound.second.handle);
	}
}

// 2Dサウンドのロード
void SoundManager::LoadSoundFile2D(const std::string& fileName, const std::string& extension, SoundType type)
{
	// サウンドタイプによってそれぞれのファイルパスを設定
	std::string path;
	if (type == SoundType::BGM)
	{
		path = bgm_resourse_file_path;
	}
	else if(type == SoundType::SE2D)
	{
		path = se2d_resourse_file_path;
	}
	else
	{
		// 2Dじゃないので止める
		assert(!"サウンドタイプが無効です");
	}

	// ファイルパスの作成
	path += fileName;
	path += extension;

	// サウンドのロード
	int handle = LoadSoundMem(path.c_str());
	assert(handle != -1);

	// データテーブルに格納
	m_soundDataTable[fileName].handle = handle;
}

// 3Dサウンドのロード
void SoundManager::LoadSoundFile3D(const std::string& fileName, const std::string& extension)
{
	// ファイルパスの作成
	std::string path = se3d_resourse_file_path;
	path += fileName;
	path += extension;

	// 3Dサウンドのロード
	SetCreate3DSoundFlag(TRUE);
	int handle = LoadSoundMem(path.c_str());
	SetCreate3DSoundFlag(FALSE);
	assert(handle != -1);

	// データテーブルに格納
	m_soundDataTable[fileName].handle = handle;
}

// サウンドのCSVファイルを読み込む
void SoundManager::LoadCsvSoundFile()
{
	// ファイル情報の読み込み(読み込みに失敗したら止める)
	auto str = FileUtil::LoadCsvFile(sound_data_csv_file_path);
	for (auto& data : str)
	{
		// csvデータから取得したデータからフォントハンドルの作成して格納
		SoundData soundData;
		soundData.handle = -1;	

		// string型からfloat型に変換し格納
		soundData.volumeRate = std::stof(data[static_cast<int>(SoundFileDataType::VOLUM_RATE)]);

		// string型で格納
		soundData.extension = data[static_cast<int>(SoundFileDataType::EXTENSION)];				

		// サウンドタイプの保存
		// 変換したデータをファイル名をIDとして格納
		// サウンドのタイプによってそれぞれロード
		int iSoundType = std::stoi(data[static_cast<int>(SoundFileDataType::SOUND_TYPE)]);	// string型からint型に変換
		SoundType soundType = static_cast<SoundType>(iSoundType);							// int型からSoundType型に変換
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
			// あり得ない値なので止める
			assert(!"サウンドタイプが無効です");
			break;
		}

		// 初期化
		soundData.volume = max_volume;
		soundData.fadeData.isFade = false;
	}
}

// 指定の2DSEを鳴らす
void SoundManager::PlaySE(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());			

	// 2DSE以外の場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::SE2D);					

	// サウンドの再生
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);

	// 音量を最大に設定
	m_soundDataTable[fileName].volume = max_volume;
	SetVolume(fileName, m_soundDataTable[fileName].volume);
}

// 指定の2DSEをループ再生
void SoundManager::PlaySELoop(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());			

	// 2DSE以外の場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::SE2D);					

	// サウンドの再生
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_LOOP);

	// 音量を最大に設定
	m_soundDataTable[fileName].volume = max_volume;
	SetVolume(fileName, m_soundDataTable[fileName].volume);
}

// 指定の3DSEを鳴らす
void SoundManager::PlaySE3D(const std::string& fileName, const Vector3& soundPos, const float soundRadius)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());			

	// 3DSE以外の場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::SE3D);					

	// 再生する位置の設定
	Set3DPositionSoundMem(soundPos.ToDxLibVector3(), m_soundDataTable[fileName].handle);

	// 再生する半径の設定
	Set3DRadiusSoundMem(soundRadius, m_soundDataTable[fileName].handle);

	// サウンドの再生
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_BACK);

	// 音量を最大に設定
	m_soundDataTable[fileName].volume = max_volume;
	SetVolume(fileName, m_soundDataTable[fileName].volume);
}

// 指定のBGMを鳴らす
void SoundManager::PlayBGM(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	

	// BGM以外の場合は止める
	assert(m_soundDataTable[fileName].type == SoundType::BGM);					

	// サウンドの再生
	PlaySoundMem(m_soundDataTable[fileName].handle, DX_PLAYTYPE_LOOP);

	// 音量を最大に設定
	m_soundDataTable[fileName].volume = max_volume;
	SetVolume(fileName, m_soundDataTable[fileName].volume);
}

// 特定のサウンドが再生中かチェック
bool SoundManager::IsPlaySound(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	

	// 再生中かチェック
	bool sound = CheckSoundMem(m_soundDataTable[fileName].handle);
	return sound;
}

// 特定のサウンドを止める
void SoundManager::StopSound(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());	

	// サウンドの停止
	StopSoundMem(m_soundDataTable[fileName].handle);
}

// すべてのサウンドを止める
void SoundManager::StopAllSound()
{
	// 全てのサウンドを止める
	for (auto& sound : m_soundDataTable)
	{
		StopSoundMem(sound.second.handle);
	}
}

// BGMを止める
void SoundManager::StopBGM()
{
	// BGMを止める
	for (auto& sound : m_soundDataTable)
	{
		if (sound.second.type == SoundType::BGM)
		{
			StopSoundMem(sound.second.handle);
		}
	}
}

// 音量調節
void SoundManager::SetVolume(const std::string& fileName, const int volume)
{
	// サウンドに設定された音量調節
	int setVolume = volume;
	m_soundDataTable[fileName].volume = setVolume;
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

// 音のフェードインの設定
void SoundManager::SetFadeSound(const std::string& fileName, const int fadeFrame, const int initVolume, const int afterVolume)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());		

	// 音量を初期音量に設定
	SetVolume(fileName, initVolume);

	// 初期化
	m_soundDataTable[fileName].fadeData.isFade = true;
	m_soundDataTable[fileName].fadeData.currentFadeFrame = 0;
	m_soundDataTable[fileName].fadeData.initVolume = initVolume;
	m_soundDataTable[fileName].fadeData.afterVolume = afterVolume;
	m_soundDataTable[fileName].fadeData.fadeFrame = fadeFrame;
}

// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
void SoundManager::Set3DSoundListenerPosAndFrontPos_UpVecY(const Vector3& pos, const Vector3& angle)
{
	DxLib::Set3DSoundListenerPosAndFrontPos_UpVecY(pos.ToDxLibVector3(), (pos + angle).ToDxLibVector3());
}

// サウンドのループ範囲を設定
void SoundManager::SetLoopAreaTimePos(const std::string& fileName, LONGLONG startTime, LONGLONG endTime)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

	//// サウンドの総再生時間を取得
	//LONGLONG time = GetSoundTotalTime(m_soundDataTable[fileName].handle);

	//// 総再生時間からstarTime(0.0～1.0)とendTime(0.0～1.0)を設定
	//startTime = time * startTime;
	//endTime = time * endTime;

	// ループ範囲を設定
	int result = SetLoopAreaTimePosSoundMem(startTime, endTime, m_soundDataTable[fileName].handle);
	assert(result != -1 && "サウンドのループ範囲の設定に失敗しました");
}

// サウンドの再生時間を取得
LONGLONG SoundManager::GetSoundTotalTime(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

	// 再生時間を取得
	LONGLONG time = DxLib::GetSoundTotalTime(m_soundDataTable[fileName].handle);
	return time;
}

// サウンドの音量を取得
int SoundManager::GetSoundVolume(const std::string& fileName)
{
	// ロードしていない場合は止める
	assert(m_soundDataTable.find(fileName) != m_soundDataTable.end());

	// 音量を取得
	return m_soundDataTable[fileName].volume;
}

// 再生中のBGMのファイル名の取得
std::string SoundManager::GetPlayBGMFileName() const
{
	// 再生中のBGMのファイル名を取得
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

// BGMが再生中か
bool SoundManager::IsPlayBGM() const
{
	// BGMが再生中か
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

// 最大の音量を取得
int SoundManager::GetMaxVolume() const
{
	return max_volume;
}
