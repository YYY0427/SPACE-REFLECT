#include "SkyDome.h"
#include "../Model.h"
#include <string>

namespace
{
	// モデルのファイルパス
	const std::string model_file_path = "Data/Model/SkyDome.mv1";

	// モデルの拡大率
	const Vector3 model_scale = { 100, 100, 100 };
}

// コンストラクタ
SkyDome::SkyDome(Vector3 cameraPos) :
	m_pos(cameraPos)
{
	// モデルのインスタンスの作成
	m_pModel = std::make_unique<Model>(model_file_path);

	// モデルの位置を設定
	m_pModel->SetPos(m_pos);

	// モデルの回転を設定
	m_pModel->SetRot({ 0, 0, 0 });

	// モデルの拡大率を設定
	m_pModel->SetScale(model_scale);
}

// デストラクタ
SkyDome::~SkyDome()
{
}

// 更新
void SkyDome::Update(Vector3 cameraPos)
{
	m_pos.z = cameraPos.z;

	// モデルの位置をプレイヤーの位置に合わせる
	m_pModel->SetPos(m_pos);
}

// 描画
void SkyDome::Draw()
{
	m_pModel->Draw();
}

// 座標の設定
void SkyDome::SetPos(Vector3 pos)
{
	m_pos = pos;
	m_pModel->SetPos(m_pos);
}