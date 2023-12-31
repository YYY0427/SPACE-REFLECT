#pragma once
#include "SceneBase.h"
#include <vector>

/// <summary>
/// 設定シーン
/// </summary>
class OptionScene final : public SceneBase
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
	void Update() override final;
	
	// 描画
	void Draw() override final;

private:
	// 設定項目
	enum class OptionItem
	{
		WINDOW_MODE,	// ウィンドウモード
		MASTER_VOLUME,	// マスターボリューム
		BGM_VOLUME,		// BGMボリューム
		SE_VOLUME,		// SEボリューム
		EXIT,			// 終了
		NUM				// 項目数
	};

private:
	// 選択中の項目
	int m_currentSelectItem;

	// サウンドアイコン画像ハンドル
	int m_soundIconHandle;

	// 項目の描画色データ
	std::vector<int> m_itemColorTable;
};