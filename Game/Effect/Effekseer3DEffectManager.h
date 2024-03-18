#pragma once
#include "../Math/Vector3.h"
#include <string>
#include <map>
#include <list>

/// <summary>
/// Effekseerを使った3Dエフェクトの管理を行うシングルトンクラス
/// </summary>
class Effekseer3DEffectManager
{
private:
	// エフェクトの再生タイプ
	enum class PlayType
	{
		NORMAL,
		FOLLOW,
		LOOP,
		LOOP_AND_FOLLOW,
	};

	// エフェクトのファイルタイプ
	enum class EffectFileType
	{
		ID,			// エフェクトのID
		FILE_PATH,	// エフェクトのファイルパス
	};

private:
	// エフェクトデータ
	struct EffectData
	{
		std::string effectId;				// エフェクトのID
		int* playingEffectHandle;			// エフェクトのハンドル
		PlayType	type;					// 再生タイプ
		Vector3		pos;					// 位置	
		Vector3		rot;					// 回転
		Vector3		scale;					// 拡大率
		float		speed;					// 再生速度
		bool		isZBuffer;				// Zバッファを使うか
	};

	// 追従エフェクトデータ
	struct FollowEffectData
	{
		std::string effectId;				// エフェクトのID
		int*        playingEffectHandle;	// エフェクトのハンドル
		PlayType    type;					// 再生タイプ
		Vector3*    pos;					// 追従したい位置情報変数のアドレス
		Vector3     rot;					// 回転
		Vector3		scale;					// 拡大率
		float		speed;					// 再生速度
		bool		isZBuffer;				// Zバッファを使うか
	};

	// エフェクトリソースデータ
	struct EffectResourceData
	{
		std::string effectFilePath;		// エフェクトのファイルパス
		int         effectHandle;		// エフェクトのハンドル
	};

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Effekseer3DEffectManager();

	/// <summary>
	/// Effekseer3DEffectManagerの唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static Effekseer3DEffectManager& GetInstance();

	// Effekseerの初期化とエフェクトのロード
	// 初期化に失敗したら止める
	void Init();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 指定のエフェクトの再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="id">外部ファイルで設定したID</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffect(int& playingEffectHandle, 
					const std::string& id, 
					const Vector3& pos, 
					const Vector3& scale = { 1.0f, 1.0f, 1.0f }, 
					const float speed = 1.0f, 
					const Vector3& rot = { 0.0f, 0.0f, 0.0f },
					const bool isZbuffer = true);

	/// <summary>
	/// 指定のエフェクトのループ再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="id">外部ファイルで設定したID</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectLoop(int& playingEffectHandle, 
						const std::string& id, 
						const Vector3& pos,
						const Vector3& scale = { 1.0f, 1.0f, 1.0f }, 
						const float speed = 1.0f, 
						const Vector3& rot = { 0.0f, 0.0f, 0.0f },
						const bool isZbuffer = true);

	/// <summary>
	/// 指定のエフェクトの追従再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="id">外部ファイルで設定したID</param>
	/// <param name="pos">追従したい位置情報変数のアドレス</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectFollow(int& playingEffectHandle, 
						  const std::string& id, 
						  Vector3* pos, 
						  const Vector3& scale = { 1.0f, 1.0f, 1.0f }, 
						  const float speed = 1.0f, 
						  const Vector3& rot = { 0.0f, 0.0f, 0.0f },
						  const bool isZbuffer = true);

	/// <summary>
	/// 指定のエフェクトの追従とループ再生
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル(返ってきたエフェクトハンドルを使って値の変更を行う)</param>
	/// <param name="id">外部ファイルで設定したID</param>
	/// <param name="pos">追従したい位置情報変数のアドレス</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void PlayEffectLoopAndFollow(int& playingEffectHandle, 
								 const std::string& id, 
								 Vector3* pos, 
								 const Vector3& scale = { 1.0f, 1.0f, 1.0f }, 
								 const float speed = 1.0f, 
								 const Vector3& rot = { 0.0f, 0.0f, 0.0f },
								 const bool isZbuffer = true);

	/// <summary>
	/// エフェクトの全ての値の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="pos">位置</param>
	/// <param name="scale">拡大率</param>
	/// <param name="speed">再生速度</param>
	/// <param name="rot">回転率</param>
	void SetEffectAllParam(const int playingEffectHandle, const Vector3& pos, const Vector3& scale, const float speed, const Vector3& rot);

	/// <summary>
	/// エフェクトの拡大率の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="scale">拡大率</param>
	void SetEffectScale(const int playingEffectHandle, const Vector3& scale);

	/// <summary>
	/// エフェクトの再生速度の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="speed">再生速度</param>
	void SetEffectSpeed(const int playingEffectHandle, const float speed);

	/// <summary>
	/// エフェクトの回転率の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="rot">回転率</param>
	void SetEffectRot(const int playingEffectHandle, const Vector3& rot);

	/// <summary>
	/// エフェクトの位置の変更
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="pos">位置</param>
	void SetEffectPos(const int playingEffectHandle, const Vector3& pos);

	/// <summary>
	/// 特定のエフェクトが再生中か
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <returns>true : 再生中、false : 再生していない</returns>
	bool IsPlayingEffect(const int playingEffectHandle);

	/// <summary>
	/// 特定のエフェクトの再生をストップ
	/// 再生タイプがループのエフェクトの場合は止まった後、次のフレームで再生される
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	void StopEffect(const int playingEffectHandle);

	// エフェクト全ての再生をストップ
	// 再生タイプがループのエフェクトの場合は止まった後、次のフレームで再生される
	void StopAllEffect();

	/// <summary>
	/// 特定のエフェクトの削除
	/// 保存しているデータテーブルから削除する
	/// 主に再生タイプがループのエフェクトを削除するのに使う
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	void DeleteEffect(const int playingEffectHandle);

	/// <summary>
	/// 全てのエフェクトの削除
	/// </summary>
	void DeleteAllEffect();

private:
	/// <summary>
	/// コンストラクタ
	/// シングルトンパターンなのでprivate</summary>
	Effekseer3DEffectManager();

	/// <summary>
	/// コピーコンストラクタ禁止
	/// </summary>
	/// <param name=""></param>
	Effekseer3DEffectManager(const Effekseer3DEffectManager&) = delete;	

	/// <summary>
	/// 代入禁止
	/// </summary>
	/// <param name=""></param>
	void operator = (const Effekseer3DEffectManager&) = delete;			

	/// <summary>
	/// エフェクトのCSVファイルの読み込み
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	void LoadCsvEffectFile(const std::string& filePath);

	/// <summary>
	/// ループエフェクトの再再生
	/// ループエフェクトの再生が終了していたらエフェクトを再生する
	/// </summary>
	/// <param name="playingEffectHandle">エフェクトのハンドル</param>
	/// <param name="id">外部ファイルで設定したのID</param>
	void PlayEffectLoop(int& playingEffectHandle, const std::string& id);

private:
	// エフェクトのリソーステーブル
	std::map<std::string, EffectResourceData> m_effectResourceTable;

	// エフェクトのデータを保存したテーブル
	std::list<EffectData> m_effectDataTable;

	// 追従エフェクトのデータを保存したテーブル
	std::list<FollowEffectData> m_followEffectDataTable;

	// 画像ハンドル
	int m_imgHandle;
};