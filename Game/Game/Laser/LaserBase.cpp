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

// レーザーを止める
void LaserBase::Stop(Vector3 pos)
{
	// なにもしない
}

// モデルハンドルの取得
int LaserBase::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
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
