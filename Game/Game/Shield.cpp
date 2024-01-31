#include "Shield.h"
#include "Player.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Image3D.h"
#include "../UI/Gauge.h"
#include "../UI/UIManager.h"
#include "../UI/ImageUI.h"
#include "../Util/InputState.h"
#include "../Util/Range.h"
#include "../Util/Timer.h"
#include "../Math/MathUtil.h"
#include "../Math/Matrix.h"
#include "../Math/Vector2.h"
#include <string>

namespace
{
	// シールド画像のファイルパス
	const std::string img_file_path = "Data/Image/Shield.png";	

	// シールド画像のサイズ
	constexpr float shield_width = 120.0f;	// 横幅サイズ
	constexpr float shield_height = 120.0f;	// 縦幅サイズ

	// シールドのエフェクトの拡大率
	const Vector3 effect_scale = { 80.0f,  80.0f, 80.0f, };	

	// エネルギーゲージUIの画像のファイルパス
	const std::string enerugy_gage_ui_file_path = "Data/Image/EnerugyBar.png";	

	// エネルギーゲージUIの背景画像のファイルパス
	const std::string enerugy_gage_ui_back_file_path = "Data/Image/StatusBase.png";

	// エネルギーゲージUIの位置
	const Vector2 enerugy_gage_ui_pos = { 215, 660 };

	// エネルギーゲージUIのサイズ
	const Vector2 enerugy_gage_ui_size = { 300, 13 };

	// エネルギー文字のファイルパス
	const std::string enerugy_string_file_path = "Data/Image/stamina.png";

	// エネルギー文字の位置
	const Vector2 enerugy_string_pos = { 70, 620 };

	// エネルギー文字の拡大率
	const Vector2 enerugy_string_scale = { 0.8f, 0.8f };

	// 最大エネルギーゲージ量
	constexpr int max_enerugy_gage = 1000;

	// プレイヤーからのシールドの距離
	constexpr float player_distance = 50.0f;

	// エネルギーの回復速度
	constexpr int enerugy_recovery_speed = 1;

	// エネルギーの減る速度
	constexpr int enerugy_decrease_speed = 1;
}

// コンストラクタ
Shield::Shield(Player& player) :
	m_player(player),
	m_isInput(false),
	m_effectHandle(-1),
	m_enerugyGage(max_enerugy_gage),
	m_slowValue(1.0f)
{
	// 3D画像のインスタンスの作成
	m_pImage = std::make_shared<Image3D>(img_file_path);

	// エネルギーゲージUIのインスタンスの作成
	m_pEnergyGage = std::make_shared<Gauge>(
		enerugy_gage_ui_file_path,
		enerugy_gage_ui_back_file_path,
		"",
		max_enerugy_gage,
		enerugy_gage_ui_pos,
		enerugy_gage_ui_size,
		true,
		3.0f,
		false,
		0.0f);

	// エネルギーゲージUIの文字の作成
	auto pEnerugyString = std::make_shared<ImageUI>(enerugy_string_file_path);
	pEnerugyString->SetPos(enerugy_string_pos);
	pEnerugyString->SetScale(enerugy_string_scale);

	// UIの登録
	UIManager::GetInstance().AddUI("EnerugyGage", m_pEnergyGage, 0, { 0, 1 });
	UIManager::GetInstance().AddUI("EnerugyString", pEnerugyString, 0, { 0, 1 });

	// シールド画像の初期化
	m_pImage->SetPos(m_player.GetPos());		 // 位置	
	m_pImage->SetImgWidth(shield_width);		 // 横幅
	m_pImage->SetImgHeight(shield_height);		 // 縦幅
}

// デストラクタ
Shield::~Shield()
{
}

// 更新
void Shield::Update()
{
	// プレイヤーが生きているか
	if (m_player.IsLive())
	{
		// 初期化
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(m_effectHandle);

		m_isInput = false;
		const Range<int> enerugyGageRange(0, max_enerugy_gage);

		// 入力されているか
		if (InputState::IsPressed(InputType::SHIELD))
		{
			// 入力されている
			m_isInput = true;
		}

		// 右スティックの入力情報の取得
		int up = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::UP);
		int down = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::DOWN);
		int right = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::RIGHT);
		int left = InputState::IsPadStick(PadLR::RIGHT, PadStickInputType::LEFT);

		// シールドの位置の計算
		Vector3 tempVec = { (right + -left) * 10 * m_slowValue, (up + -down) * 10 * m_slowValue, player_distance };

		// プレイヤーの平行移動行列の取得
		Matrix playerMtx = Matrix::GetTranslate(m_player.GetPos());

		// シールドの相対位置とプレイヤーの平行行列から位置情報を作成
		m_pos = Vector3::Transform(tempVec, playerMtx);
		//	pos_ = { player_.GetPos().x, player_.GetPos().y, player_.GetPos().z + player_distance };

		// ベクトルから角度を求める
		m_rot = -Matrix::ToEulerAngle(Matrix::GetRotationMatrix({0, 0, 1}, tempVec));
		Vector3 effectRot = -Matrix::ToEulerAngle(Matrix::GetRotationMatrix({ 1, 0, 0 }, tempVec));

		// 入力されていたら
		if (m_isInput)
		{
			// エネルギーゲージが残っていたら
			if (m_enerugyGage > 0)
			{
				// シールドを出している間は常にエネルギーゲージを減らす
				m_enerugyGage -= enerugy_decrease_speed * m_slowValue;

				// シールドエフェクトの再生
				/*effectManager.PlayEffect(
					m_effectHandle, 
					EffectID::player_shield, 
					{ m_pos.x, m_pos.y - 100.0f, m_pos.z },
					effect_scale, 
					1.0f, 
					effectRot);*/
			}
		}
		else
		{
			// 入力されていないならエネルギーゲージを回復させる
			m_enerugyGage += enerugy_recovery_speed * m_slowValue;
		}

		// エネルギーゲージの範囲を制限
		m_enerugyGage = enerugyGageRange.Clamp(m_enerugyGage);

		// 画像の設定
		m_pImage->SetPos(m_pos); // 位置
		m_pImage->SetRot(m_rot); // 回転
		m_pImage->Update();		 // 更新

		// エネルギーゲージの設定
		m_pEnergyGage->SetValue(m_enerugyGage);
	}
	else
	{
		// プレイヤーが死んでいたらエフェクトを消す
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(m_effectHandle);
	}
}

// 描画
void Shield::Draw()
{
	// プレイヤーが生きていたら
	// シールドを出していたら
	if (IsShield() && m_player.IsLive())
	{
//#ifdef _DEBUG
		m_pImage->Draw();
//#endif
	}
}

// 位置の取得
Vector3 Shield::GetPos() const
{
	return m_pos;
}

// シールドを使用しているか
bool Shield::IsShield() const
{
	return (m_enerugyGage > 0) && (m_isInput);
}

// シールドの頂点情報の取得
std::array<VERTEX3D, 6> Shield::GetVertex() const
{
	return m_pImage->GetVertex();
}

// スローの値の設定
void Shield::SetSlowValue(float slowValue)
{
	m_slowValue = slowValue;
}