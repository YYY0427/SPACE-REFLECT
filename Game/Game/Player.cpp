#include "Player.h"
#include "Shield.h"
#include "../Util/InputState.h"
#include "../Util/FileUtil.h"
#include "../Util/Range.h"
#include "../Application.h"
#include "../Math/Matrix.h"
#include "../Math/MathUtil.h"
#include "../Math/Vector2.h"
#include "../UI/Gauge.h"
#include "../UI/StatusBack.h"
#include "../UI/ImageUI.h"
#include "../UI/UIManager.h"
#include "../Model.h"
#include "../ModelHandleManager.h"
#include "../Effect/Effekseer3DEffectManager.h"
#include "../Editor/DataReaderFromUnity.h"
#include "../MyDebug/DebugText.h"
#include "../SoundManager.h"
#include "Camera.h"
#include <algorithm>

namespace
{
	// プレイヤーのパラメータのファイルパス
	const std::string player_param_file_path = "Data/Csv/PlayerParam.csv";

	// プレイヤーの移動量
	const Vector3 player_vec_up    = { 0, 1, 0 };
	const Vector3 player_vec_down  = { 0, -1, 0 };
	const Vector3 player_vec_right = { -1, 0, 0 };
	const Vector3 player_vec_left  = { 1, 0, 0 };

	// プレイヤーの初期の向いている方向
	const Vector3 init_model_direction = { 0, 0, 1 };

	// 何フレーム前まで位置情報を保存するか
	constexpr int log_frame = 10;

	// プレイヤーのHP文字
	const Vector2 hp_string_pos   = { 70, 560 };	// 位置
	const Vector2 hp_string_scale = { 0.8f, 0.8f };	// 拡大率

	// プレイヤーのHPバー
	const Vector2 hp_bar_pos  = { 215, 600 };		// 位置
	const Vector2 hp_bar_size = { 300, 13 };		// サイズ

	// プレイヤーのHPバーのファイルパス
	const std::string hp_bar_file_path = "Data/Image/HPBar.png";

	// プレイヤーのHPバーの背景画像のファイルパス
	const std::string hp_bar_back_file_path = "Data/Image/StatusBase.png";

	// プレイヤーのHP文字のファイルパス
	const std::string hp_string_file_path = "Data/Image/Life.png";

	// ゲームオーバー演出のフレーム数
	const int game_over_frame = 60 * 5;

	// ブーストエフェクトのプレイヤーとの相対位置
	const Vector3 boost_effect_relative_pos = { 0, -30.0f, -30.0f };

	// プレイヤーを進んでいるように見せるために傾ける角度
	constexpr float tilt_angle = 25.0f;

	// カメラがプレイヤーを追い越しているかどうかの範囲
	constexpr float camera_over_range = 200.0f;

	// プレイヤーの移動ベクトルの減少率
	constexpr float move_vec_decrease_rate = 0.9f;

	// 透明度の最大値
	constexpr float max_opacity = 1.0f;

	// プレイヤーを傾ける大きさ
	constexpr float tilt_size = 0.02f;

	// ゲームオーバー時のプレイヤーを1フレームごとに回転させる角度
	constexpr float game_over_rotate_angle = 5.0f;

	// ゲームオーバー時のプレイヤーの移動ベクトル
	const Vector3 game_over_move_vec = { 0.0f, -0.5f, 1.0f };
}

