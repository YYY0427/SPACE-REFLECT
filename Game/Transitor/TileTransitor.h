#pragma once
#include "TransitorBase.h"
#include <vector>
#include <random>

/// <summary>
/// 画面切り替え演出
/// タイル状に画面が切り替わる
/// </summary>
class TileTransitor final : public TransitorBase
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="cellSize">タイルの大きさ</param>
    /// <param name="interval">切り替わる時間</param>
    TileTransitor(int cellSize = 50, int interval = 60);

    // 更新
    void Update() override final;

    // 描画
    void Draw() override final;

private:
    // タイルのXYインデックス
    struct XYIdx 
    {
        int xIdx, yIdx;
    };

private:
    // 乱数エンジン
    std::mt19937 m_mt;

    // タイルのXYインデックステーブル
    std::vector<XYIdx> m_tiles;

    // タイルの大きさ
    int m_cellSize;
};