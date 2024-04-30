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
	OptionScene(SceneManager& manager, const State state);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~OptionScene();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override final;

	/// <summary>
	/// 終了処理
	/// </summary>
	void End() override final;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override final;

	/// <summary>
	/// ステージセレクト時のオプションの更新
	/// </summary>
	void UpdateStageSelect();

	/// <summary>
	/// ポーズ時のオプションの更新
	/// </summary>
	void UpdatePause();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// ステージセレクト時のオプションの描画
	/// </summary>
	void DrawStageSelect();

	/// <summary>
	/// ポーズ時のオプションの描画
	/// </summary>
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

	// どのシーンから遷移してきたか
	State m_state;

	// 選択中の項目
	int m_currentSelectItem;

	// モザイク処理用スクリーン
	int m_gaussScreen;

	// 画像ハンドル
	int m_soundIconHandle;
	int m_rbButtonImgHandle;
	int m_lbButtonImgHandle;

	// 項目の描画色データ
	std::vector<unsigned int> m_itemColorTable;
};