//  コンストラクタ
Player::Player(const std::string& objectDataFileName) :
	m_moveVec(0, 0, 0),
	m_ultimateTimer(0),
	m_isInputLeftStick(false),
	m_playerDeadEffectHandle(-1),
	m_isPlayerDeadEffect(false),
	m_isStartAnimation(false),
	m_isReflect(false),
	m_boostEffectHandle(-1),
	m_damageEffectHandle(-1),
	m_opacity(1.0f),
	m_gameOverWaitFrame(game_over_frame),
	m_isGameOverSE(false)
{
	// データの読み込み
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Player");
	m_pos     = data.front().pos;
	m_rot     = data.front().rot;
	m_scale   = data.front().scale;

	// 外部ファイルからパラメータを読み込み
	LoadParameter(player_param_file_path);
	m_maxHp = GetParameter(DataType::PlayerParamType::MAX_HP);
	m_hp    = m_maxHp;
	m_boostEffectScale.x = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_X);
	m_boostEffectScale.y = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_Y);
	m_boostEffectScale.z = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SCALE_Z);
	m_boostEffectSpeed   = GetParameter(DataType::PlayerParamType::BOOST_EFFECT_SPEED);
	m_playerSize.x		 = GetParameter(DataType::PlayerParamType::PlAYER_WIDTH);	
	m_playerSize.y       = GetParameter(DataType::PlayerParamType::PlAYER_HEIGHT);
	m_collisionRadius	 = GetParameter(DataType::PlayerParamType::COLLISION_RADIUS);
	m_moveSpeed.x		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_XY);
	m_moveSpeed.y		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_XY);
	m_moveSpeed.z		 = GetParameter(DataType::PlayerParamType::MOVE_SPEED_Z);

	// プレイヤーモデルのインスタンスの生成
	m_pModel = std::make_shared<Model>(ModelHandleManager::GetInstance().GetHandle("Player"));

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);// 不透明度
	m_pModel->SetScale(m_scale);	// 拡大率
	m_pModel->SetRot(m_rot);		// 回転
	m_pModel->SetPos(m_pos);		// 位置
	m_pModel->Update();				// 更新

	// ブーストエフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffectLoop(
		m_boostEffectHandle, 
		"PlayerBoost",
		m_pos + boost_effect_relative_pos,
		m_boostEffectScale,
		m_boostEffectSpeed,
		{ m_rot.x, 0.0f, 0.0f });
}

//  デストラクタ
Player::~Player()
{
}

// スタート演出の更新
void Player::UpdateStart(const Vector3& cameraPos)
{
	// Z軸方向に移動
	m_moveVec.z = GetParameter(DataType::PlayerParamType::START_MOVE_SPEED_Z);
	m_pos.z += m_moveVec.z;

	if (m_pos.z > cameraPos.z + camera_over_range)
	{
		// スタート演出をしたかフラグを立てる
		m_isStartAnimation = true;
	}

	// プレイヤーを傾ける
	m_rot.x = -MathUtil::ToRadian(tilt_angle);

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);

	// モデルの設定
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新
}

