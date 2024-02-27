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
	Triangle(const int frame);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Triangle();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="bossEnemyPos">ボス敵の位置</param>
	void Update(const Vector3& bossEnemyPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了したか
	/// </summary>
	/// <returns>true : 終了、false : 途中</returns>
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