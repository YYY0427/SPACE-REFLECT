#pragma once
#include "SceneBase.h"
#include <vector>

// パッドの設定を行うシーン
class PadSettingScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	PadSettingScene(SceneManager& manager);

	// デストラクタ
	~PadSettingScene();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	/// <summary>
	/// パッドの反転のオン、オフの表示
	/// </summary>
	/// <param name="isOn">パッドの反転がオンかどうか</param>
	/// <param name="item">項目の番号</param>
	void DrawPadReverse(bool isOn, int item);

private:
	// 項目
	enum class Item
	{
		PAD_SENS_X,		// パッドスティックの横感度
		PAD_SENS_Y,		// パッドスティックの縦感度
		PAD_REVERSE_X,	// パッドスティックの横反転
		PAD_REVERSE_Y,	// パッドスティックの縦反転
		BACK,			// 戻る
		TOTAL_VALUE		// 項目の合計値
	};
private:
	// 選択中の項目
	int currentSelectItem_;

	// 項目の描画色データ
	std::vector<int> itemColorTable_;
};