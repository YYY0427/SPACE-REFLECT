#pragma once
#include "../Scene/SceneBase.h"
#include <memory>
#include <string>

/// <summary>
/// ステージ基底クラス
/// </summary>
class StageBase
{
public:
	// コンストラクタ
	StageBase(SceneManager& manager);

	// デストラクタ
	virtual ~StageBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

protected:
	// シーンマネージャー
	SceneManager& m_manager;
};