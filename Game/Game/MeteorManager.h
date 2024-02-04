#pragma once
#include <memory>
#include <list>
#include "Meteor.h"
#include "../Math/Vector3.h"

/// <summary>
/// 隕石の管理クラス
/// </summary>
class MeteorManager
{
public:
	// コンストラクタ
	MeteorManager(std::string objectDataFileName);

	// デストラクタ
	~MeteorManager();

	// 更新
	void Update(Vector3 cameraPos);

	// 描画
	void Draw();

	// 隕石の生成
	void CreateMeteor(int createIntervalFrame, Vector3 playerPos);

	// スローの値の設定
	void SetSlowValue(float slowValue);

	// 隕石の取得
	const std::list<std::shared_ptr<Meteor>>& GetMeteor() const;

private:
	// 隕石
	std::list<std::shared_ptr<Meteor>> m_pMeteorList;

	// 隕石のモデルハンドル
	int m_modelHandle;	

	// 生成間隔
	int m_createIntervalFrameTimer;
};