// 更新
void Player::UpdatePlay(const float cameraHorizon)
{
	if (!m_pBackUI)
	{
		auto& soundManager = SoundManager::GetInstance();
		soundManager.PlaySE("GageRecovery");

		// プレイヤーUIの背景画像のインスタンスの生成
		m_pBackUI = std::make_shared<StatusBack>();

		// UIの登録
		UIManager::GetInstance().AddUI("PlayerStatusBack", m_pBackUI, 0, { -2, 0 });
	}

	// スタート演出をしていない場合
	if (!m_pBackUI->IsStartAnimation())
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
			m_maxHp,
			hp_bar_pos,
			hp_bar_size,
			120);

		// HP文字のインスタンスの生成
		auto pHpString = std::make_shared<ImageUI>(hp_string_file_path);
		pHpString->SetPos(hp_string_pos);
		pHpString->SetScale(hp_string_scale);

		// UIの登録
		UIManager::GetInstance().AddUI("HPBar", m_pHPbar, 0, { -2, 0 });
		UIManager::GetInstance().AddUI("HPString", pHpString, 0, { -2, 0 });
	}

	auto& effectManager = Effekseer3DEffectManager::GetInstance();

	// 左スティックの入力情報の取得
	int up = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::UP);
	int down = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::DOWN);
	int left = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::LEFT);
	int right = InputState::IsPadStick(PadLR::LEFT, PadStickInputType::RIGHT);

	// カメラの回転に合わせて移動ベクトルを作成
	Vector3 moveUp    = Vector3::Transform(player_vec_up, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveDown  = Vector3::Transform(player_vec_down, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveRight = Vector3::Transform(player_vec_right, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveLeft  = Vector3::Transform(player_vec_left, Matrix::GetRotationY(cameraHorizon));

	// 移動情報の初期化
	m_isInputLeftStick = false;
	m_moveVec.z = 0;
	Vector3 moveVecX = { 0, 0, 0 };
	Vector3 moveVecY = { 0, 0, 0 };

	// 移動ベクトルの減少
	m_moveVec *= move_vec_decrease_rate;

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

		Vector3 moveSpeed;
		moveSpeed = m_moveSpeed;

		// カメラの移動範囲を超えている場合
		// 移動スピードを減らす
		// (カメラが動かないため、カメラが動いているときと違う挙動に見えるため)
		if (m_pCamera->IsOverMoveRangeX())
		{
			moveSpeed.x = m_moveSpeed.x * (1.0f - m_pCamera->GetCameraMoveRate());
		}
		if (m_pCamera->IsOverMoveRangeY())
		{
			moveSpeed.y = m_moveSpeed.y * (1.0f - m_pCamera->GetCameraMoveRate());
		}

		// プレイヤーのスピードを掛ける
		m_moveVec = m_moveVec * moveSpeed;

		DebugText::AddLog("PlayerMoveSpeed", { moveSpeed.x, moveSpeed.y, moveSpeed.z });
	}

	// 作成した移動ベクトルで座標の移動
	Vector3 tempPos = m_pos + m_moveVec;

	// ワールド座標をスクリーン座標に変換
	Vector3 screenPos = Vector3::FromDxLibVector3(
		ConvWorldPosToScreenPos(tempPos.ToDxLibVector3()));

	// 画面外に出ないようにする
	Size size = Application::GetInstance().GetWindowSize();
	if (screenPos.x > size.width - m_playerSize.x)
	{
		screenPos.x = size.width - m_playerSize.x;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.x < 0 + m_playerSize.x)
	{
		screenPos.x = 0 + m_playerSize.x;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.y > size.height - m_playerSize.y)
	{
		screenPos.y = size.height - m_playerSize.y;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else if (screenPos.y < 0 + m_playerSize.y)
	{
		screenPos.y = 0 + m_playerSize.y;

		Vector3 worldPos = Vector3::FromDxLibVector3(
			ConvScreenPosToWorldPos(screenPos.ToDxLibVector3()));
		m_pos.x = worldPos.x;
		m_pos.y = worldPos.y;
	}
	else
	{
		m_pos = tempPos;
	}

	// 常にZ軸方向に移動
	m_moveVec.z = m_moveSpeed.z;
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

	Vector3 moveVec = m_moveVec;
	Vector3 moveSpeed = m_moveSpeed;
	if (m_pCamera->IsOverMoveRangeX())
	{
		moveSpeed.x = m_moveSpeed.x * (1.0f - m_pCamera->GetCameraMoveRate());
		moveVec.x /= moveSpeed.x;
		moveVec.x *= m_moveSpeed.x;
	}
	if (m_pCamera->IsOverMoveRangeY())
	{
		moveSpeed.y = m_moveSpeed.y * (1.0f - m_pCamera->GetCameraMoveRate());
		moveVec.y /= moveSpeed.y;
		moveVec.y *= m_moveSpeed.y;
	}

	// 移動ベクトルの大きさからプレイヤーの傾き具合を算出
	// X軸回転は進んでいるように見せるよう傾ける
	m_rot = { MathUtil::ToRadian(tilt_angle) + (-moveVec.y * tilt_size), 0.0f, -moveVec.x * tilt_size };

	// 不透明度を元に戻す
//	m_opacity = max_opacity;

	// エフェクトの設定
	effectManager.SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, 0.0f, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);

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

// ゲームクリアの更新
void Player::UpdateGameClear()
{
	// シールドの削除
	m_pShield.reset();

	// 常にZ軸方向に移動
	m_moveVec.z = m_moveSpeed.z;
	m_pos.z += m_moveVec.z;

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->Update();					// 更新

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectPos(m_boostEffectHandle, m_pos + boost_effect_relative_pos);
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);
}

// ゲームオーバーの更新
bool Player::UpdateGameOver()
{
	// エフェクトの停止
	Effekseer3DEffectManager::GetInstance().DeleteEffect(m_boostEffectHandle);

	// 移動ベクトル作成
	m_moveVec = game_over_move_vec;
	m_moveVec = m_moveVec.Normalized();
	m_moveVec *= 2.0f;

	// プレイヤーを回転
	m_rot += { 0, 0, MathUtil::ToRadian(game_over_rotate_angle) };

	// 作成した移動ベクトルで座標の移動
	m_pos = m_pos + m_moveVec;


	if (m_gameOverWaitFrame <= game_over_frame * 0.9)
	{
		if (!m_isGameOverSE)
		{
			// プレイヤーが死んだ音の再生
			SoundManager::GetInstance().PlaySE("GameOver");
			m_isGameOverSE = true;
		}
	}

	if(m_gameOverWaitFrame-- <= 0)
	{
		// まだ再生していない場合
		if (!m_isPlayerDeadEffect)
		{
			// フラグを立てる
			m_isPlayerDeadEffect = true;

			// でかい爆発エフェクトを再生
			Effekseer3DEffectManager::GetInstance().PlayEffect(
				m_playerDeadEffectHandle,
				"PlayerDied",
				m_pos,
				{ 50.0f, 50.0f, 50.0f },
				0.5f);

			// 大きい爆発音の再生
			SoundManager::GetInstance().PlaySE("NormalEnemyDie");
		}
		else
		{
			// でかい爆発エフェクトの再生が終了したら
			if (!Effekseer3DEffectManager::GetInstance().IsPlayingEffect(m_playerDeadEffectHandle))
			{
				return true;
			}
		}
	}

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetPos(m_pos);	// 位置
	m_pModel->SetRot(m_rot);	// 回転
	m_pModel->Update();			// 更新

	return false;
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
		DrawSphere3D(m_pos.ToDxLibVector3(), m_collisionRadius, 8, 0xff0000, 0xff0000, false);

		// プレイヤーの位置情報の描画
		DebugText::AddLog("PlayerPos", { m_pos.x, m_pos.y, m_pos.z});
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
		SetWriteZBuffer3D(false);
		m_pShield->Draw();
		SetWriteZBuffer3D(true);
	}
}

