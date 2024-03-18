#pragma once
#include <memory>
#include <list>
#include "Meteor.h"
#include "../Math/Vector3.h"

// プロトタイプ宣言
class Player;

/// <summary>
/// 隕石の管理クラス
/// </summary>
class MeteorManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="objectDataFileName">オブジェクトデータのファイル名</param>
	/// <param name="pPlayer">プレイヤーのポインタ</param>
	MeteorManager(const std::string& objectDataFileName, const std::shared_ptr<Player>& pPlayer);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MeteorManager();

	/// <summary>
	/// ゲームスタート時の更新
	/// </summary>
	void UpdateStart();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="cameraPos">カメラの座標</param>
	void Update(const Vector3& cameraPos);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 小さい隕石の生成
	/// </summary>
	void CreateSmallMeteor(const float playerRelativePos);

	/// <summary>
	/// 全ての隕石の削除
	/// </summary>
	void DeleteAllMeteor();

	/// <summary>
	/// 隕石リストの取得
	/// </summary>
	/// <returns>隕石リスト</returns>
	const std::list<std::shared_ptr<Meteor>>& GetMeteor() const;

private:
	// 隕石リスト
	std::list<std::shared_ptr<Meteor>> m_pMeteorList;

	// ポインタ
	std::shared_ptr<Player> m_pPlayer;

	// 生成間隔フレーム
	int m_smallCreateIntervalFrame;
};