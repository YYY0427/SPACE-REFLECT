#pragma once
#include "SceneBase.h"
#include <vector>

// 設定シーン
class OptionScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	OptionScene(SceneManager& manager);

	// デストラクタ
	~OptionScene();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	// 項目
	enum class Item
	{
		LANGUAGE,			// 言語
		WINDOW_MODE,		// ウィンドウモード
		MASTER_VOLUME,		// 全体音量
		BGM_VOLUME,			// BGM音量
		SE_VOLUME,			// SE音量
		PAD_SETTING,		// パッド設定シーン
		BACK,				// 戻る
		TOTAL_VALUE			// 項目の合計値
	};

private:
	// 現在選択中の項目
	int currentSelectItem_;

	// 項目の描画色データ
	std::vector<int> itemColorTable_;

	// 音量アイコンの画像ハンドル
	int soundIconImgHandle_;
};