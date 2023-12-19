#pragma once
#include "TransitorBase.h"
class IrisTransitor : public TransitorBase
{
public:
	IrisTransitor(bool irisOut = false, int interval = 60, bool isTiled = false, int gHandle = -1);
	~IrisTransitor();

	void Update() override;
	void Draw() override;
private:
	// マスク転送用グラフィックハンドル
	int m_handleForMaskScreen;

	// マスクハンドル
	int m_maskH;

	// 対角線の長さ
	float m_diagonalLength;

	// アイリスアウトフラグ
	bool m_irisOut;

	int m_gHandle;
	bool m_isTiled;
};