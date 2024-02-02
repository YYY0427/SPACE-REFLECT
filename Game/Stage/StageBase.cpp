#include "StageBase.h"
#include "../Score/ScoreRanking.h"
#include "../Score/Score.h"
#include "../Util/InputState.h"
#include "../Scene/StageSelectScene.h"
#include "../Scene/SceneManager.h"

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
void StageBase::UpdateResult(std::string stageName, std::string playerName)
{
	// 決定ボタンが押されたら
	if (InputState::IsTriggered(InputType::DECISION))
	{
		// スコアランキングの更新
		ScoreRanking::GetInstance().AddScore(stageName, playerName.c_str(), Score::GetInstance().GetTotalScore());

		// シーン遷移
		m_manager.ChangeScene(std::make_shared<StageSelectScene>(m_manager));
	}
}