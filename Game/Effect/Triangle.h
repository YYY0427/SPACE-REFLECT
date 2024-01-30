#pragma once
#include "../Math/Vector3.h"
#include "../Math/Vector2.h"
#include "../Util/Timer.h"
#include <array>
#include <vector>

/// <summary>
/// ボス死亡時の演出に使用する三角形
/// </summary>
class Triangle
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="frame">何フレームで演出を行うか</param>
	Triangle(int frame);

	// デストラクタ
	~Triangle();

	// 更新
	void Update(Vector3 bossEnemyPos);

	// 描画
	void Draw();

	// 終了したかどうか
	bool IsEnd() const;

private:
	// 三角形のデータ
	struct TriangleData
	{
		std::array<Vector2, 3> pos;		// 位置
		float alpha;					// アルファ値
	};

private:
	// 三角形のデータテーブル
	std::vector<TriangleData> m_triangleDataTable;

	// 角度
	float m_angle;	

	// 三角形の色
	Vector3 m_color;

	// 何フレームで演出を行うか
	Timer<int> m_frame;	

	// 三角形を描画する間隔のフレーム
	Timer<int> m_intervalFrame;	
};