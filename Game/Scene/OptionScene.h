#pragma once
#include "SceneBase.h"
#include "../StateMachine.h"
#include <vector>

/// <summary>
/// 設定シーン
/// </summary>
class OptionScene final : public SceneBase
{
public:
	// ステート
	enum class State
	{
		STAGE_SELECT,	// ステージセレクト
		PAUSE,			// オプション
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	OptionScene(SceneManager& manager, State state);
	
	// デストラクタ
	~OptionScene();
	
	// 更新
	void Update() override final;
	void UpdateStageSelect();
	void UpdatePause();
	
	// 描画
	void Draw() override final;
	void DrawStageSelect();
	void DrawPause();

private:
	// 設定項目
	enum class OptionItem
	{
		WINDOW_MODE,	// ウィンドウモード
		MASTER_VOLUME,	// マスターボリューム
		BGM_VOLUME,		// BGMボリューム
		SE_VOLUME,		// SEボリューム
		NUM				// 項目数
	};

private:
	// ステートマシン
	StateMachine<State> m_updateStateMachine;
	StateMachine<State> m_drawStateMachine;

	// 選択中の項目
	int m_currentSelectItem;

	// 画像ハンドル
	int m_soundIconHandle;
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;

	// 項目の描画色データ
	std::vector<unsigned int> m_itemColorTable;
};