// 外部ファイルからパラメータを読み込む
void Player::LoadParameter(const std::string& fileName)
{
	// ファイルの読み込み
	auto data = FileUtil::LoadCsvFile(fileName);

	// 値の格納
	for (auto& line : data)
	{
		std::string key;
		for (size_t i = 0; i < line.size(); i++)
		{
			// 1列目は説明が書いてあるので飛ばす
			if (i == 0) continue;

			if (i == 1)
			{
				// キーの設定
				key = line[i];
			}
			else
			{
				// パラメータの設定
				m_parameterTable[key] = std::stof(line[i]);
			}
		}
	}
}

// ダメージ処理
void Player::OnDamage(const int damage)
{
	// HPを減らす
	// 0以下にはならない
	m_hp = (std::max)(m_hp - damage, 0);

	// HPバーの値の設定
	m_pHPbar->SetValue(m_hp);

	if (m_onDamageSEWaitFrame++ % 3 == 0)
	{
		// ダメージ音の再生
		auto& soundManager = SoundManager::GetInstance();
		SoundManager::GetInstance().PlaySE("PlayerDamage");

		m_onDamageSEWaitFrame = 0;
	}

	// エフェクトの再生
	Effekseer3DEffectManager::GetInstance().PlayEffect(
		m_damageEffectHandle,
		"EnemyAttackHitEffect",
		{ m_pos.x, m_pos.y, m_pos.z },
		{ 25.0f, 25.0f, 25.0f },
		1.0f);
}

// シールドが反射した時の処理
void Player::OnReflect()
{
	// レーザーが見えやすいように不透明度を下げる
	m_opacity = 0.1f;
	m_pModel->SetOpacity(m_opacity);	
}

// プレイヤーを徐々に透明にする
void Player::FadeOut()
{
	// 不透明度を下げる
	m_opacity -= 0.01f;
	m_opacity = (std::max)(m_opacity, 0.0f);

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->Update();					// 更新
}

// プレイヤーが生きているか
bool Player::IsEnabled() const
{
	return (m_hp > 0) ? true : false;
}

// 位置情報の取得
const Vector3& Player::GetPos() const
{
	return m_pos;
}

// 移動ベクトルの取得
const Vector3& Player::GetMoveVec() const
{
	return m_moveVec;
}

// プレイヤーの当たり判定の半径の取得
float Player::GetCollisionRadius() const
{
	return m_collisionRadius;
}

// プレイヤーモデルのハンドルの取得
int Player::GetModelHandle() const
{
	return m_pModel->GetModelHandle();
}

 // スタート演出をしたかフラグの取得
bool Player::IsStartAnimation() const
{
	return m_isStartAnimation;
}

// シールドのインスタンスの取得
const std::shared_ptr<Shield>& Player::GetShield() const
{
	return m_pShield;
}

// 決められたフレームの数だけ位置情報を保存するテーブルの取得
const std::deque<Vector3>& Player::GetPosLogTable() const
{
	return m_posLogTable;
}

// パラメーターの取得
float Player::GetParameter(const DataType::PlayerParamType type)
{
	// データからキーを取得
	auto& key = DataType::PlayerParamKey[static_cast<int>(type)];

	// キーが存在するか確認
	if (m_parameterTable.find(key) != m_parameterTable.end())
	{
		// 存在する場合は値を返す
		return m_parameterTable.at(key);
	}

	// キーが存在しない場合
	// エラーメッセージを出力
	assert(!"Playerクラスのパラメータにkeyが存在しません");
	return -1;
}

// カメラのポインタの設定
void Player::SetCameraPointer(const std::shared_ptr<Camera>& pCamera)
{
	m_pCamera = pCamera;
}
