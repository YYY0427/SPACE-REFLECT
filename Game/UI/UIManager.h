#pragma once
#include <string>
#include <vector>
#include <memory>
#include "UIBase.h"
#include "../Util/SingletonBase.h"
#include "../Math/Vector2.h"

namespace UI
{
	/// <summary>
	/// UIの管理するシングルトンクラス
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<Manager>;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~Manager(){}

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// UIの追加
		/// </summary>
		/// <param name="key">キー</param>
		/// <param name="ui">UIのポインタ</param>
		/// <param name="drawOrder">描画順</param>
		/// <param name="storeVec">UIを格納するベクトル</param>
		void AddUI(const std::string& key, const std::shared_ptr<UIBase>& ui, const int drawOrder, const Math::Vector2& storeVec = {});

		/// <summary>
		/// UIの削除
		/// </summary>
		/// <param name="key">削除したいUIのキー</param>
		void DeleteUI(const std::string& key);

		/// <summary>
		/// 全てのUIの削除
		/// </summary>
		void DeleteAllUI();

		/// <summary>
		/// UIの格納
		/// </summary>
		void Store();

	private:
		/// <summary>
		/// コンストラクタ 
		/// シングルトンパターンなのでprivate
		/// </summary>
		Manager(){}

	private:
		// UIのデータ
		struct UIData
		{
			std::string key;			// キー
			std::shared_ptr<UIBase> ui; // UIのポインタ
			int drawOrder;				// 描画順
			Math::Vector2 storeVec;			// UIを格納するベクトル
		};

	private:
		// UIのテーブル
		std::vector<UIData> m_uiTable;
	};
}