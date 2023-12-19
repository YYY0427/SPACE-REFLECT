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
	//末尾のみ実行
	m_scenes.back()->Update();
}

// 描画
void SceneManager::Draw()
{
	//先頭から順に描画(最後に積んだものが最後に描画される)
	for (auto& scene : m_scenes) 
	{
		scene->Draw();
	}
}

// シーンの切り替え
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> nextScene)
{
	//リストが空っぽだったら入れ替えるのではなく
	if (m_scenes.empty())
	{
		//末尾に追加する
		m_scenes.push_back(nextScene);
	}
	else 
	{
		//既に１つ以上あったら末尾のものを入れ替える
		m_scenes.back() = nextScene;
	}
}

// 現在のシーンの上にシーンを積む(ポーズ)
void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_scenes.push_back(scene);
}

// 一番上のシーンを削除
void SceneManager::PopScene()
{
	m_scenes.pop_back();
}