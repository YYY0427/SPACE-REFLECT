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
	const std::string img_file_path = "Data/Image/Shield1.png";	

	// シールド画像のサイズ
	constexpr float shield_width = 110.0f;	// 横幅サイズ
	constexpr float shield_height = 110.0f;	// 縦幅サイズ

	// シールドのエフェクトの拡大率
	const Vector3 effect_scale = { 80.0f,  80.0f, 80.0f, };	

	// エネルギーゲージUIの画像のファイルパス
	const std::string energy_gage_ui_file_path = "Data/Image/EnerugyBar.png";	

	// エネルギーゲージUIの背景画像のファイルパス
	const std::string energy_gage_ui_back_file_path = "Data/Image/StatusBase.png";

	// エネルギーゲージUIの位置
	const Vector2 energy_gage_ui_pos = { 215, 660 };

	// エネルギーゲージUIのサイズ
	const Vector2 energy_gage_ui_size = { 300, 13 };

	// エネルギー文字のファイルパス
	const std::string energy_string_file_path = "Data/Image/stamina.png";

	// エネルギー文字の位置
	const Vector2 energy_string_pos = { 70, 620 };

	// エネルギー文字の拡大率
	const Vector2 energy_string_scale = { 0.8f, 0.8f };

	// 最大エネルギーゲージ量
	constexpr int max_energy_gage = 500;

	// プレイヤーからのシールドの距離
	constexpr float player_distance = 100.0f;

	// エネルギーの回復速度
	constexpr int energy_recovery_speed = 3;

	// エネルギーの減る速度
	constexpr int energy_decrease_speed = 1;

	// シールドの拡縮の速度
	constexpr float scale_speed = 0.1f;

	// シールドの拡縮の大きさ
	constexpr float scale_size = 3.0f;
}

// コンストラクタ
Shield::Shield(Player& player) :
	m_player(player),
	m_isInput(false),
	m_effectHandle(-1),
	m_enerugyGage(max_energy_gage),
	m_scale({ shield_width, shield_height }),
	m_sinFrame(0),
	m_alpha(255)
{
	// 3D画像のインスタンスの作成
	m_pImage = std::make_shared<Image3D>(img_file_path);

	// エネルギーゲージUIのインスタンスの作成
	m_pEnergyGage = std::make_shared<Gauge>(
		energy_gage_ui_file_path,
		energy_gage_ui_back_file_path,
		"",
		max_energy_gage,
		energy_gage_ui_pos,
		energy_gage_ui_size,
		120);

	// エネルギーゲージUIの文字の作成
	auto pEnergyString = std::make_shared<ImageUI>(energy_string_file_path);
	pEnergyString->SetPos(energy_string_pos);
	pEnergyString->SetScale(energy_string_scale);

	// UIの登録
	UIManager::GetInstance().AddUI("EnergyGage", m_pEnergyGage, 0, { 0, 1 });
	UIManager::GetInstance().AddUI("EnergyString", pEnergyString, 0, { 0, 1 });

	// シールド画像の初期化
	m_pImage->SetPos(m_player.GetPos());		 // 位置	
	m_pImage->SetImgWidth(m_scale.x);		 // 横幅
	m_pImage->SetImgHeight(m_scale.y);		 // 縦幅
}

// デストラクタ
Shield::~Shield()
{
}

// 更新
void Shield::Update()
{
	// HPが増えていく演出中ならなにもしない
	if (!m_pEnergyGage->IsEndEntranceAnim()) return;

	// プレイヤーが生きていないならなにもしない
	if (!m_player.IsEnabled()) return;

	// 初期化
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.DeleteEffect(m_effectHandle);

	m_isInput = false;
	const Range<int> enerugyGageRange(0, max_energy_gage);

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
	Vector3 tempVec = { (right + -left) * 10.0f, (up + -down) * 10.0f, player_distance };

	// プレイヤーの平行移動行列の取得
	Matrix playerMtx = Matrix::GetTranslate(m_player.GetPos());

	// シールドの相対位置とプレイヤーの平行行列から位置情報を作成
	m_pos = Vector3::Transform(tempVec, playerMtx);

	// ベクトルから角度を求める
	m_rot = -Matrix::ToEulerAngle(Matrix::GetRotationMatrix({ 0, 0, 1 }, tempVec));

	// 入力されていたら
	if (m_isInput)
	{
		// エネルギーゲージが残っていたら
		if (m_enerugyGage > 0)
		{
			// シールドを出している間は常にエネルギーゲージを減らす
			m_enerugyGage -= energy_decrease_speed;

			// エネルギーゲージの残量が3割を切ったら
			if (m_enerugyGage < max_energy_gage * 0.35f)
			{
				// シールドを点滅
				m_alpha = (0.5f * sinf(m_sinFrame * 0.5f) + 0.5f) * 255.0f;
			}
			else
			{
				m_alpha = 255;
			}
		}
	}
	else
	{
		// 入力されていないならエネルギーゲージを回復させる
		m_enerugyGage += energy_recovery_speed;
	}

	// エネルギーゲージの範囲を制限
	m_enerugyGage = enerugyGageRange.Clamp(m_enerugyGage);

	// シールドの拡縮
	m_sinFrame++;
	m_scale.x = shield_width + (sinf(m_sinFrame * scale_speed) * scale_size);
	m_scale.y = shield_height + (sinf(m_sinFrame * scale_speed) * scale_size);

	// 画像の設定
	m_pImage->SetAlpha(m_alpha);			 // 透明度
	m_pImage->SetImgWidth(m_scale.x);		 // 横幅
	m_pImage->SetImgHeight(m_scale.y);		 // 縦幅
	m_pImage->SetPos(m_pos); // 位置
	m_pImage->SetRot(m_rot); // 回転
	m_pImage->Update();		 // 更新

	// エネルギーゲージの設定
	m_pEnergyGage->SetValue(m_enerugyGage);
}

// 描画
void Shield::Draw()
{
	// プレイヤーが生きていたら
	// シールドを出していたら
	if (IsShield() && m_player.IsEnabled())
	{
		m_pImage->Draw();
	}
}

// 位置の取得
const Vector3& Shield::GetPos() const
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