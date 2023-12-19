#include "TransitorBase.h"
#include "../Application.h"
#include "DxLib.h"
#include <cassert>

// コンストラクタ
TransitorBase::TransitorBase(int interval) : 
	m_interval(interval),
	m_frame(0),
	m_oldScene(0),
	m_nextScene(0)
{
}

// デストラクタ
TransitorBase::~TransitorBase()
{
	// 画面を削除
	DeleteGraph(m_oldScene);
	DeleteGraph(m_nextScene);
}

// 演出開始
void TransitorBase::Start()
{
	// 画面サイズを取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 画面を作成
	m_oldScene = MakeScreen(size.width, size.height);
	m_nextScene = MakeScreen(size.width, size.height);

	// 描画先に設定されているグラフィック領域から指定領域のグラフィックを読みこむ
	int result = GetDrawScreenGraph(0, 0, size.width, size.height, m_oldScene);
	assert(result != -1);

	// 初期化
	m_frame = 0;
}

// 演出が終了したかどうか
bool TransitorBase::IsEnd() const
{
	return m_frame >= m_interval;
}
