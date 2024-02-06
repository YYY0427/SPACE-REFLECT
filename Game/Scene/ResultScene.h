#pragma once
#include "SceneBase.h"
#include "../Math/Vector2.h"
#include <string>

/// <summary>
/// リザルト画面
/// </summary>
class ResultScene : public SceneBase
{
public:
	// コンストラクタ
	ResultScene(SceneManager& manager, std::string stageName);

	// デストラクタ
	~ResultScene();

	// 更新
	void Update() override final;

	// 描画
	void Draw() override final;

private:
	// ウィンドウの位置
	Vector2 m_windowPos;

	// ステージ名
	std::string m_stageName;
};