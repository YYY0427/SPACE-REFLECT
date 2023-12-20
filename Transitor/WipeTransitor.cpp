#include "WipeTransitor.h"
#include "../Application.h"
#include "DxLib.h"
#include <array>
#include <cassert>

// コンストラクタ
WipeTransitor::WipeTransitor(TransitDirection dir, int interval) :
	TransitorBase(interval)
{
	// ワイプする方向によってグラデーションの色を変える
	float left, right, top, bottom;
	left = right = top = bottom = 1.0f;

	// ワイプする方向には色を付けない
	switch (dir) 
	{
	case TransitDirection::UP:
		bottom = 0.0f;
		break;
	case TransitDirection::DOWN:
		top = 0.0f;
		break;
	case TransitDirection::LEFT:
		right = 0.0f;
		break;
	case TransitDirection::RIGHT:
		left = 0.0f;
		break;
	default:
		assert(false);
		break;
	}

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// カラーを設定
	std::array<COLOR_U8, 4> colors{};	
	colors[0].r = colors[0].g = colors[0].b = colors[0].a = 255 * left * top;		// 左上
	colors[1].r = colors[1].g = colors[1].b = colors[1].a = 255 * right * top;		// 右上
	colors[2].r = colors[2].g = colors[2].b = colors[2].a = 255 * left * bottom;	// 左下
	colors[3].r = colors[3].g = colors[3].b = colors[3].a = 255 * right * bottom;	// 右下


	// 左上の頂点の情報をセット( 1ポリゴン目の第1頂点 )
	std::array<VERTEX2D, 6> vertices{};
	vertices[0].pos.x = 0;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0.0f;
	vertices[0].rhw = 1.0f;
	vertices[0].dif = colors[0];
	vertices[0].u = 0.0f;
	vertices[0].v = 0.0f;

	// 右上の頂点の情報をセット( 1ポリゴン目の第2頂点 )
	vertices[1].pos.x = size.width;
	vertices[1].pos.y = 0;
	vertices[1].pos.z = 0.0f;
	vertices[1].rhw = 1.0f;
	vertices[1].dif = colors[1];
	vertices[1].u = 0.0f;
	vertices[1].v = 0.0f;

	// 左下の頂点の情報をセット( 1ポリゴン目の第3頂点 )
	vertices[2].pos.x = 0;
	vertices[2].pos.y = size.height;
	vertices[2].pos.z = 0.0f;
	vertices[2].rhw = 1.0f;
	vertices[2].dif = colors[2];
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;

	// 右下の頂点の情報をセット( 2ポリゴン目の第1頂点 )
	vertices[3].pos.x = size.width;
	vertices[3].pos.y = size.height;
	vertices[3].pos.z = 0.0f;
	vertices[3].rhw = 1.0f;
	vertices[3].dif = colors[3];
	vertices[3].u = 0.0f;
	vertices[3].v = 0.0f;

	// 2ポリゴン目の第2頂点は左下の頂点なのでコピー
	vertices[4] = vertices[2];

	// 2ポリゴン目の第3頂点は右上の頂点なのでコピー
	vertices[5] = vertices[1];
	m_gradationH = MakeScreen(size.width, size.height, true);
	auto bkScrH = GetDrawScreen();

	//レンダ―ターゲットを変更し、グラデーションの描画
	SetDrawScreen(m_gradationH);
	DrawPolygon2D(vertices.data(), 2, DX_NONE_GRAPH, true);

	//グラデーション描いたらまた元に戻す
	SetDrawScreen(bkScrH);
}

// デストラクタ
WipeTransitor::~WipeTransitor()
{
	// グラデーションのハンドルを削除
	DeleteGraph(m_gradationH);
}

// 更新
void WipeTransitor::Update()
{
	// 演出時間内なら
	if (m_frame < m_interval)
	{
		// フレームを進める
		m_frame++;

		// 描画先を切り替え先に変更
		SetDrawScreen(m_nextScene);
	}
	else if (m_frame == m_interval)
	{
		// 描画先を裏画面に変更
		SetDrawScreen(DX_SCREEN_BACK);
	}
}

// 描画
void WipeTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if (IsEnd()) return;

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	// 切り替え先の画面を描画
	DrawGraph(0, 0, m_nextScene, true);

	// 割合の計算
	auto rate = static_cast<float>(m_frame) / static_cast<float>(m_interval);

	// 切り替え前の画面をグラデーションでブレンドして描画
	DrawBlendGraph(0, 0, m_oldScene, true, m_gradationH, 255 * rate, 64);
}