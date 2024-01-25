#include "Gauge.h"
#include "../Util/DrawFunctions.h"
#include <DxLib.h>

namespace
{
	// ゲージのフレームのサイズ
	constexpr float gauge_frame_size = 3.0f;
}

// コンストラクタ
Gauge::Gauge(
	const std::string gaugeImgFilePath, 
	const std::string gaugeBackImgFilePath, 
	const std::string gaugeFrameImgFilePath, 
	float maxValue, 
	Vector2 pos, 
	Vector2 dimensions, 
	bool isGaugeBurst, 
	float addGaugeSpeed, 
	bool isDelayedDamage, 
	float aimGaugeSpeed) :
	m_maxValue(maxValue),
	m_aimValue(maxValue),
	m_backValue(maxValue),
	m_pos(pos),
	m_dimensions(dimensions),
	m_isDelayed(isDelayedDamage),
	m_aimGaugeSpeed(aimGaugeSpeed),
	m_addGaugeSpeed(addGaugeSpeed)
{
	// バースト演出を行うかどうか
	if (isGaugeBurst)
	{
		m_currentValue = 0.0f;
		m_backValue = 0.0f;
		m_updateFunc = &Gauge::BurstUpdate;
		m_isEndBurst = false;
	}
	else
	{
		m_currentValue = m_maxValue;
		m_updateFunc = &Gauge::NormalUpdate;
		m_isEndBurst = true;
	}

	// 画像の読み込み
	// 画像が指定されていない場合は-1を代入する
	(gaugeImgFilePath == "") ? 
		m_gaugeImgH = -1 : m_gaugeImgH = my::MyLoadGraph(gaugeImgFilePath.c_str());
	(gaugeBackImgFilePath == "") ?
		m_gaugeBackImgH = -1 : m_gaugeBackImgH = my::MyLoadGraph(gaugeBackImgFilePath.c_str());
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
		DrawExtendGraph(
			m_pos.x - m_dimensions.x / 2,
			m_pos.y - m_dimensions.y / 2,
			m_pos.x - (m_dimensions.x / 2) + ((m_dimensions.x) * (static_cast<float>(m_backValue / m_maxValue))),
			m_pos.y + m_dimensions.y / 2,
			m_gaugeBackImgH,
			true);

		// ゲージの描画
		DrawExtendGraph(
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

void Gauge::SetValue(float afterValue)
{
	if (m_isDelayed)
	{
		// ディレイタイムの設定
		m_damageFrameCount.SetTime(60);
	}

	// 目標値を設定
	m_aimValue = afterValue;
	m_aimValue = (std::max)(m_aimValue, 0.0f);
}

// バースト演出が終了したか
bool Gauge::IsEndBurst() const
{
	return m_isEndBurst;
}

// 通常時の更新
void Gauge::NormalUpdate()
{
	// 目標値を現在の値に合わせる
	m_currentValue = m_aimValue;
	m_currentValue = (std::max)(m_currentValue, 0.0f);

	m_damageFrameCount.Update(-1);
	if (m_damageFrameCount.GetTime() <= 0)
	{
		// 背景のゲージと目標値が一致していない場合
		if (m_backValue != m_aimValue)
		{
			// 毎フレーム緩やかに目標に近づく
			m_backValue -= m_aimGaugeSpeed;

			// 目標に合致したら止める
			if (m_backValue < m_aimValue)
			{
				m_backValue = m_aimValue;
			}
		}
	}
}

// バースト演出の更新
void Gauge::BurstUpdate()
{
	// 目標値に向かってゲージを増やす
	m_currentValue += m_addGaugeSpeed;
	m_backValue += m_addGaugeSpeed;

	// 目標値を超えたら
	if (m_currentValue >= m_maxValue && m_backValue >= m_maxValue)
	{
		// バースト演出が終了したことを示すフラグを立てる
		m_isEndBurst = true;

		// 目標値にする
		m_currentValue = m_maxValue;
		m_backValue = m_maxValue;

		// 更新関数を通常時の更新に戻す
		m_updateFunc = &Gauge::NormalUpdate;
	}
}