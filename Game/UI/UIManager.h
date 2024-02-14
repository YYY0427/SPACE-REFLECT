#pragma once
#include <string>
#include <vector>
#include <memory>
#include "UIBase.h"
#include "../Math/Vector2.h"

/// <summary>
/// UIの管理クラス
/// </summary>
class UIManager
{
public:
	// デストラクタ
	~UIManager();

	// UIの更新
	void Update();

	// UIの描画
	void Draw();

	/// <summary>
	/// UIManagerの唯一のインスタンスを返す
	/// </summary>
	/// <returns>唯一の実態の参照</returns>
	static UIManager& GetInstance();

	/// <summary>
	/// UIの追加
	/// </summary>
	/// <param name="key">キー</param>
	/// <param name="ui">UIのポインタ</param>
	/// <param name="drawOrder">描画順</param>
	/// <param name="storeVec">UIを格納するベクトル</param>
	void AddUI(std::string key, std::shared_ptr<UIBase> ui, int drawOrder, Vector2 storeVec = {});

	// UIの削除
	void DeleteUI(std::string key);

	// すべてのUIの削除
	void DeleteAllUI();

	// UIの格納
	void Store();

private:
	// コンストラクタ
	// シングルトンパターンなのでprivate
	UIManager();

	// コピーも代入も禁止
	UIManager(const UIManager&) = delete;			// コピーコンストラクタ禁止
	void operator = (const UIManager&) = delete;	// 代入禁止

private:
	// UIのデータ
	struct UIData
	{
		std::string key;			// キー
		std::shared_ptr<UIBase> ui; // UIのポインタ
		int drawOrder;				// 描画順
		Vector2 storeVec;			// UIを格納するベクトル
	};

private:
	// UIのテーブル
	std::vector<UIData> m_uiTable;
};