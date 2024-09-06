#include "LoadScene.h"
#include "../MyDebug/DebugText.h"
#include "../Input/InputManager.h"
#include <DxLib.h>

namespace
{
	// ロード画面が表示される最低時間
	// ロード画面が一瞬だけ表示されるのを防ぐため
	constexpr int lowest_time_load_scene = 120;
}

namespace Scene
{
	// コンストラクタ
	Load::Load(const std::shared_ptr<Scene::Manager>& pSceneManager, const bool isEternal) :
		Scene::Base(pSceneManager),
		m_isDrawEternal(isEternal),
		m_time(0)
	{
	}

	// デストラクタ
	Load::~Load()
	{
	}

	// 初期化
	void Load::Init()
	{
	}

	// 終了処理
	void Load::End()
	{
	}

	// 更新
	void Load::Update()
	{
		// 現在のシーン名を表示
		Debug::Text::AddLog("LoadScene");

		// 永続表示でない場合
		if (!m_isDrawEternal)
		{
			// 経過時間を表示
			Debug::Text::AddLog("Time", { m_time });

			// 非同期代読み込みを行っていなく、
			// ロード画面が表示される最低時間を過ぎたら
			if (GetASyncLoadNum() <= 0 &&
				m_time++ >= lowest_time_load_scene)
			{
				// ロード画面を削除することを通知
				m_pSceneManager->SetLoadSceneFlag(false);

				// ロード画面を削除
				m_pSceneManager->PopScene();
			}
		}
		// 永続表示の場合
		else
		{
			// 戻るボタンが押されたら
			if (Input::Manager::IsTriggered(Input::Type::BACK))
			{
				// ロード画面を削除
				m_pSceneManager->PopScene();
			}
		}
	}

	// 描画
	void Load::Draw()
	{
		DrawBox(0, 0, 1920, 1080, 0x000000, true);
	}
}
