#pragma once
#include <array>
#include "../Util/SingletonBase.h"

namespace Score
{
	// スコアの種類
	enum class Type
	{
		// 増加項目
		ENEMY,		// 敵を倒したスコア
		BOSS,		// ボスを倒したスコア
		ITEM,		// アイテムを取得したスコア

		// 減少項目
		DAMAGE,		// ダメージをスコア

		// スコアの種類の数
		NUM
	};

	/// <summary>
	/// スコアを管理するシングルトンクラス
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する 
		friend class SingletonBase<Manager>;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Manager();

		/// <summary>
		/// スコアの追加
		/// </summary>
		/// <param name="type">スコアの種類</param>
		void AddScore(const Type type);

		/// <summary>
		/// スコアの描画
		/// </summary>
		void DrawScore() const;

		/// <summary>
		/// スコアの取得
		/// </summary>
		/// <param name="type">スコアの種類</param>
		/// <returns>スコア</returns>
		int GetScore(const Type type) const;

		/// <summary>
		/// 合計スコアの取得
		/// </summary>
		/// <returns>合計スコア</returns>
		int GetTotalScore() const;

		/// <summary>
		/// スコアのリセット
		/// </summary>
		void Reset();

	private:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Manager();

	private:
		// スコア
		std::array<int, static_cast<int>(Type::NUM)> m_score;
	};
}