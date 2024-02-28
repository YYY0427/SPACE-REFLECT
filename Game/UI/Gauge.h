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
	/// <param name="addGaugeFrame">登場演出のゲージの増えるフレーム数(0の場合登場演出を行わない)</param>
	/// <param name="subGaugeFrame">ディレイ演出のゲージの減るフレーム</param>
	/// <param name="isDelayFrame">ディレイ演出のフレーム数(0以下の場合行わない)</param>
	Gauge(
		const std::string& gaugeImgFilePath,
		const std::string& gaugeBackImgFilePath,
		const std::string& gaugeFrameImgFilePath,
		const float maxValue,
		const Vector2& pos,
		const Vector2& dimensions,
		const int   addGaugeFrame = 0,
		const int   subGaugeFrame = 0,
		const int   isDelayFrame  = 0);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Gauge();

	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// ゲージの値を設定
	/// </summary>
	/// <param name="afterHp">設定する値</param>
	void SetValue(const float afterHp);

	/// <summary>
	/// 登場演出が終了したか 
	/// </summary>
	/// <returns>true : 終了、false : 途中</returns>
	bool IsEndEntranceAnim() const;

private:
	/// <summary>
	/// 通常時の更新
	/// </summary>
	void NormalUpdate();	

	/// <summary>
	/// 登場演出の更新
	/// </summary>
	void EntranceAnimUpdate();

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

	Vector2 m_dimensions;	// ゲージの大きさ

	// 登場演出でゲージが増えるフレーム数
	int m_addGaugeFrame;	

	// ディレイ演出でゲージが減る速度
	int m_subGaugeFrame;	

	// フラグ
	int  m_isDelayFrame;				// ダメージを受けてからゲージが減るまでの時間があるか
	bool m_isEndEntranceAnim;		// 登場演出が終了したか(演出を行わない場合はtrue)

	// ダメージを受けてからのフレーム数
	Timer<int> m_damageFrameCount;
};

