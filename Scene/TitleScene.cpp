#include "TitleScene.h"
#include "DebugScene.h"
#include "SceneManager.h"
#include "../InputState.h"
#include "../Application.h"
#include "../Transitor/WipeTransitor.h"
#include "DxLib.h"

TitleScene::TitleScene(SceneManager& manager) :
	SceneBase(manager)
{
	m_pTransitor = std::make_unique<WipeTransitor>();
	m_pTransitor->Start();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Update()
{
	if (InputState::IsTriggered(InputType::DECISION))
	{
		m_manager.ChangeScene(std::make_shared<DebugScene>(m_manager));
		return;
	}

	m_pTransitor->Update();
}

void TitleScene::Draw()
{
	ClearDrawScreen();

	// ウィンドウサイズの取得
	auto& size = Application::GetInstance().GetWindowSize();	

	DrawBox(0, 0, size.width, size.height, 0xff0000, true);
	m_pTransitor->Draw();
}
