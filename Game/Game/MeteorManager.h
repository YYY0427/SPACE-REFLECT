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
	void UpdateStart(Vector3 playerVec);
	void Update(Vector3 cameraPos);

	// 描画
	void Draw();

	// 隕石の生成
	void SmallMeteorCreate(Vector3 playerPos);
	void CreateMeteor(int createIntervalFrame, Vector3 playerPos);

	// 隕石の削除
	void DeleteMeteor();

	// 隕石の取得
	const std::list<std::shared_ptr<Meteor>>& GetMeteor() const;

private:
	// 隕石
	std::list<std::shared_ptr<Meteor>> m_pMeteorList;

	// 生成間隔
	int m_createIntervalFrame;
	int m_smallCreateIntervalFrame;
};