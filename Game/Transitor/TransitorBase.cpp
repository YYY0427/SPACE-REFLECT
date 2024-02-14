#include "TransitorBase.h"
#include "../Application.h"
#include "DxLib.h"
#include <cassert>

// コンストラクタ
TransitorBase::TransitorBase(int interval) : 
	m_interval(interval),
	m_frame(0),
	m_oldScene(-1),
	m_nextScene(-1),
	m_isOldScene(false)
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
	m_oldScene = MakeGraph(size.width, size.height);
	m_nextScene = MakeScreen(size.width, size.height);

	// 描画先に設定されているグラフィック領域から指定領域のグラフィックを読みこむ
	SetOldScene();

	// 初期化
	m_frame = 0;
}

// 演出が終了したかどうか
bool TransitorBase::IsEnd() const
{
	return m_frame >= m_interval;
}

// フレームの設定
void TransitorBase::SetFrame(int frame)
{
	m_frame = frame;
}

// 描画先に設定されているグラフィック領域から指定領域のグラフィックを読みこむ
void TransitorBase::SetOldScene()
{
	// 画面サイズを取得
	const auto& size = Application::GetInstance().GetWindowSize();

	// 描画先に設定されているグラフィック領域から指定領域のグラフィックを読みこむ
	int result = GetDrawScreenGraph(0, 0, size.width, size.height, m_oldScene, true);
	assert(result != -1);
}