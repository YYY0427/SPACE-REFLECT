#include "TileTransitor.h"
#include "../Application.h"
#include "DxLib.h"

// コンストラクタ
TileTransitor::TileTransitor(int cellSize, int interval) :
	TransitorBase(interval),
	m_cellSize(cellSize)
{
	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// セルの数を計算
	int xNum = (size.width / m_cellSize) + 1;
	int yNum = (size.height / m_cellSize) + 1;

	// セルの数分のメモリを確保
	m_tiles.reserve(xNum * yNum);

	// 配列にセルのインデックスを格納
	for (int yIdx = 0; yIdx < yNum; yIdx++)
	{
		for (int xIdx = 0; xIdx < xNum; xIdx++)
		{
			m_tiles.push_back({xIdx, yIdx});
		}
	}
}

// 更新
void TileTransitor::Update()
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

	// 演出が終了していたら更新しない
	if(IsEnd()) return;

	// 配列の並びをシャッフル
	std::shuffle(m_tiles.begin(), m_tiles.end(), m_mt);

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// セルの数を計算
	int xNum = (size.width / m_cellSize) + 1;
	int yNum = (size.height / m_cellSize) + 1;

	// 消すセルの数を計算
	int eraseNum = ((xNum * yNum) / m_interval);

	// 消すセルの数が配列のサイズより大きい場合
	if (m_tiles.size() > eraseNum)
	{
		// 配列の末尾からeraseNum分だけ削除
		m_tiles.erase(m_tiles.end() - eraseNum, m_tiles.end());
	}
	else
	{
		// 配列をクリア
		m_tiles.clear();
	}
}

void TileTransitor::Draw()
{
	// 演出が終了していたら描画しない
	if(IsEnd()) return;

	// 描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

	// ウィンドウサイズの取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 切り替え先の画面を描画
	DrawRectGraph(0, 0, 0, 0, size.width, size.height, m_nextScene, true);

	// 切り替え前の画面をタイルで描画
	for (const auto& tile : m_tiles)
	{
		DrawRectGraph(
			tile.xIdx * m_cellSize, 
			tile.yIdx * m_cellSize,
			tile.xIdx * m_cellSize, 
			tile.yIdx * m_cellSize, 
			m_cellSize, m_cellSize, 
			m_oldScene, true);
	}
}
