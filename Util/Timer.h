#pragma once
#include <algorithm>

// タイマークラス
template<class T>
class Timer
{
public: 
	// コンストラクタ
	Timer() :
		m_time(0),
		m_limitTime(0)
	{
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="limitTime">制限時間</param>
	Timer(T limitTime) :
		m_time(0),
		m_limitTime(limitTime)
	{
	}

	// デストラクタ
	virtual ~Timer() {};

	/// <summary>
	/// タイマーの更新
	/// </summary>
	/// <param name="deltaTime">タイマーを1フレーム毎に更新する値</param>
	void Update(T deltaTime)
	{
		// 更新
		m_time += deltaTime;

		//if (limitTime_ > 0)
		//{
		//	// 制限時間は超えない
		//	time_ = (std::min)(time_, limitTime_);
		//}
	}

	// タイマーの初期化
	void Reset()
	{
		m_time = 0;
	}

	/// <summary>
	/// タイムアウトかどうか
	/// 制限時間を設定していない場合、常にfalseを返す
	/// </summary>
	/// <returns>true : タイムアウト, false : タイム内</returns>
	bool IsTimeOut() const
	{
		// 制限時間を設定していない場合、タイムアウトを常に返す
		if (m_limitTime <= 0)
		{
			return true;
		}

		return m_time >= m_limitTime;
	}

	/// <summary>
	/// タイムの取得
	/// </summary>
	/// <returns>タイム</returns>
	T GetTime() const { return m_time; }

	/// <summary>
	/// タイムの設定
	/// </summary>
	/// <param name="limitTime">設定したいタイム</param>
	void SetTime(T time) { m_time = time; }

	// 制限時間の取得
	T GetLimitTime() const { return m_limitTime; }
	
	// 制限時間の設定
	void SetLimitTime(T limitTime) { m_limitTime = limitTime; }

private:
	// 現在の時間
	T m_time;

	// 制限時間
	T m_limitTime;
};