#include "LaserBase.h"
#include "../../Model.h"
#include "../../Effect/Effekseer3DEffectManager.h"

// コンストラクタ
LaserBase::LaserBase() :
	m_isEnabled(true),
	m_isReflect(false),
	m_speed(0.0f),
	m_opacity(1.0f),
	m_reflectLaserKey(-1)
{
}

// デストラクタ
LaserBase::~LaserBase()
{
}

// レーザーの削除
void LaserBase::Delete()
{
	m_isEnabled = false;
}

// 位置の設定
void LaserBase::SetPos(const Vector3& pos)
{
	m_pos = pos;
}

// 反射レーザーのキーの設定
void LaserBase::SetReflectLaserKey(const int key)
{
	m_reflectLaserKey = key;
}

// モデルハンドルの取得
int LaserBase::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

// 反射レーザーのキーの取得
int LaserBase::GetReflectLaserKey() const
{
	return m_reflectLaserKey;
}

// 位置情報の取得
const Vector3& LaserBase::GetPos() const
{
	return m_pos;
}

// モデルの先端の位置の取得
const Vector3& LaserBase::GetEndPos() const
{
	return m_endPos;
}

// 方向ベクトルの取得
Vector3 LaserBase::GetDirection() const
{
	return { 0, 0, 0 };
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
