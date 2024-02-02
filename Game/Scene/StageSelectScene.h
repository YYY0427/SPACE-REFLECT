#pragma once
#include "SceneBase.h"
#include <memory>

// プロトタイプ宣言
class Camera;

// ステージ項目
enum class Stage
{
	TUTORIAL,	// チュートリアル
	STAGE_1,	// ステージ1
	EXIT,		// 終了
	NUM			// 項目数
};

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャー</param>
	StageSelectScene(SceneManager& manager);

	// デストラクタ
	~StageSelectScene();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// ポインタ
	std::unique_ptr<Camera> m_pCamera;

	// 選択されている項目
	int m_currentSelectItem;
};

