#pragma once
#include "TransitorBase.h"
#include<vector>
#include<random>

class TileTransitor : public TransitorBase
{
public:
    TileTransitor(int cellSize = 50, int interval = 60);
    void Update() override;
    void Draw() override;

private:
    struct XYIdx 
    {
        int xidx, yidx;
    };

private:
    int cellSize_ = 50;
    std::mt19937 mt_;
    std::vector<XYIdx> tiles_;
};