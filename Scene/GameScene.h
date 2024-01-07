#pragma once
#include "SceneBase.h"
#include <string>
#include <memory>

// プロトタイプ宣言
class UIManager;
class Player;
class Camera;	
class SkyDome;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene final : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	/// <param name="fileName">ステージファイル名</param>
	GameScene(SceneManager& manager, std::string fileName);
	
	// デストラクタ
	~GameScene();
	
	// 更新
	void Update() override final;
	
	// 描画
	void Draw() override final;

private:
	// ポインタ
	std::shared_ptr<UIManager> m_pUIManager;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<SkyDome> m_pSkyDome;
};

