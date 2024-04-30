#pragma once
#include "SceneBase.h"
#include "../Math/Vector2.h"
#include <vector>

// プロトタイプ宣言
class Camera;
class SkyDome;

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
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();
	
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
	/// 描画
	/// </summary>
	void Draw() override final;

	/// <summary>
	/// レーザーの設定
	/// </summary>
	/// <param name="pos1">左上の位置</param>
	/// <param name="pos2">右下の位置</param>
	/// <param name="color">レーザーの色</param>
	void SetLaser(const Vector2& pos1, const Vector2& pos2, const unsigned int color);

private:
	// ここから飛べるシーン項目
	enum class SceneItem
	{
		GAME,	// ゲーム
		NUM		// 項目数
	};

private:
	// レーザーのデータ
	struct LaserData
	{
		Vector2 pos1;	
		Vector2 pos2;
		Vector2 startPos;
		float thickness;
		int alpha;
		unsigned int color;
	};

private:
	// ポインタ
	std::shared_ptr<Camera> m_pCamera;
	std::shared_ptr<SkyDome> m_pSkyDome;

	// レーザーのデータ
	std::vector<LaserData> m_laserData;

	// 現在選択しているシーン項目
	int m_currentSelectSceneItem;

	// アルファ値
	int m_alpha;

	// アルファ値の加算値
	int m_alphaAdd;

	// フレーム
	int m_frame;

	// ガウスハンドル
	int m_gaussHandle;

	// ボタンが押されたかどうか
	bool m_isInput;
};