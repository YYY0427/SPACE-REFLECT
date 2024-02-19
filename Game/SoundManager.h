#pragma once
#include <map>
#include <string>
#include <DxLib.h>

/// <summary>
/// サウンドの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
public:
	// デストラクタ
	~SoundManager();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static SoundManager& GetInstance();

	// ファイルからサウンドのデータを読み取ってデータテーブルに格納
	void LoadAndStoreSoundFileData();

	/// <summary>
	/// 指定の2DSEを鳴らす
	/// サウンドをロードされていない場合、2DSE以外の場合は止まる
	/// </summary>
	/// <param name="name">再生したいサウンドのファイル名(拡張子は含まない)</param>
	void Play(std::string fileName);

	/// <summary>
	/// 指定の3DSEを鳴らす
	/// サウンドをロードされていない場合、3DSE以外の場合は止まる
	/// </summary>
	/// <param name="fileName">再生したいサウンドのファイル名</param>
	/// <param name="soundPos">再生位置</param>
	/// <param name="soundRadius">聞こえる距離</param>
	void Play3D(std::string fileName, VECTOR soundPos, float soundRadius);

	/// <summary>
	/// 指定のBGMを鳴らす
	/// サウンドをロードされていない場合、BGM以外の場合は止まる
	/// </summary>
	/// <param name="fileName">再生したいサウンドのファイル名</param>
	void PlayBGM(std::string fileName);

	/// <summary>
	/// 特定のサウンドが再生中かチェック
	/// サウンドがロードされていなかったら止める
	/// </summary>
	/// <param name="fileName">再生しているかチェックしたいサウンドのファイル名</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool PlayingCheckSound(std::string fileName);

	/// <summary>
	/// 特定のサウンドを止める
	/// サウンドがロードされていなかったら止める
	/// </summary>
	/// <param name="fileName">止めたいサウンドのファイル名(拡張子は含まない)</param>
	void StopSound(std::string fileName);

	/// <summary>
	/// すべてのサウンドを止める
	/// </summary>
	void StopAllSound();

	/// <summary>
	/// 音量調節
	/// 設定したい音量とサウンドに設定された音量とコンフィグで設定された音量から求めた最終的な音量に調節
	/// </summary>
	/// <param name="fileName">音量調節をしたサウンドのファイル名(拡張子は含まない)</param>
	/// <param name="volume">設定したい音量(0~255)</param>
	void SetVolume(std::string fileName, int volume);

	/// <summary>
	/// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
	/// </summary>
	/// <param name="pos">リスナーの座標</param>
	/// <param name="angle">リスナーの向いている角度(ラジアン)</param>
	void Set3DSoundListenerPosAndFrontPos_UpVecY(VECTOR pos, VECTOR angle);

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	SoundManager();

	// コピーと代入の禁止
	SoundManager(const SoundManager&) = delete;			// コピーコンストラクタ禁止
	void operator = (const SoundManager&) = delete;		// 代入禁止

	/// <summary>
	/// 2Dサウンドのロード
	/// ロードに失敗したら止める
	/// </summary>
	/// <param name="fileName">ロードしたいサウンドファイル名(拡張子なし)</param>
	/// <param name="extension">ロードしたサウンドの拡張子</param>
	void LoadSoundFile2D(std::string fileName, std::string ext);

	/// <summary>
	/// 3Dサウンドのロード
	/// ロードに失敗したら止める
	/// </summary>
	/// <param name="fileName">ロードしたいサウンドファイル名(拡張子なし)</param>
	/// <param name="extension">ロードしたサウンドの拡張子</param>
	void LoadSoundFile3D(std::string fileName, std::string ext);

private:
	// サウンドの種類
	enum class SoundType
	{
		BGM,
		SE2D,
		SE3D,
	};

	// サウンドのデータの種類
	enum class SoundDataType
	{
		FILE_NAME,
		EXTENSION,
		SOUND_TYPE,
		VOLUM_RATE,
	};

private:
	// サウンドのデータ
	struct SoundData
	{
		SoundType type;			// BGMか3DのSEか2DのSEか
		std::string extension;	// サウンドファイルの拡張子
		float volumeRate;		// ボリューム調整
		int handle;				// ハンドル
	};

private:
	// ロードしたサウンドのファイル名をIDとしたサウンドデータのテーブル
	std::map<std::string, SoundData> m_soundDataTable;
};