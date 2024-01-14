#include "LaserBase.h"

// コンストラクタ
LaserBase::LaserBase() :
	m_isEnabled(true),
	m_isReflect(false)
{
}

// デストラクタ
LaserBase::~LaserBase()
{
}

// 存在フラグの取得
bool LaserBase::IsEnabled() const
{
	return m_isEnabled;
}

// 反射フラグの取得
bool LaserBase::IsReflect() const
{
	return m_isReflect;
}
