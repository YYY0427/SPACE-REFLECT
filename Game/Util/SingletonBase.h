#pragma once
#include <memory>

/// <summary>
/// シングルトンクラスの基底クラス
/// </summary>
template<class T>
class SingletonBase
{
public:
	/// <summary>
	/// インスタンスを取得する
	/// 初めて取得する場合はインスタンスを作成する
	/// </summary>
	/// <returns>インスタンス</returns>
	static T* GetInstance()
	{
		// インスタンスがない場合は作成する
		if (!m_pInstance)
		{
			// インスタンスを作成
			m_pInstance = new T();
		}
		// インスタンスを返す
		return m_pInstance;
	}

	/// <summary>
	/// インスタンスを破棄する
	/// </summary>
	static void DeleteInstance()
	{
		// インスタンスがある場合は破棄する
		if (m_pInstance)
		{
			// インスタンスを破棄
			delete m_pInstance;

			// インスタンスをnullptrにする
			m_pInstance = nullptr;
		}
	}

protected:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SingletonBase() {}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SingletonBase() {}

private:
	// コピー禁止
	SingletonBase(const SingletonBase&) = delete;

	// 代入禁止
	SingletonBase& operator=(const SingletonBase&) = delete;

	// インスタンス
	static T* m_pInstance;
};

// インスタンスの実体化
template <typename T> T* SingletonBase<T>::m_pInstance;