#pragma once
#include "SceneBase.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene final : public SceneBase
{
public:
	/// </summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	TitleScene(SceneManager& manager);
	
	// デストラクタ
	~TitleScene();
	
	// 更新
	void Update() override final;
	
	// 描画
	void Draw() override final;

private:
	// ここから飛べるシーン項目
	enum class SceneItem
	{
		GAME,	// ゲーム
		NUM		// 項目数
	};

private:
	// 現在選択しているシーン項目
	int m_currentSelectSceneItem;

	// アルファ値
	int m_alpha;

	// フレーム
	int m_frame;

	// ガウスハンドル
	int m_gaussHandle;

	// ボタンが押されたかどうか
	bool m_isInput;
};