#include "SceneManager.h"
#include "SceneBase.h"

// コンストラクタ
SceneManager::SceneManager()
{
}

// デストラクタ
SceneManager::~SceneManager()
{
}

// 更新
void SceneManager::Update()
{
	// リストが空っぽだったら何もしない
	if (!m_scenes.empty())
	{
		// 先頭のみ実行
		m_scenes.front()->Update();
	}
}

// 描画
void SceneManager::Draw()
{
	// 逆順に描画(最後に積んだものが最初に描画される)
	for (auto rit = m_scenes.rbegin(); rit != m_scenes.rend(); rit++)
	{
		rit->get()->Draw();
	}
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