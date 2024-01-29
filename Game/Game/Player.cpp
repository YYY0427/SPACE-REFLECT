#include "Player.h"
#include "Shield.h"
#include "../Util/InputState.h"
#include "../Model.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Util/Range.h"
#include "../Application.h"
#include "../Math/Matrix.h"
#include "../Math/MathUtil.h"
#include "../Math/Vector2.h"
#include "../UI/Gauge.h"
#include "../UI/StatusBack.h"
#include "../UI/ImageUI.h"
#include "../UI/UIManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../MyDebug/DebugText.h"
#include <string>
#include <algorithm>

namespace
{
	// プレイヤーモデルのファイルのパス
	const std::string model_file_path = "Data/Model/Player.mv1";

	// プレイヤーの移動量
	const Vector3 player_vec_up = { 0, 1, 0 };
	const Vector3 player_vec_down = { 0, -1, 0 };
	const Vector3 player_vec_right = { -1, 0, 0 };
	const Vector3 player_vec_left = { 1, 0, 0 };

	// プレイヤーの通常移動速度
	constexpr float move_normal_speed = 1.5f;

	// プレイヤーのZ軸移動速度
	constexpr float move_z_speed = 1.5f;

	// プレイヤーの当たり判定の半径
	constexpr float model_collision_radius = 50.0f;

	// 無敵時間のフレーム数
	constexpr int ultimate_frames = 120;

	// 最大HP
	constexpr int max_hp = 300;

	// 何フレーム前まで位置情報を保存するか
	constexpr int log_frame = 10;

	// プレイヤーの横幅
	constexpr float player_width = 50.0f;

	// プレイヤーの縦幅
	constexpr float player_height = 50.0f;

	// プレイヤーのHPバーのファイルパス
	const std::string hp_bar_file_path = "Data/Image/HPBar.png";

	// プレイヤーのHPバーの背景画像のファイルパス
	const std::string hp_bar_back_file_path = "Data/Image/StatusBase.png";

	// プレイヤーのHP文字のファイルパス
	const std::string hp_string_file_path = "Data/Image/Life.png";

	// プレイヤーのHP文字の位置
	const Vector2 hp_string_pos = { 70, 560 };

	// プレイヤーのHP文字の拡大率
	const Vector2 hp_string_scale = { 0.8f, 0.8f };

	// プレイヤーのHPバーの位置
	const Vector2 hp_bar_pos = { 215, 600 };

	// プレイヤーのHPバーのサイズ
	const Vector2 hp_bar_size = { 300, 13 };

	// ゲームのスタート演出の移動速度
	constexpr float start_move_speed = 25.0f;

	// ブーストエフェクトの拡大率
	const Vector3 boost_effect_scale = { 30.0f, 30.0f, 30.0f };

	// ブーストエフェクトのスピード
	constexpr float boost_effect_speed = 1.0f;
}

//  コンストラクタ
Player::Player() :
	m_moveVec(0, 0, 0),
	m_hp(max_hp),
	m_ultimateTimer(0),
	m_isInputLeftStick(false),
	m_moveSpeed(move_normal_speed),
	m_playerDeadEffectHandle(-1),
	m_isPlayerDeadEffect(false),
	m_isStartAnimation(false),
	m_slowValue(1.0f),
	m_windEffectHandle(-1),
	m_opacity(1.0f)
{
	// データの読み込み
	auto& data = DataReaderFromUnity::GetInstance().GetData("Player");
	m_pos = data.front().pos;
	m_rot = data.front().rot;
	m_scale = data.front().scale;

	// プレイヤーモデルのインスタンスの生成
	m_pModel = std::make_shared<Model>(model_file_path.c_str());

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);// 不透明度
	m_pModel->SetScale(m_scale);	// 拡大率
	m_pModel->SetRot(m_rot);		// 回転
	m_pModel->SetPos(m_pos);		// 位置
	m_pModel->Update();				// 更新

	// ブーストエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		m_boostEffectHandle, 
		EffectID::player_boost, 
		{ m_pos.x, m_pos.y + 30.0f, m_pos.z },
		boost_effect_scale,
		boost_effect_speed,
		{ m_rot.x, 0.0f, 0.0f });

	//// 風エフェクトの再生
	//Effekseer3DEffectManager::GetInstance().PlayEffectLoopAndFollow(
	//	m_windEffectHandle,
	//	EffectID::wind,
	//	&m_pos,
	//	{ 100.0f, 100.0f, 100.0f },
	//	1.0f,
	//	{ 0.0f, -DX_PI_F / 2, 0.0f });
}

//  デストラクタ
Player::~Player()
{
}

