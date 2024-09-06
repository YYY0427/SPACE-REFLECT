#include "SceneManager.h"
#include "SceneBase.h"
#include "LoadScene.h"
#include "../Application.h"

namespace
{
	// FPSの表示を更新する間隔(フレーム数)
	constexpr int fps_update_interval = 10;
}

namespace Scene
{
	// コンストラクタ
	Manager::Manager() :
		m_updateTime(0),
		m_drawTime(0),
		m_prevFps(0.0f),
		m_isLoadScene(false)
	{
	}

	// デストラクタ
	Manager::~Manager()
	{
	}

	// 更新
	void Manager::Update()
	{
		// ミリ秒単位の精度を持つカウンタの現在の値を得る
		LONGLONG start = GetNowHiPerformanceCount();

		// リストが空っぽだったら何もしない
		if (!m_pSceneTable.empty())
		{
			// 先頭のみ実行
			m_pSceneTable.front()->Update();
		}

		// 更新時間を計測
		m_updateTime = GetNowHiPerformanceCount() - start;
	}

	// 描画
	void Manager::Draw()
	{
		// ミリ秒単位の精度を持つカウンタの現在の値を得る
		LONGLONG start = GetNowHiPerformanceCount();

		// 逆順に描画(最後に積んだものが最初に描画される)
		for (auto rit = m_pSceneTable.rbegin(); rit != m_pSceneTable.rend(); rit++)
		{
			// 描画
			rit->get()->Draw();
		}

		// 描画時間を計測
		m_drawTime = GetNowHiPerformanceCount() - start;

		// FPSの描画
		DrawFps();

		// 更新時間の描画
		DrawUpdateTime();

		// 描画時間の描画
		DrwaDrawTime();
	}

	// シーンの切り替え
	void Manager::ChangeScene(const std::shared_ptr<Scene::Base>& nextScene)
	{
		// リストが空っぽだったら入れ替えるのではなく
		if (m_pSceneTable.empty())
		{
			// シーンの追加
			PushScene(nextScene);
		}
		else
		{
			// シーンの終了処理
			m_pSceneTable.front()->End();

			// 既に１つ以上あったら先頭のものを入れ替える
			m_pSceneTable.front() = nextScene;

			// シーンの初期化
			m_pSceneTable.front()->Init();

			// 非同期読み込みのチェック
			CheckAsyncLoad();
		}
	}

	// シーンの切り替えと前のシーンを削除
	void Manager::ChangeAndClearScene(const std::shared_ptr<Scene::Base>& nextScene)
	{
		// シーンの全削除
		ClearScene();

		// シーンの切り替え
		ChangeScene(nextScene);
	}

	// 非同期読み込みのチェック 
	void Manager::CheckAsyncLoad()
	{
		// 非同期読み込み中の数を取得
		if (GetASyncLoadNum() > 0 && !m_isLoadScene)
		{
			// フラグを立てる
			m_isLoadScene = true;

			// 非同期読み込みがあった場合ロード画面に飛ぶ
			PushScene(std::make_shared<Scene::Load>(shared_from_this()));
		}
	}

	// 現在のシーンの上にシーンを積む(ポーズ)
	void Manager::PushScene(const std::shared_ptr<Scene::Base>& scene)
	{
		// シーンの追加
		m_pSceneTable.push_front(scene);

		// シーンの初期化
		m_pSceneTable.front()->Init();

		// 非同期読み込みのチェック
		CheckAsyncLoad();
	}

	// 一番上のシーンを削除
	void Manager::PopScene()
	{
		// シーンの終了処理
		m_pSceneTable.front()->End();

		// シーンの削除
		m_pSceneTable.pop_front();
	}

	// シーンの全削除
	void Manager::ClearScene()
	{
		// シーンの終了処理
		for (auto& scene : m_pSceneTable)
		{
			scene->End();
		}

		// シーンの全削除
		m_pSceneTable.clear();
	}

	// FPSの描画
	void Manager::DrawFps()
	{
#ifdef _DEBUG
		// 画面サイズの取得
		Size size = Application::GetInstance()->GetWindowSize();

		// fpsの取得
		float nowFps = GetFPS();

		// fpsの更新
		static int count = 0;
		if (count++ % fps_update_interval == 0)
		{
			m_prevFps = nowFps;
		}

		// fpsを描画
		DrawFormatString(300, size.height - 40, 0xffffff, "FPS : %f", m_prevFps);
#endif
	}

	// 更新時間の描画
	void Manager::DrawUpdateTime()
	{
#ifdef _DEBUG
		// 画面サイズの取得
		const auto& size = Application::GetInstance()->GetWindowSize();

		// fpsの取得
		float nowFps = GetFPS();

		// 1フレームにかかった描画時間を表示
		float rate = static_cast<float>(m_updateTime + m_drawTime) / static_cast<long long>(1000000 / nowFps);
		int width = static_cast<int>(size.width * rate);
		DrawBox(16, size.height - 40, 32, size.height - 24, 0xff0000, true);
		DrawString(40, size.height - 40, "DrawTime", 0xffffff);
		DrawBox(0, size.height - 16, width, size.height, 0xff0000, true);
#endif
	}

	// 描画時間の描画
	void Manager::DrwaDrawTime()
	{
#ifdef _DEBUG
		// 画面サイズの取得
		Size size = Application::GetInstance()->GetWindowSize();

		// fpsの取得
		float nowFps = GetFPS();

		// 1フレームにかかった更新時間を表示
		float rate = static_cast<float>(m_updateTime + m_drawTime) / static_cast<long long>(1000000 / nowFps);
		int width = static_cast<int>(size.width * rate);
		rate = static_cast<float>(m_updateTime) / static_cast<long long>(1000000 / nowFps);
		width = static_cast<int>(size.width * rate);
		DrawBox(150, size.height - 40, 166, size.height - 24, 0x0000ff, true);
		DrawString(174, size.height - 40, "UpdateTime", 0xffffff);
		DrawBox(0, size.height - 16, width, size.height, 0x0000ff, true);
#endif
	}
}