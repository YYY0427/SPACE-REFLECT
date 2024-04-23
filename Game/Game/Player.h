#pragma once
#include "../DataType/PlayerParamDataType.h"
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Util/Timer.h"
#include <deque>
#include <memory>
#include <string>
#include <map>

// プロトタイプ宣言
class Model;
class Shield;
class Gauge;
class StatusBack;
class Camera;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objectDataFileName">オブジェクトデータのファイル名</param>
	Player(const std::string& objectDataFileName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// ゲーム開始時の更新
	/// </summary>
	/// <param name="cameraPos">カメラの位置</param>
	void UpdateStart(const Vector3& cameraPos);

	/// <summary>
	/// ゲーム中の更新
	/// </summary>
	/// <param name="cameraHorizon">カメラの水平角度</param>
	void UpdatePlay(const float cameraHorizon);

	/// <summary>
	/// ゲームクリア時の更新
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// ゲームオーバー時の更新
	/// </summary>
	/// <returns>ゲームオーバーが終了したか</returns>
	bool UpdateGameOver();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// シールドの描画
	/// </summary>
	void DrawShield();

	/// <summary>
	/// 外部ファイルからパラメータを読み込む
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadParameter(const std::string& fileName);

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	/// <param name="damage">ダメージ量</param>
	void OnDamage(const int damage);

	/// <summary>
	/// シールドが反射した時の処理
	/// </summary>
	void OnReflect();

	/// <summary>
	/// プレイヤーを徐々に透明にする
	/// </summary>
	void FadeOut();


	//// ゲッター ////

	/// <summary>
	/// モデルハンドルの取得
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle() const;		

	/// <summary>
	/// 当たり判定の半径の取得
	/// </summary>
	/// <returns>当たり半径</returns>
	float GetCollisionRadius() const;	

	/// <summary>
	/// スタート演出をしたかフラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsStartAnimation() const;

	/// <summary>
	/// プレイヤーが存在しているかの取得
	/// </summary>
	/// <returns>プレイヤーが存在しているか</returns>
	bool IsEnabled() const;					

	/// <summary>
	/// 位置情報の取得
	/// </summary>
	/// <returns>位置情報</returns>
	const Vector3& GetPos() const;	

	/// <summary>
	/// 移動ベクトルの取得
	/// </summary>
	/// <returns>移動ベクトル</returns>
	const Vector3& GetMoveVec() const;		

	/// <summary>
	/// シールドのポインタの取得
	/// </summary>
	/// <returns>シールドのポインタ</returns>
	const std::shared_ptr<Shield>& GetShield() const;

	/// <summary>
	/// プレイヤーの位置情報を保存しているテーブルの取得
	/// </summary>
	/// <returns>位置情報を保存しているテーブル</returns>
	const std::deque<Vector3>& GetPosLogTable() const;

	/// <summary>
	/// パラメータの取得
	/// </summary>
	/// <param name="type">パラメータの種類</param>
	/// <returns>パラメータ</returns>
	float GetParameter(const DataType::PlayerParamType type);


	//// セッター ////

	/// <summary>
	/// カメラのポインターの設定
	/// </summary>
	/// <param name="pCamera">カメラのポインタ</param>
	void SetCameraPointer(const std::shared_ptr<Camera>& pCamera);

private:
	// ポインタ
	std::shared_ptr<Model> m_pModel;		// モデル
	std::shared_ptr<Shield> m_pShield;		// シールド
	std::shared_ptr<Gauge> m_pHPbar;		// HPバー
	std::shared_ptr<StatusBack> m_pBackUI;	// ステータスバックUI
	std::shared_ptr<Camera> m_pCamera;		// カメラ

	// パラメーター
	Vector3 m_pos;				// 位置
	Vector3 m_rot;				// 回転
	Vector3 m_scale;			// 拡大率
	Vector3 m_moveVec;			// 移動ベクトル
	Vector3 m_moveSpeed;		// 移動速度
	Vector2 m_playerSize;		// プレイヤーのサイズ

	float   m_startMoveSpeedZ;	// スタート時の速度
	float   m_opacity;			// 透明度
	float   m_collisionRadius;	// 当たり判定の半径

	// HP
	int m_hp;
	int m_maxHp;

	// フラグ
	bool m_isInputLeftStick;	// 左スティックの入力フラグ
	bool m_isReflect;			// 反射中か
	bool m_isStartAnimation;	// スタート演出をしたかフラグ
	bool m_isGameOverSE;		// ゲームオーバーSEを再生したかフラグ

	// エフェクト
	int		m_boostEffectHandle;		// ブーストエフェクトハンドル
	Vector3 m_boostEffectScale;			// ブーストエフェクトの拡大率
	float	m_boostEffectSpeed;			// ブーストエフェクトの速度
	int		m_damageEffectHandle;		// ダメージエフェクトハンドル
	int		m_playerDeadEffectHandle;	// プレイヤー死亡エフェクトハンドル
	bool	m_isPlayerDeadEffect;		// プレイヤー死亡エフェクトの再生フラグ

	// タイマー
	int		   m_gameOverWaitFrame;			// 待機フレーム
	int		   m_onDamageSEWaitFrame;		// ダメージSEの再生待ちフレーム
	int		   m_ultimateTimer;				// 無敵時間のタイマー
	Timer<int> m_waitTimer;					// 待機時間のタイマー

	// 外部ファイルから読み込んだパラメータ
	std::map<std::string, float> m_parameterTable;

	// 決められたフレーム前まで位置情報を保存しているテーブル
	std::deque<Vector3> m_posLogTable;
};