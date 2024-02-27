#include "Triangle.h"
#include "../Math/MathUtil.h"
#include "../Application.h"
#include <cmath>
#include <DxLib.h>

namespace
{
	// 三角形の数
	constexpr int triangle_num = 4;

	// 三角形の初期角度
	constexpr float triangle_init_angle = 135.0f;

	// 三角形の初期色
	const Vector3 triangle_init_color = { 255, 255, 255 };

	// 三角形のなす角
	constexpr float triangle_angle = 10.0f;

	// 三角形どうしの間隔の角度
	constexpr float triangle_interval_angle = 90.0f;

	// 三角形の色を変化させる速度
	constexpr float color_speed = 2.0f;

	// 三角形の最終的な色
	const Vector3 end_triangle_color = { 255, 0, 0 };

	// 三角形の色を変化させる時間(1.0 = m_frame)
	constexpr float change_color_time = 0.5f;

	// 三角形の最大のアルファ値
	constexpr int max_alpha = 255;
}

// コンストラクタ
Triangle::Triangle(const int frame) :
	m_frame(frame),
	m_intervalFrame(frame / (triangle_num + 1)),
	m_angle(triangle_init_angle),
	m_color(triangle_init_color)
{
}

// デストラクタ
Triangle::~Triangle()
{
}

// 更新
void Triangle::Update(const Vector3& bossEnemyPos)
{
	// タイマーの更新
	m_frame.Update(1);
	m_intervalFrame.Update(1);

	// 三角形を描画する間隔のフレーム経過して
	// 三角形の数が指定数に達していない場合は追加
	if (m_intervalFrame.IsTimeOut() && m_triangleDataTable.size() < triangle_num)
	{
		// タイマーのリセット
		m_intervalFrame.Reset();

		// 三角形のデータを追加
		TriangleData triangleData;

		// 三角形の位置を設定
		auto& screenSize    = Application::GetInstance().GetWindowSize();
		Vector3 screenPos   = Vector3::FromDxLibVector3(ConvWorldPosToScreenPos(bossEnemyPos.ToDxLibVector3()));
		triangleData.pos[0] = Vector2(screenPos.x, screenPos.y);
		triangleData.pos[1] = Vector2((0.5f * cosf(m_angle) + 0.5f) * screenSize.width, (0.5f * sinf(m_angle) + 0.5f) * screenSize.width);
		triangleData.pos[2] = Vector2((0.5f * cosf(m_angle + triangle_angle) + 0.5f) * screenSize.width, (0.5f * sinf(m_angle + triangle_angle) + 0.5f) * screenSize.width);
		/*triangleData.pos[1] = 
			Vector2(cosf(MathUtil::ToRadian(m_angle)) * screenSize.width,
			sinf(MathUtil::ToRadian(m_angle)) * screenSize.height);

		triangleData.pos[2] =
			Vector2(cosf(MathUtil::ToRadian(m_angle + triangle_angle)) * screenSize.width,
					sinf(MathUtil::ToRadian(m_angle + triangle_angle)) * screenSize.height);*/

		// 角度を加算
		m_angle += triangle_interval_angle;

		// アルファ値を設定
		triangleData.alpha = max_alpha;

		// 三角形のデータを追加
		m_triangleDataTable.push_back(triangleData);
	}

	// 三角形のデータを更新
	for (auto& triangle : m_triangleDataTable)
	{
		// チカチカさせる
		triangle.alpha *= -1;

		// 三角形の位置を設定
		Vector3 screenPos = Vector3::FromDxLibVector3(ConvWorldPosToScreenPos(bossEnemyPos.ToDxLibVector3()));
		triangle.pos[0] = Vector2(screenPos.x, screenPos.y);
	}

	// 特定の時間になったら三角形の色を変化させる
	if (m_frame.GetTime() >= m_frame.GetLimitTime() * change_color_time)
	{
		// 三角形の色を変化させる
		m_color -= color_speed;

		// 三角形の色は最終的な色で止める
		m_color.x = (std::max)(m_color.x, end_triangle_color.x);
		m_color.y = (std::max)(m_color.y, end_triangle_color.y);
		m_color.z = (std::max)(m_color.z, end_triangle_color.z);
	}
}

// 描画
void Triangle::Draw()
{
	// 演出が終わっていたら描画しない
	if (IsEnd()) return;

	for (auto& triangle : m_triangleDataTable)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, triangle.alpha);
		DrawTriangle(triangle.pos[0].x, triangle.pos[0].y, triangle.pos[1].x, triangle.pos[1].y, triangle.pos[2].x, triangle.pos[2].y, 
			GetColor(m_color.x, m_color.y, m_color.z), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

// 終了したか
bool Triangle::IsEnd() const
{
	return m_frame.IsTimeOut();
}
