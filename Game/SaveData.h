#pragma once

// プロトタイプ宣言
class SoundManager;

/// <summary>
/// セーブデータの管理を行うシングルトンクラス
/// </summary>
class SaveData
{
private:
	// セーブデータ
	struct Data
	{
		int version = 0;		// データのバージョン
		int masterVolume = 5;	// 全体の音量
		int bgmVolume = 5;		// BGM音量	
		int seVolume = 5;		// SE音量	
		int padStickSensitivityX = 6;	// パッドのスティックの感度X
		int padStickSensitivityY = 3;	// パッドのスティックの感度Y
		bool padStickReverseX = false;	// パッドのスティックの横のリバース
		bool padStickReverseY = false;	// パッドのスティックの縦のリバース
		bool windowMode = false;		// ウィンドウモードの切り替え	
	};
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SaveData();

	/// <summary>
	/// 唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static SaveData& GetInstance();

	/// <summary>
	/// セーブデータの読み込み 
	/// </summary>
	void Load();

	/// <summary>
	/// セーブデータの書き込み 
	/// セーブデータのバージョンが一致しない場合は止める
	/// </summary>
	void Write();

	/// <summary>
	/// セーブデータを新規作成して上書き 
	/// </summary>
	void CreateNewData();

	/// <summary>
	/// セーブデータの取得
	/// </summary>
	/// <returns>セーブデータ</returns>
	const Data& GetSaveData() const;

	/// <summary>
	/// コンフィグの値の設定
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="configValue">設定したい値</param>
	/// <param name="splitNum">分割数</param>
	template<class T> void SetConfigValue(T& configValue, int splitNum);

	/// <summary>
	/// コンフィグのスイッチの設定
	/// </summary>
	/// <param name="config">設定したいスイッチ</param>
	void SetConfigSwitch(bool& config);

	/// <summary>
	/// 全体音量の設定
	/// 音量の設定(最大値を超えると0に戻る)
	/// </summary>
	/// <param name="configNum">音量の分割数</param>
	void SetMasterVolume(const int configNum);	

	/// <summary>
	/// BGMの音量の設定
	/// 音量の設定(最大値を超えると0に戻る)
	/// </summary>
	/// <param name="configNum">音量の分割数</param>
	void SetBgmVolume(const int configNum);	

	/// <summary>
	/// SEの音量の設定
	/// 音量の設定(最大値を超えると0に戻る)
	/// </summary>
	/// <param name="configNum">音量の分割数</param>
	void SetSeVolume(const int configNum);		

	/// <summary>
	/// パッドのスティックのX感度の設定 
	/// 感度の設定(最大値を超えると0に戻る)
	/// </summary>
	/// <param name="configNum">分割数</param>
	void SetPadStickSensitivityX(const int configNum);

	/// <summary>
	/// パッドのスティックのY感度の設定 
	/// 感度の設定(最大値を超えると0に戻る)
	/// </summary>
	/// <param name="configNum">分割数</param>
	void SetPadStickSensitivityY(const int configNum);

	/// <summary>
	/// パッドのスティックXのリバースの設定 
	/// </summary>
	void SetPadStickReverseX();

	/// <summary>
	/// パッドのスティックYのリバースの設定 
	/// </summary>
	void SetPadStickReverseY();

	/// <summary>
	/// ウィンドウモードの切り替え 
	/// </summary>
	void SetWindowMode();

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなのでprivate
	/// </summary>
	SaveData();

	// コピーと代入の禁止
	SaveData(const SaveData&) = delete;			// コピーコンストラクタ禁止
	void operator = (const SaveData&) = delete;	// 代入禁止

	/// <summary>
	/// データの削除 
	/// </summary>
	void InitData();

private:
	// セーブデータ
	Data m_saveData;

	// サウンドマネージャー
	SoundManager& m_soundManger;
};