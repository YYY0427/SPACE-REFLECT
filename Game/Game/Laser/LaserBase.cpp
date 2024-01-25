#include "LaserBase.h"
#include "../../Model.h"
#include "../../Effect/Effekseer3DEffectManager.h"

// コンストラクタ
LaserBase::LaserBase() :
	m_isEnabled(true),
	m_isReflect(false),
	m_speed(0.0f),
	m_opacity(1.0f)
{
}

// デストラクタ
LaserBase::~LaserBase()
{
}

// 消す
void LaserBase::Delete()
{
	m_isEnabled = false;
}

// モデルハンドルの取得
int LaserBase::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

// 位置情報の取得
Vector3 LaserBase::GetPos() const
{
	return m_pos;
}

// 方向ベクトルの取得
Vector3 LaserBase::GetDirection() const
{
	return Vector3();
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
