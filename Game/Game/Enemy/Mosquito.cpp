#include "Mosquito.h"
#include "../../Model.h"

// コンストラクタ
Mosquito::Mosquito(int modelHandle)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_shared<Model>(modelHandle);
}

// デストラクタ
Mosquito::~Mosquito()
{
}

// 更新
void Mosquito::Update()
{
	m_pModel->Update();
}

// 描画
void Mosquito::Draw()
{
	m_pModel->Draw();
}