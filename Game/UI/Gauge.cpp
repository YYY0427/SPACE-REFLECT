#include "Gauge.h"
#include "../Util/DrawFunctions.h"
#include "../SoundManager.h"
#include <DxLib.h>

namespace
{
	// ゲージのフレームのサイズ
	constexpr float gauge_frame_size = 3.0f;
}

// コンストラクタ
Gauge::Gauge(
	const std::string& gaugeImgFilePath, 
	const std::string& gaugeBackImgFilePath, 
	const std::string& gaugeFrameImgFilePath, 
	const float maxValue, 
	const Vector2& pos, 
	const Vector2& dimensions, 
	const int   addGaugeFrame, 
	const int   subGaugeFrame,
	const int   isDelayFrame) :
	m_maxValue(maxValue),
	m_aimValue(maxValue),
	m_dimensions(dimensions),
	m_isDelayFrame(isDelayFrame),
	m_subGaugeFrame(subGaugeFrame),
	m_addGaugeFrame  (addGaugeFrame),
	m_currentValue(0.0f),
	m_backValue(0.0f),
	m_updateFunc(&Gauge::EntranceAnimUpdate),
	m_isEndEntranceAnim(false)
{
	// 初期化
	m_pos = pos;

	// 画像の読み込み
	// 画像が指定されていない場合は-1を代入する
	(gaugeImgFilePath == "") ? 
		m_gaugeImgH = -1      : m_gaugeImgH      = my::MyLoadGraph(gaugeImgFilePath.c_str());
	(gaugeBackImgFilePath == "") ?
		m_gaugeBackImgH = -1  : m_gaugeBackImgH  = my::MyLoadGraph(gaugeBackImgFilePath.c_str());
	(gaugeFrameImgFilePath == "") ?
		m_gaugeFrameImgH = -1 : m_gaugeFrameImgH = my::MyLoadGraph(gaugeFrameImgFilePath.c_str());
}

// デストラクタ
Gauge::~Gauge()
{
	// 画像の削除
	DeleteGraph(m_gaugeImgH);
	DeleteGraph(m_gaugeBackImgH);
	DeleteGraph(m_gaugeFrameImgH);
}

// 更新
void Gauge::Update()
{
	// 更新関数を呼ぶ
	(this->*m_updateFunc)();
}

// 描画
void Gauge::Draw()
{
	// ゲージが0より小さかったら描画しない
	if (m_backValue > 0.0f)
	{
		// 背景のゲージの描画
		DrawExtendGraphF(
			m_pos.x - m_dimensions.x / 2,
			m_pos.y - m_dimensions.y / 2,
			m_pos.x - (m_dimensions.x / 2) + ((m_dimensions.x) * (static_cast<float>(m_backValue / m_maxValue))),
			m_pos.y + m_dimensions.y / 2,
			m_gaugeBackImgH,
			true);

		// ゲージの描画
		DrawExtendGraphF(
			m_pos.x - m_dimensions.x / 2,
			m_pos.y - m_dimensions.y / 2,
			m_pos.x - (m_dimensions.x / 2) + ((m_dimensions.x) * (static_cast<float>(m_currentValue / m_maxValue))),
			m_pos.y + m_dimensions.y / 2,
			m_gaugeImgH,
			true);
	}
	// フレームの描画
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2,
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x - (m_dimensions.x / 2) - gauge_frame_size,
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x - (m_dimensions.x / 2),
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
	DrawExtendGraph(
		m_pos.x + (m_dimensions.x / 2),
		m_pos.y - m_dimensions.y / 2 - gauge_frame_size,
		m_pos.x + (m_dimensions.x / 2) + gauge_frame_size,
		m_pos.y + m_dimensions.y / 2 + gauge_frame_size,
		m_gaugeFrameImgH,
		true);
}

// ゲージの値を設定
void Gauge::SetValue(const float afterValue)
{
	// ディレイタイムの設定
	m_damageFrameCount.SetTime(m_isDelayFrame);

	// 目標値を設定
	m_aimValue = afterValue;
	m_aimValue = (std::max)(m_aimValue, 0.0f);
}

// バースト演出が終了したか
bool Gauge::IsEndEntranceAnim() const
{
	return m_isEndEntranceAnim;
}

// 通常時の更新
void Gauge::NormalUpdate()
{
	// 目標値を現在の値に合わせる
	m_currentValue = m_aimValue;
	m_currentValue = (std::max)(m_currentValue, 0.0f);

	// ダメージ演出の更新
	m_damageFrameCount.Update(-1);
	if (m_damageFrameCount.GetTime() <= 0)
	{
		// 背景のゲージと目標値が一致していない場合
		if (m_backValue != m_aimValue)
		{
			// サブゲージフレームが設定されている場合
			if (m_subGaugeFrame > 0)
			{
				// 目標値に向かって決められたフレーム数で減少する
				m_backValue -= static_cast<float>(m_maxValue / m_subGaugeFrame);
			}
			// そうでない場合
			else
			{
				// 目標値にする
				m_backValue = m_aimValue;
			}
			
			// 目標に合致したら止める
			if (m_backValue < m_aimValue)
			{
				m_backValue = m_aimValue;
			}
		}
	}
}

// 登場演出時の更新
void Gauge::EntranceAnimUpdate()
{
	// ゲージの増加フレームが設定されている場合
	if (m_addGaugeFrame > 0)
	{
		// 目標値に向かって決められたフレーム数で増加する
		m_currentValue += static_cast<float>(m_maxValue / m_addGaugeFrame);
		m_backValue    += static_cast<float>(m_maxValue / m_addGaugeFrame);
	}
	// そうでない場合
	else
	{
		// 目標値にする
		m_currentValue = m_maxValue;
		m_backValue    = m_maxValue;
	}

	// 目標値を超えたら
	if (m_currentValue >= m_maxValue && 
		m_backValue >= m_maxValue)
	{
		// 演出終了
		m_isEndEntranceAnim = true;

		// 目標値にする
		m_currentValue = m_maxValue;
		m_backValue    = m_maxValue;

		// 更新関数を通常時の更新に戻す
		m_updateFunc = &Gauge::NormalUpdate;
	}
}