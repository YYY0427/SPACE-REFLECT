#pragma once
#include <DxLib.h>
#include <memory>
#include <deque>
#include "../Math/Vector3.h"
#include "../Util/Timer.h"
#include <string>

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
	Player(std::string objectDataFileName);

	// デストラクタ
	~Player();

	// 更新
	void UpdateStart(Vector3 cameraPos);
	void Update(float cameraHorizon);
	void UpdateGameClear();
	void UpdateGameOver();

	// 描画
	void Draw();
	void DrawShield();

	// プレイヤーのダメージ処理
	void OnDamage(int damage);

	// シールドが反射した時の処理
	void OnReflect();

	// プレイヤーが生きているか
	bool IsLive() const;

	// 位置情報の取得
	Vector3 GetPos() const;

	// 移動ベクトルの取得
	Vector3 GetMoveVec() const;

	// プレイヤーの当たり判定の半径の取得
	float GetCollsionRadius() const;

	// プレイヤーモデルのハンドルの取得
	int GetModelHandle() const;

	// スタート演出をしたかフラグの取得
	bool GetIsStartAnimation() const;

	// シールドのインスタンスの取得
	std::shared_ptr<Shield> GetShield() const;

	// 決められたフレームの数だけ位置情報を保存するテーブルの取得
	std::deque<Vector3> GetPosLogTable() const;

	// スローの値の設定
	void SetSlowValue(float slowValue);

private:
	// ポインタ
	std::shared_ptr<Model> m_pModel;
	std::shared_ptr<Shield> m_pShield;
	std::shared_ptr<Gauge> m_pHPbar;
	std::shared_ptr<StatusBack> m_pBackUI;

	// 決められたフレーム前まで位置情報を保存しているテーブル
	std::deque<Vector3> m_posLogTable;

	// 位置情報
	Vector3 m_pos;

	// 回転情報
	Vector3 m_rot;

	// 拡大率
	Vector3 m_scale;

	// 移動ベクトル
	Vector3 m_moveVec;

	// 不透明度
	float m_opacity;

	// プレイヤーの移動速度
	float m_moveSpeed;

	// 命
	int m_hp;

	// スローの値
	float m_slowValue;

	// フラグ
	bool m_isInputLeftStick;	// 左スティックの入力フラグ
	bool m_isReflect;			// 反射中か
	bool m_isStartAnimation;	// スタート演出をしたかフラグ

	// エフェクト
	int m_windEffectHandle;			// 風エフェクトのハンドル
	int m_boostEffectHandle;		// ブーストエフェクトハンドル
	int m_damageEffectHandle;		// ダメージエフェクトハンドル
//	Vector3 m_boostEffectScale;		// ブーストエフェクトの拡大率
//	float m_boostEffectSpeed;		// ブーストエフェクトの再生速度
	int m_playerDeadEffectHandle;	// プレイヤー死亡エフェクトハンドル
	bool m_isPlayerDeadEffect;		// プレイヤー死亡エフェクトの再生フラグ

	// タイマー
	int m_ultimateTimer;		// 無敵時間のタイマー
	Timer<int> m_waitTimer;		// 待機時間のタイマー
};