// スタート演出の更新
void Player::UpdateStart(Vector3 cameraPos)
{
	// Z軸方向に移動
	m_moveVec.z = (start_move_speed * m_slowValue);
	m_pos.z += m_moveVec.z;

	if (m_pos.z > cameraPos.z + 200)
	{
		// スタート演出をしたかフラグを立てる
		m_isStartAnimation = true;
	}

	// プレイヤーを傾ける
	m_rot.x = -MathUtil::ToRadian(25.0f);

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectPos(m_boostEffectHandle, { m_pos.x, m_pos.y, m_pos.z - 30.0f });
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, boost_effect_scale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, boost_effect_speed * m_slowValue);

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 更新
void Player::Update(float cameraHorizon)
{
	if (!m_pBackUI)
	{
		// プレイヤーUIの背景画像のインスタンスの生成
		m_pBackUI = std::make_shared<StatusBack>();

		// UIの登録
		UIManager::GetInstance().AddUI("PlayerStatusBack", m_pBackUI, 0, { 2, 1 });
	}

	// スタート演出をしていない場合
	if (!m_pBackUI->GetIsStartAnimation())
	{
		// スタート演出
		m_pBackUI->UpdateStartAnimation();
	}
	// スタート演出が終わったら一回だけ
	else if(!m_pShield && !m_pHPbar)
	{
		// シールドのインスタンスの生成
		m_pShield = std::make_shared<Shield>(*this);

		// HPバーのインスタンスの生成
		m_pHPbar = std::make_shared<Gauge>(
			hp_bar_file_path,
			hp_bar_back_file_path,
			"",
			max_hp,
			hp_bar_pos,
			hp_bar_size,
			true,
			3.0f,
			false,
			0.0f);

		// HP文字のインスタンスの生成
		auto pHpString = std::make_shared<ImageUI>(hp_string_file_path);
		pHpString->SetPos(hp_string_pos);
		pHpString->SetScale(hp_string_scale);

		// UIの登録
		UIManager::GetInstance().AddUI("HPBar", m_pHPbar, 0, { 0, 1 });
		UIManager::GetInstance().AddUI("HPString", pHpString, 0, { 0, 1 });
	}

	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// 左スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	// カメラの回転に合わせて移動ベクトルを作成
	Vector3 moveUp = Vector3::Transform(player_vec_up, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveDown = Vector3::Transform(player_vec_down, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveRight = Vector3::Transform(player_vec_right, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveLeft = Vector3::Transform(player_vec_left, Matrix::GetRotationY(cameraHorizon));

	// 移動情報の初期化
	m_isInputLeftStick = false;
	m_moveVec = { 0, 0, 0 };
	Vector3 moveVecX = { 0, 0, 0 };
	Vector3 moveVecY{ 0, 0, 0 };

	// スティックが入力されていたら移動ベクトルにスティックが傾いている方向のベクトルを代入
	// スティックの傾きぐわいによってベクトルが大きくなる
	if (up > 0)
	{
		moveVecY = moveUp * up;
		m_isInputLeftStick = true;
	}
	if (left > 0)
	{
		moveVecX = moveLeft * left;
		m_isInputLeftStick = true;
	}
	if (down > 0)
	{
		moveVecY = moveDown * down;
		m_isInputLeftStick = true;
	}
	if (right > 0)
	{
		moveVecX = moveRight * right;
		m_isInputLeftStick = true;
	}

	// スティックが入力されている場合のみ移動
	if (m_isInputLeftStick)
	{
		// プレイヤーから見てx方向とz方向のベクトルを足して移動ベクトルを作成する
		m_moveVec = moveVecY + moveVecX;

		// プレイヤーのスピードを掛ける
		m_moveVec *= (m_moveSpeed * m_slowValue);

		// 作成した移動ベクトルで座標の移動
		Vector3 tempPos = m_pos + m_moveVec;

		// ワールド座標をスクリーン座標に変換
		Vector3 screenPos = Vector3::FromDxLibVector3(
			ConvWorldPosToScreenPos(tempPos.ToDxLibVector3()));

		// 画面外に出ないようにする
		Size size = Application::GetInstance().GetWindowSize();
		if (screenPos.x > size.width - player_width)
		{
			screenPos.x = size.width - player_width;

			Vector3 worldPos = Vector3::FromDxLibVector3(
				ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
			m_pos.x = worldPos.x;
			m_pos.y = worldPos.y;
		}
		else if (screenPos.x < 0 + player_width)
		{
			screenPos.x = 0 + player_width;

			Vector3 worldPos = Vector3::FromDxLibVector3(
				ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
			m_pos.x = worldPos.x;
			m_pos.y = worldPos.y;
		}
		else if (screenPos.y > size.height - player_height)
		{
			screenPos.y = size.height - player_height;

			Vector3 worldPos = Vector3::FromDxLibVector3(
				ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
			m_pos.x = worldPos.x;
			m_pos.y = worldPos.y;
		}
		else if (screenPos.y < 0 + player_height)
		{
			screenPos.y = 0 + player_height;

			Vector3 worldPos = Vector3::FromDxLibVector3(
				ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
			m_pos.x = worldPos.x;
			m_pos.y = worldPos.y;
		}
		else
		{
			m_pos = tempPos;
		}
	}

	// 常にZ軸方向に移動
	m_moveVec.z = (move_z_speed * m_slowValue);
	m_pos.z += m_moveVec.z;

	// ログに追加
	m_posLogTable.push_front(m_pos);

	// 保存可能な数を超えていたら末尾から消す
	if (m_posLogTable.size() > log_frame)
	{
		m_posLogTable.pop_back();
	}

	// 無敵時間のタイマーの更新
	// 0以下にはならない
	m_ultimateTimer = (std::max)(--m_ultimateTimer, 0);

	// 移動ベクトルの大きさからプレイヤーの傾き具合を算出
	// X軸回転は進んでいるように見せるよう傾ける
	float rotX = MathUtil::ToRadian(25.0f);
	m_rot = { rotX + m_moveVec.z * 0.01f, 0.0f, -m_moveVec.x * 0.01f };

	// 不透明度を元に戻す
	m_opacity = 1.0f;

	// エフェクトの設定
	effectManager.SetEffectPos(m_boostEffectHandle, { m_pos.x, m_pos.y, m_pos.z - 30.0f });
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, boost_effect_scale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, boost_effect_speed * m_slowValue);

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->Update();					// 更新

	// シールドがある場合
	if (m_pShield)
	{
		// シールドの更新
		m_pShield->Update();
	}
}

void Player::GameOverUpdate()
{
	// UIを格納
	UIManager::GetInstance().Store();

	// タイマーの更新
	m_waitTimer.Update(1);

	// タイマーが制限時間を超えていてエフェクトを再生していない場合
	// エフェクトを再生する
	if (m_waitTimer.IsTimeOut() && !m_isPlayerDeadEffect)
	{
		// 再生フラグを立てる
		m_isPlayerDeadEffect = true;

		// 再生しているエフェクトを削除
		auto& effectManager = Effekseer3DEffectManager::GetInstance();
		effectManager.DeleteEffect(m_boostEffectHandle);

		// プレイヤー死亡エフェクトの再生
		effectManager.PlayEffectFollow(
			m_playerDeadEffectHandle, 
			EffectID::player_dead, 
			&m_pos, 
			{ 50.0f, 50.0f, 50.0f }, 0.5f);
	}
	// タイマーが制限時間以内の場合は移動
	else if (!m_waitTimer.IsTimeOut())
	{
		// 移動ベクトル作成
		m_moveVec = { 0.0f, -0.1f, 1.0f };
		m_moveVec = m_moveVec.Normalized();
		m_moveVec *= 2.0f;

		// 作成した移動ベクトルで座標の移動
		m_pos = m_pos + m_moveVec;
	}

	// 位置座標の設定
	m_pModel->SetPos(m_pos);

	// 向いている方向の設定
	m_pModel->SetRot({ m_moveVec.Length(), 0.0f, m_moveVec.Length() });

	// アニメーションと当たり判定の更新
	m_pModel->Update();
}

// 描画
void Player::Draw()
{
	// 死んでいない場合
	if (!m_isPlayerDeadEffect)
	{
		// プレイヤーモデルの描画
		m_pModel->Draw();

#ifdef _DEBUG
		// プレイヤーの当たり判定の描画
		DrawSphere3D(m_pos.ToDxLibVector3(), model_collision_radius, 8, 0xff0000, 0xff0000, false);

		// プレイヤーの位置情報の描画
		DebugText::Log("プレイヤー 座標", { m_pos.x, m_pos.y, m_pos.z});
#endif 
	}
}

// シールドの描画
void Player::DrawShield()
{
	// シールドがある場合
	// プレイヤーが死んでいる場合は描画しない
	if (m_pShield && !m_isPlayerDeadEffect)
	{
		// シールドの描画
		m_pShield->Draw();
	}
}

// ダメージ処理
void Player::OnDamage(int damage)
{
	// HPを減らす
	// 0以下にはならない
	m_hp = (std::max)(m_hp - damage, 0);

	// HPバーの値の設定
	m_pHPbar->SetValue(m_hp);
}

// シールドが反射した時の処理
void Player::OnReflect()
{
	//// レーザーが見えやすいように不透明度を下げる
	m_opacity = 0.1f;
	m_pModel->SetOpacity(m_opacity);	
}

// プレイヤーが生きているか
bool Player::IsLive() const
{
	return (m_hp > 0) ? true : false;
}

// 位置情報の取得
Vector3 Player::GetPos() const
{
	return m_pos;
}

// 移動ベクトルの取得
Vector3 Player::GetMoveVec() const
{
	return m_moveVec;
}

// プレイヤーの当たり判定の半径の取得
float Player::GetCollsionRadius() const
{
	return model_collision_radius;
}

// プレイヤーモデルのハンドルの取得
int Player::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

 // スタート演出をしたかフラグの取得
bool Player::GetIsStartAnimation() const
{
	return m_isStartAnimation;
}

// スローの値の設定
void Player::SetSlowValue(float slowValue)
{
	m_slowValue = slowValue;
}

// シールドのインスタンスの取得
std::shared_ptr<Shield> Player::GetShield() const
{
	return m_pShield;
}

// 決められたフレームの数だけ位置情報を保存するテーブルの取得
std::deque<Vector3> Player::GetPosLogTable() const
{
	return m_posLogTable;
}