#include "TestScene.h"
#include "../Application.h"
#include "../Transitor/TileTransitor.h"
#include "../Transitor/FadeTransitor.h"
#include "../Transitor/IrisTransitor.h"
#include "../Transitor/WipeTransitor.h"
#include "../Transitor/PushTransitor.h"
#include "../Transitor/StripTransitor.h"
#include "../UI/Warning.h"
#include "../Util/FileUtil.h"
#include "../UI/ResultWindow.h"
#include "../Math/MathUtil.h"
#include "../MyDebug/DebugText.h"
#include "DxLib.h"

namespace
{
	const std::string file_extension = ".bmp";
}

// �R���X�g���N�^
TestScene::TestScene(SceneManager& manager) :
	SceneBase(manager)
{
}

// �f�X�g���N�^
TestScene::~TestScene()
{
}

// ������
void TestScene::Init()
{
	// ��ʐ؂�ւ����o�̐ݒ�
	m_pTransitor = std::make_unique<StripTransitor>();
	m_pTransitor->Start();
}

// �I������
void TestScene::End()
{
}

// �X�V
void TestScene::Update()
{
	// ��ʐ؂�ւ����o�̍X�V
	m_pTransitor->Update();
}

// �`��
void TestScene::Draw()
{
	// ��ʂ��N���A
	ClearDrawScreen();

	// �w�i�̕`��
	const auto& size = Application::GetInstance().GetWindowSize();
	DrawBox(0, 0, size.width, size.height, 0x000000, true);
	
	static int count = 0;
	count++;
	if (count % 10 == 0)
	{
		Debug::Text::AddLog("TEST", { Math::Util::GetRandFloat(0.1f, 1.0) });
	}

	//// �ǂݍ��񂾃t�@�C���̕`��
	//for (int i = 0; i < m_fileNames.size(); i++)
	//{
	//	for (int j = 0; j < m_fileNames[i].size(); j++)
	//	{
	//		DrawString(20 * j, 50 + (i * 20), m_fileNames[i][j].c_str(), 0xffffff);
	//	}
	//}

	//int i = 0;
	//for (int i = 0; i < m_fileNames.size(); i++)
	//{
	//	// �g���q���폜
	//	m_fileNames[i] = m_fileNames[i].substr(0, m_fileNames[i].size() - file_extension.size());

	//	// �t�@�C�����̕`��
	//	DrawString(0, 50 + (i * 20), m_fileNames[i].c_str(), 0xffffff);
	//}

	// ��ʐ؂�ւ����o�̕`��
	m_pTransitor->Draw();
}
