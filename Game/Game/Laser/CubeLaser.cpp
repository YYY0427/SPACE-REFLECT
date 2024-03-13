#include "CubeLaser.h"
#include "../Player.h"
#include "../Camera.h"
#include "../../Application.h"
#include "../../Model.h"
#include "../../Math/MathUtil.h"
#include "../../ModelHandleManager.h"
#include <DxLib.h>
#include <cassert>

namespace
{
	// 拡大速度
	constexpr float scale_speed = 0.00025f;

	// 不透明度の上昇速度
	constexpr float opacity_speed = 0.005f;

	// 不透明度の最大値
	constexpr float max_opacity = 1.0f;

	// 1フレームに回転する角度
	const float max_delta_rot = 1.0f;  // 最大
	const float min_delta_rot = 0.1f;  // 最小

	// レーザーの拡大率
	constexpr float max_scale = 5.0f;	// 最大
	constexpr float min_scale = 1.0f;	// 最小

	// スクリーンサイズ
	const auto& screenSize = Application::GetInstance().GetWindowSize();

	// モデルの位置を少しずらす範囲
	constexpr float model_pos_range = 5.0f;

	// モデルの不透明度をランダムで変える範囲
	constexpr float model_max_opacity_range = 0.7f;
	constexpr float model_min_opacity_range = 0.0f;

	// ガウスの使用ピクセル幅(8, 16, 32 の何れか)
	constexpr int gauss_pixel_width = 8;

	// ガウスのぼかしパラメータ(100 で約1ピクセル分の幅)
	constexpr int gauss_param = 200;

	// 何個モデルを描画するか
	constexpr int model_num = 10;

	// レーザーの中心の色
	const COLOR_F laser_center_color = GetColorF(255, 255, 255, 255);

	// レーザーの中心の色の不透明度
	constexpr float laser_center_opacity = 1.0f;

	// 加算合成の回数
	constexpr int add_blend_num = 2;

	// 加算合成の割合
	constexpr int add_blend_rate = 255;
}

// コンストラクタ
CubeLaser::CubeLaser(const Vector3& firePos, 
				     const float laserSpeed, 
					 const std::shared_ptr<Player>& pPlayer,
					 const std::shared_ptr<Camera>& pCamera)
{
	// 初期化
	m_pos     = firePos;
	m_pPlayer = pPlayer;
	m_pCamera = pCamera;
	m_opacity = 0.0f;	
	m_moveVec = (m_pPlayer->GetPos() - m_pos).Normalized() * laserSpeed;

	// 1フレームに回転する量をランダムで取得
	m_deltaRot.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));
	m_deltaRot.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_delta_rot, max_delta_rot));

	// 最大拡大率をランダムで取得
	m_maxScale.x = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));
	m_maxScale.y = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));
	m_maxScale.z = MathUtil::ToRadian(MathUtil::GetRandFloat(min_scale, max_scale));

	// ガウス用スクリーンの作成
	m_gaussScreen = MakeScreen(screenSize.width, screenSize.height);

	// モデル設定
	m_pModel = std::make_unique<Model>(ModelHandleManager::GetInstance().GetHandle("CubeLaser"));
	m_pModel->SetUseCollision(true);	// 当たり判定を使用する
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(m_rot);			// 回転情報
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetPos(m_pos);			// 位置情報
	m_pModel->Update();					// 当たり判定の更新
}

// デストラクタ
CubeLaser::~CubeLaser()
{
	// ガウス用スクリーンの削除
	DeleteGraph(m_gaussScreen);
}

// 更新
void CubeLaser::Update()
{
	// 不透明度を加算
	m_opacity = (std::min)(m_opacity + opacity_speed, max_opacity);

	// 移動
	m_pos.z += m_pPlayer->GetMoveVec().z;
	m_pos   += m_moveVec;

	// 拡大
	m_scale += scale_speed;

	// 拡大率の制限
	m_scale.x = (std::min)(m_scale.x, m_maxScale.x);
	m_scale.y = (std::min)(m_scale.y, m_maxScale.y);
	m_scale.z = (std::min)(m_scale.z, m_maxScale.z);

	// 回転
	m_rot += m_deltaRot;

	// モデル設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetRot(m_rot);			// 回転情報
	m_pModel->SetScale(m_scale);		// 拡大率
	m_pModel->SetPos(m_pos);			// 位置情報
	m_pModel->Update();					// 当たり判定の更新
}

// 描画
void CubeLaser::Draw()
{
	// 描画スクリーンを取得して保存しておく
	int screen = GetDrawScreen();

	// ガウス用スクリーンに描画
	SetDrawScreen(m_gaussScreen);

	// 画面をクリア
	ClearDrawScreen();

	for (int i = 0; i < model_num; i++)
	{
		// モデルの位置を少しずらして描画
		Vector3 pos;
		pos.x = m_pos.x + MathUtil::GetRandFloat(-model_pos_range, model_pos_range);
		pos.y = m_pos.y + MathUtil::GetRandFloat(-model_pos_range, model_pos_range);
		pos.z = m_pos.z + MathUtil::GetRandFloat(-model_pos_range, model_pos_range);
		m_pModel->SetPos(pos);

		// 不透明度を少し変えて描画
		float opacity = MathUtil::GetRandFloat(model_min_opacity_range, model_max_opacity_range);
		m_pModel->SetOpacity(opacity);

		// モデルを描画
		m_pModel->Draw();

		// もとに戻す
		m_pModel->SetPos(m_pos);
		m_pModel->SetOpacity(m_opacity);
	}
	// ガウスフィルターの適用
	GraphFilter(m_gaussScreen, DX_GRAPH_FILTER_GAUSS, gauss_pixel_width, gauss_param);

	// レーザーの白い部分の描画
	m_pModel->SetAllMaterialDifColor(laser_center_color);
	m_pModel->SetOpacity(laser_center_opacity);
	m_pModel->Draw();

	// もとに戻す
	m_pModel->RestoreAllMaterialDifColor();
	m_pModel->SetOpacity(m_opacity);

	// 描画スクリーンを戻す
	SetDrawScreen(screen);

	// 加算合成
	SetDrawBlendMode(DX_BLENDMODE_ADD, add_blend_rate);
	for (int i = 0; i < add_blend_num; i++)
	{
		// さっき作成したガウス画像を描画
		DrawGraph(0, 0, m_gaussScreen, true);
	}
	// 加算合成を解除
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}