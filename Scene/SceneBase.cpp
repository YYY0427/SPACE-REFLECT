#include "SceneBase.h"
#include "../UI/Fade.h"
#include <cmath>

// コンストラクタ
SceneBase::SceneBase(SceneManager& manager) :
	m_manager(manager),
	m_pFade(std::make_unique<Fade>())
{
}

// デストラクタ
SceneBase::~SceneBase()
{
}
