#pragma once
#include "UIBase.h"
#include "../Math/Vector2.h"
#include "../Util/Timer.h"
#include <string>

/// <summary>
/// ゲージクラス
/// HP,MPなどのゲージなど
/// </summary>
class Gauge final : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="gaugeImgFilePath">ゲージのファイルパス</param>
	/// <param name="gaugeBackImgFilePath">ゲージ背景のファイルパス</param>
	/// <param name="gaugeFrameImgFilePath">ゲージフレームのファイルパス</param>
	/// <param name="maxValue">ゲージの最大値</param>
	/// <param name="pos">ゲージの位置</param>
	/// <param name="dimensions">ゲージのサイズ</param>
	/// <param name="isBurst">バースト演出を行うかどうか</param>
	/// <param name="addGaugeSpeed">バースト演出のゲージの増える速度</param>
	/// <param name="isDelayedDamage">ゲージのディレイ演出を行うかどうか</param>
	/// <param name="aimGaugeSpeed">ディレイ演出の減る速度</param>
	Gauge(
		const std::string gaugeImgFilePath, 
		const std::string gaugeBackImgFilePath,
		const std::string gaugeFrameImgFilePath,
		float maxValue,
		Vector2 pos,
		Vector2 dimensions,
		bool isGaugeBurst,
		float addGaugeSpeed,
		bool isDelayed,
		float aimGaugeSpeed);

	// デストラクタ
	~Gauge();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

	// ダメージを受けた時の処理
	void SetValue(float afterHp);

private:
	// 通常時の更新
	void NormalUpdate();	

	// バースト演出時の更新
	void BurstUpdate();

private:
	// ゲージの更新関数
	void(Gauge::*m_updateFunc)();

	// 画像ハンドル
	int m_gaugeImgH;		// ゲージ
	int m_gaugeBackImgH;	// ゲージの背景
	int m_gaugeFrameImgH;	// ゲージの枠

	// ゲージの値
	float m_currentValue;	// 現在の値
	float m_backValue;		// 背景の値
	float m_aimValue;		// 目標の値
	float m_maxValue;		// 最大値

	Vector2 m_pos;			// ゲージの位置
	Vector2 m_dimensions;	// ゲージの大きさ

	float m_addGaugeSpeed;	// ゲージが増える速度
	float m_aimGaugeSpeed;	// ゲージが減る速度

	// フラグ
	bool isDelayed;	// ダメージを受けてからゲージが減るまでの時間があるか

	// ダメージを受けてからのフレーム数
	Timer<int> m_damageFrameCount;
};

