#pragma once
#include "Math/Vector3.h"
#include <map>
#include <string>
#include <DxLib.h>

/// <summary>
/// サウンドの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
private:
	// サウンドの種類
	enum class SoundType
	{
		BGM,
		SE2D,
		SE3D,
	};

	// サウンドのデータの種類
	enum class SoundFileDataType
	{
		FILE_NAME,	// ファイル名
		EXTENSION,	// 拡張子
		SOUND_TYPE,	// サウンドの種類
		VOLUM_RATE,	// 音量の内部レート
	};

private:
	// サウンドフェードデータ
	struct SoundFadeData
	{
		int  currentFadeFrame;	// 現在のフェードフレーム
		int  fadeFrame;			// フェードのフレーム数
		int  initVolume;		// 初期音量
		int  afterVolume;		// 最終的な音量
		bool isFade;			// フェード中か
	};

	// サウンドのデータ
	struct SoundData
	{
		SoundFadeData fadeData;		// フェードデータ
		SoundType     type;			// BGMか3DのSEか2DのSEか
		std::string   extension;	// サウンドファイルの拡張子
		float		  volumeRate;	// ボリューム調整
		int 		  volume;		// ボリューム
		int			  handle;		// ハンドル
	};

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SoundManager();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static SoundManager& GetInstance();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// サウンドのCSVファイルを読み込む
	/// </summary>
	void LoadCsvSoundFile();

	/// <summary>
	/// 指定の2DSEを鳴らす
	/// サウンドをロードされていない場合、2DSE以外の場合は止まる
	/// </summary>
	/// <param name="name">ファイル名</param>
	void Play(const std::string& fileName);

	/// <summary>
	/// 指定の3DSEを鳴らす
	/// サウンドをロードされていない場合、3DSE以外の場合は止まる
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="soundPos">再生位置</param>
	/// <param name="soundRadius">聞こえる距離</param>
	void Play3D(const std::string& fileName, const Vector3& soundPos, const float soundRadius);

	/// <summary>
	/// 指定のBGMを鳴らす
	/// サウンドをロードされていない場合、BGM以外の場合は止まる
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void PlayBGM(const std::string& fileName);

	/// <summary>
	/// 特定のサウンドが再生中かチェック
	/// サウンドがロードされていなかったら止める
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool IsPlaySound(const std::string& fileName);

	/// <summary>
	/// 特定のサウンドを止める
	/// サウンドがロードされていなかったら止める
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void StopSound(const std::string& fileName);

	/// <summary>
	/// すべてのサウンドを止める
	/// </summary>
	void StopAllSound();

	/// <summary>
	/// 音量調節
	/// 設定したい音量とサウンドに設定された音量とコンフィグで設定された音量から求めた最終的な音量に調節
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="volume">設定したい音量(0~255)</param>
	void SetVolume(const std::string& fileName, const int volume);

	/// <summary>
	/// サウンドフェードの設定
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="fadeFrame">フレーム数</param>
	/// <param name="initVolume">初期音量</param>
	/// <param name="afterVolume">最終音量</param>
	void SetFadeSound(const std::string& fileName, const int fadeFrame, const int initVolume, const int afterVolume);

	/// <summary>
	/// 3Dサウンドのリスナーの位置とリスナーの前方位置を設定する
	/// </summary>
	/// <param name="pos">リスナーの座標</param>
	/// <param name="angle">リスナーの向いている角度(ラジアン)</param>
	void Set3DSoundListenerPosAndFrontPos_UpVecY(const Vector3& pos, const Vector3& angle);

	/// <summary>
	/// サウンドの音量を取得
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>サウンド音量</returns>
	int GetSoundVolume(const std::string& fileName);

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなのでprivate</summary>
	SoundManager();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name="soundManager">コピー元</param>
	SoundManager(const SoundManager&) = delete;		

	/// <summary>
	/// 代入演算子禁止	
	/// </summary>
	/// <param name="soundManager">代入元</param>
	void operator = (const SoundManager&) = delete;

	/// <summary>
	/// 2Dサウンドのロード
	/// ロードに失敗したら止める
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="extension">拡張子</param>
	void LoadSoundFile2D(const std::string& fileName, const std::string& extension, SoundType type);

	/// <summary>
	/// 3Dサウンドのロード
	/// ロードに失敗したら止める
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="extension">拡張子</param>
	void LoadSoundFile3D(const std::string& fileName, const std::string& extension);

private:
	// ロードしたサウンドのファイル名をIDとしたサウンドデータのテーブル
	std::map<std::string, SoundData> m_soundDataTable;
};