#include "StatusBack.h"
#include "ImageUI.h"
#include "../Math/Vector2.h"
#include <string>

namespace
{
	// 画像のファイルパス
	const std::string image_file_path = "Data/Image/PlayerStatusBased.png";

	// 最終拡大率
	constexpr float final_scale = 0.65f;

	// スタート演出の拡大速度
	constexpr float start_scale_speed = 0.05f;

	// 初期位置
	const Vector2 init_pos = { 30, 550 };
}

// コンストラクタ
StatusBack::StatusBack() :
	m_isStartAnimation(false)
{
	// 初期化
	m_pos = init_pos;

	// 画像の読み込み
	m_pImageUI = std::make_unique<ImageUI>(image_file_path);

	// 画像の設定
	m_pImageUI->SetPos(m_pos);			// 位置
	m_pImageUI->SetScale({ 0, 0 });			// 拡大率
}

// デストラクタ
StatusBack::~StatusBack()
{
	
}

// 更新
void StatusBack::Update()
{
	// 位置の更新
	m_pImageUI->SetPos(m_pos);
}

// スタート演出の更新
void StatusBack::UpdateStartAnimation()
{
	// スタート演出の拡大
	// X軸の拡大
	if (m_pImageUI->GetScale().x < final_scale)
	{
		m_pImageUI->SetScale({ m_pImageUI->GetScale().x + start_scale_speed, 0.0f });
	}
	// Y軸の拡大
	else if (m_pImageUI->GetScale().x >= final_scale && m_pImageUI->GetScale().y < final_scale)
	{
		m_pImageUI->SetScale({ final_scale, m_pImageUI->GetScale().y + start_scale_speed });
	}
	// 拡大終了
	else
	{
		m_pImageUI->SetScale({final_scale, final_scale });
		m_isStartAnimation = true;
	}
}

// 描画
void StatusBack::Draw()
{
	// 画像の描画
	m_pImageUI->Draw();
}

// スタート演出フラグの取得
bool StatusBack::IsStartAnimation() const
{
	return m_isStartAnimation;
}
