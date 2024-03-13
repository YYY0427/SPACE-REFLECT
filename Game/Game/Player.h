#pragma once
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

/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	// コンストラクタ
	Player(const std::string& objectDataFileName);

	// デストラクタ
	~Player();

	// 更新
	void UpdateStart(const Vector3& cameraPos);
	void Update(float cameraHorizon);
	void UpdateGameClear();
	bool UpdateGameOver();

	// 描画
	void Draw();
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
	void OnDamage(int damage);

	/// <summary>
	/// シールドが反射した時の処理
	/// </summary>
	void OnReflect();

	// ゲッター
	int     GetModelHandle() const;			// モデルハンドル
	float   GetCollisionRadius() const;		// 当たり判定の半径
	bool    IsStartAnimation() const;		// スタート演出をしたかフラグ
	bool    IsLive() const;					// 生存フラグ
	const Vector3& GetPos() const;			// 位置
	const Vector3& GetMoveVec() const;		// 移動ベクトル
	const std::shared_ptr<Shield>& GetShield() const;		// シールドのポインタ
	const std::deque<Vector3>&     GetPosLogTable() const;	// 位置情報のテーブル

	/// <summary>
	/// パラメータの取得
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns>パラメータ</returns>
	float GetParameter(const std::string& key) const;	

private:
	// ポインタ
	std::shared_ptr<Model> m_pModel;		// モデル
	std::shared_ptr<Shield> m_pShield;		// シールド
	std::shared_ptr<Gauge> m_pHPbar;		// HPバー
	std::shared_ptr<StatusBack> m_pBackUI;	// ステータスバックUI

	// パラメーター
	Vector3 m_pos;				// 位置
	Vector3 m_rot;				// 回転
	Vector3 m_scale;			// 拡大率
	Vector3 m_moveVec;			// 移動ベクトル
	Vector2 m_playerSize;		// プレイヤーのサイズ
	float   m_moveSpeedXY;		// 移動速度
	float   m_startMoveSpeedZ;	// スタート時の速度
	float   m_moveSpeedZ;		// Z軸方向の移動速度
	float   m_opacity;			// 透明度
	float   m_collisionRadius;	// 当たり判定の半径

	// HP
	int m_hp;
	int m_maxHp;

	// フラグ
	bool m_isInputLeftStick;	// 左スティックの入力フラグ
	bool m_isReflect;			// 反射中か
	bool m_isStartAnimation;	// スタート演出をしたかフラグ

	// エフェクト
	int		m_boostEffectHandle;		// ブーストエフェクトハンドル
	Vector3 m_boostEffectScale;			// ブーストエフェクトの拡大率
	float	m_boostEffectSpeed;			// ブーストエフェクトの速度
	int		m_damageEffectHandle;		// ダメージエフェクトハンドル
	int		m_playerDeadEffectHandle;	// プレイヤー死亡エフェクトハンドル
	bool	m_isPlayerDeadEffect;		// プレイヤー死亡エフェクトの再生フラグ

	// タイマー
	int		   m_waitFrame;					// 待機フレーム
	int		   m_ultimateTimer;				// 無敵時間のタイマー
	Timer<int> m_dieEffectIntervalTimer;	// 死亡エフェクトのインターバルタイマー
	Timer<int> m_waitTimer;					// 待機時間のタイマー

	// 外部ファイルから読み込んだパラメータ
	std::map<std::string, float> m_parameterTable;

	// 決められたフレーム前まで位置情報を保存しているテーブル
	std::deque<Vector3> m_posLogTable;
};