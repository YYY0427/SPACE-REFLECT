#include "SceneManager.h"
#include "SceneBase.h"
#include "../Application.h"

// コンストラクタ
SceneManager::SceneManager() :
	m_updateTime(0),
	m_drawTime(0)
{
}

// デストラクタ
SceneManager::~SceneManager()
{
}

// 更新
void SceneManager::Update()
{
	// ミリ秒単位の精度を持つカウンタの現在の値を得る
	LONGLONG start = GetNowHiPerformanceCount();

	// リストが空っぽだったら何もしない
	if (!m_scenes.empty())
	{
		// 先頭のみ実行
		m_scenes.front()->Update();
	}

	// 更新時間を計測
	m_updateTime = GetNowHiPerformanceCount() - start;
}

// 描画
void SceneManager::Draw()
{
	// ミリ秒単位の精度を持つカウンタの現在の値を得る
	LONGLONG start = GetNowHiPerformanceCount();

	// 逆順に描画(最後に積んだものが最初に描画される)
	for (auto rit = m_scenes.rbegin(); rit != m_scenes.rend(); rit++)
	{
		// 描画
		rit->get()->Draw();
	}

	// 描画時間を計測
	m_drawTime = GetNowHiPerformanceCount() - start;

	// fpsの取得
	float fps = GetFPS();

	// 画面サイズの取得
	Size size = Application::GetInstance().GetWindowSize();

#ifdef _DEBUG
	// 1フレームにかかった描画時間を表示
	float rate = static_cast<float>(m_updateTime + m_drawTime) / static_cast<long long>(1000000 / fps);
	int width = static_cast<int>(size.width * rate);
	DrawBox(16, size.height - 40, 32, size.height - 24, 0xff0000, true);
	DrawString(40, size.height - 40, "DrawTime", 0xffffff);
	DrawBox(0, size.height - 16, width, size.height, 0xff0000, true);

	// 1フレームにかかった更新時間を表示
	rate = static_cast<float>(m_updateTime) / static_cast<long long>(1000000 / fps);
	width = static_cast<int>(size.width * rate);
	DrawBox(150, size.height - 40, 166, size.height - 24, 0x0000ff, true);
	DrawString(174, size.height - 40, "UpdateTime", 0xffffff);
	DrawBox(0, size.height - 16, width, size.height, 0x0000ff, true);

	// fpsを描画
	DrawFormatString(300, size.height - 40, 0xffffff, "FPS : %f", fps);
#endif
}

// シーンの切り替え
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> nextScene)
{
	// リストが空っぽだったら入れ替えるのではなく
	if (m_scenes.empty())
	{
		// 先頭に追加する
		m_scenes.push_front(nextScene);
	}
	else 
	{
		// 既に１つ以上あったら先頭のものを入れ替える
		m_scenes.front() = nextScene;
	}
}

// 
void SceneManager::ChangeAndClearScene(std::shared_ptr<SceneBase> nextScene)
{
	// シーンの全削除
	ClearScene();

	// シーンの切り替え
	ChangeScene(nextScene);
}

// 現在のシーンの上にシーンを積む(ポーズ)
void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_scenes.push_front(scene);
}

// 一番上のシーンを削除
void SceneManager::PopScene()
{
	m_scenes.pop_front();
}

// シーンの全削除
void SceneManager::ClearScene()
{
	m_scenes.clear();
}