#pragma once

/// <summary>
/// ステージ基底クラス
/// </summary>
class StageBase
{
public:
	// コンストラクタ
	StageBase();

	// デストラクタ
	virtual ~StageBase();

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Draw() = 0;

protected:
};