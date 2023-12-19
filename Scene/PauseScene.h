#pragma once
#include "SceneBase.h"
#include <array>

// ポーズシーン
class PauseScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	PauseScene(SceneManager& manager);

	// デストラクタ
	~PauseScene();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	// フェード設定データ
	struct FadeData
	{
		int fadeValue;
		int fadeSpeed;
		bool isFade;
		bool isGaussFade;
		bool isOnlyStringFade;
	};
private:
	// 項目
	enum class Item
	{
		CONTINUE,		// 続ける
		OPTION,			// 設定シーン
		TITLE,			// タイトルシーン
		TOTAL_VALUE		// 項目の合計値
	};
private:
	// フェードの設定データテーブル
	std::array<FadeData, static_cast<int>(Item::TOTAL_VALUE)> fadeConfigTable_;

	// 選択中の項目
	int currentSelectItem_;
};

