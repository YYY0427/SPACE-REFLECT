#pragma once
#include <algorithm>
#include <cmath>
#include <cassert>

// 下限値と上限値を管理する範囲クラス
template<class T>
class Range
{
public:
	/// <summary>
	/// コンストラクタ
	/// 下限値と上限値の設定
	/// </summary>
	/// <param name="min">下限値</param>
	/// <param name="max">上限値</param>
	Range(const T min, const T max) :
		m_min(min),
		m_max(max)
	{
		// 下限値より上限値のほうが小さい場合止める
		assert(m_min <= m_max);
	}

	/// <summary>
	/// 値が設定した上限値の範囲内かどうか
	/// </summary>
	/// <param name="value">範囲内か調べたい値</param>
	/// <returns> true : 範囲内, false : 範囲内じゃない </returns>
	bool IsUpperLimit(const T value) const
	{
		return (value <= m_max);
	}

	/// <summary>
	/// 値が設定した下限値の範囲内かどうか
	/// </summary>
	/// <param name="value">範囲内か調べたい値</param>
	/// <returns> true : 範囲内, false : 範囲内じゃない </returns>
	bool IsLowerLimit(const T value) const
	{
		return (m_min <= value);
	}

	/// <summary>
	/// 値が設定した下限値と上限値の範囲内かどうか
	/// </summary>
	/// <param name="value">範囲内か調べたい値</param>
	/// <returns> true : 範囲内, false : 範囲内じゃない </returns>
	bool IsInside(const T value) const
	{
		return IsLowerLimit(value) && IsUpperLimit(value);
	}

	/// <summary>
	/// 値が下限値を下回った場合、値を下限値に変更
	/// 値が上限値を上回った場合、値を上限値に変更
	/// </summary>
	/// <param name="value">変更したい値</param>
	/// <returns>変更後の値</returns>
	T Clamp(const T value) const
	{
		return  std::clamp(value, m_min, m_max);
	}

	/// <summary>
	/// 値が下限値を下回った場合、値を上限値に変更
	/// 値が上限値を上回った場合、値を下限値に変更
	/// </summary>
	/// <param name="value">変更したい値</param>
	/// <returns>変更後の値</returns>
	T Wrap(const T value) const
	{
		const float n = std::fmod(value - m_min, m_max - m_min);
		return (n >= 0) ? (n + m_min) : (n + m_max);
	}

	// 設定した下限値の取得
	T GetMinValue() const { return m_min; }

	// 設定した上限値の取得
	T GetMaxValue() const { return m_max; }

private:
	// 下限値
	T m_min;

	// 上限値
	T m_max;
};