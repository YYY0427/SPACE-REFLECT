#include "SaveData.h"
#include "SoundManager.h"
#include "Util/Range.h"
#include "Util/InputState.h"
#include <string>
#include <cassert>
#include <fstream>
#include <DxLib.h>

namespace
{
	// ファイルのパス
	const std::string se_resourse_file_path = "Data/Save/SaveData.dat";

	// セーブデータバージョン　
	constexpr int current_save_version = 0;
}

// コンストラクタ
SaveData::SaveData() :
	m_soundManger(SoundManager::GetInstance())
{
	
}

// デストラクタ
SaveData::~SaveData()
{
}

// 唯一のインスタンスを返す
SaveData& SaveData::GetInstance()
{
	// 唯一の実態
	static SaveData instance;

	// 唯一の実態参照を返す
	return instance;
}

// セーブデータの読み込み
void SaveData::Load()
{
	// セーブデータの初期化
	InitData();

	std::ifstream ifs;
	ifs.open(se_resourse_file_path, std::ios_base::binary);

	// ファイル読み込み失敗
	if (ifs.fail())
	{
		// セーブデータを作る
		CreateNewData();
		return;
	}
	else
	{
		// セーブデータの読み込み
		Data data;
		ifs.read(reinterpret_cast<char*>(&data), sizeof(Data));
		ifs.close();

		if (data.version == current_save_version)
		{
			// 読み込んだデータを実際に使用するデータにコピー
			m_saveData = data;
		}
		else
		{
			// リリース後にセーブデータのバージョンが変わったら実装
			assert(0);
		}
	}
}

// セーブデータの書き込み
void SaveData::Write()
{
	assert(m_saveData.version == current_save_version);
	std::ofstream ofs(se_resourse_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&m_saveData), sizeof(Data));
}

// セーブデータを新規作成して上書き
void SaveData::CreateNewData()
{
	// セーブデータの初期化
	InitData();

	std::ofstream ofs(se_resourse_file_path, std::ios_base::binary);
	ofs.write(reinterpret_cast<char*>(&m_saveData), sizeof(SaveData));
}

// セーブデータの初期化
void SaveData::InitData()
{
	m_saveData.version = current_save_version;
	m_saveData.bgmVolume = 5;
	m_saveData.seVolume = 5;
	m_saveData.masterVolume = 5;
	m_saveData.padStickSensitivityX = 6;
	m_saveData.padStickSensitivityY = 3;
	m_saveData.padStickReverseX = false;	
	m_saveData.padStickReverseY = false;	
	m_saveData.windowMode = false;
}

// コンフィグの値の設定
template<class T> void SaveData::SetConfigValue(T& configValue, int splitNum)
{
	if (InputState::IsTriggered(InputType::RIGHT))
	{
		configValue++;
	}
	if (InputState::IsTriggered(InputType::LEFT))
	{
		configValue--;
	}

	const Range<T> configValueRange(0, splitNum + 1);

	configValue = configValueRange.Wrap(configValue);

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}

// 全体の音量の設定
// 最大値を超えると0に戻る
void SaveData::SetMasterVolume(const int configNum)
{
	SetConfigValue(m_saveData.masterVolume, configNum);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		m_soundManger.SetVolume("bgmTest", 255);
	}
}

// BGMの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetBgmVolume(const int configNum)
{
	SetConfigValue(m_saveData.bgmVolume, configNum);
}

// SEの音量の設定
// 最大値を超えると0に戻る
void SaveData::SetSeVolume(const int configNum)
{
	SetConfigValue(m_saveData.seVolume, configNum);

	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		m_soundManger.PlaySE("Select");
	}
}

// パッドのスティックのX軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityX(const int configNum)
{
	SetConfigValue(m_saveData.padStickSensitivityX, configNum);
}

// パッドのスティックのY軸感度の設定
// 最大値を超えると0に戻る
void SaveData::SetPadStickSensitivityY(const int configNum)
{
	SetConfigValue(m_saveData.padStickSensitivityY, configNum);
}

// パッドのスティックのX軸リバースの設定
void SaveData::SetPadStickReverseX()
{
	SetConfigSwitch(m_saveData.padStickReverseX);
}

// パッドのスティックのY軸リバースの設定
void SaveData::SetPadStickReverseY()
{
	SetConfigSwitch(m_saveData.padStickReverseY);
}

// ウィンドウモードの設定
void SaveData::SetWindowMode()
{
	SetConfigSwitch(m_saveData.windowMode);
}

// セーブデータの取得
const SaveData::Data& SaveData::GetSaveData() const
{
	return m_saveData;
}

// コンフィグのスイッチの設定
void SaveData::SetConfigSwitch(bool& config)
{
	if (InputState::IsTriggered(InputType::RIGHT) ||
		InputState::IsTriggered(InputType::LEFT))
	{
		if (config)
		{
			config = false;
		}
		else
		{
			config = true;
		}
	}

	// セーブデータに書き込む
	SaveData::GetInstance().Write();
}
