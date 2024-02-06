#include "StageBase.h"
#include "../Scene/SceneManager.h"
#include "../Scene/ResultScene.h"

// コンストラクタ
StageBase::StageBase(SceneManager& manager) :
	m_manager(manager)
{
}

// デストラクタ
StageBase::~StageBase()
{
}

// リザルトの更新
void StageBase::ChangeResultScene(std::string stageName)
{
	// シーン遷移
	m_manager.PushScene(std::make_shared<ResultScene>(m_manager, stageName));
}