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
#include <algorithm>

namespace
{
	// プレイヤーのパラメータのファイルパス
	const std::string player_param_file_path = "Data/Csv/PlayerParam.csv";

	// プレイヤーの移動量
	const Vector3 player_vec_up = { 0, 1, 0 };
	const Vector3 player_vec_down = { 0, -1, 0 };
	const Vector3 player_vec_right = { -1, 0, 0 };
	const Vector3 player_vec_left = { 1, 0, 0 };

	// プレイヤーの初期の向いている方向
	const Vector3 init_model_direction = { 0, 0, 1 };

	// 何フレーム前まで位置情報を保存するか
	constexpr int log_frame = 10;

	// プレイヤーのHP文字の位置
	const Vector2 hp_string_pos = { 70, 560 };

	// プレイヤーのHP文字の拡大率
	const Vector2 hp_string_scale = { 0.8f, 0.8f };

	// プレイヤーのHPバーの位置
	const Vector2 hp_bar_pos = { 215, 600 };

	// プレイヤーのHPバーのサイズ
	const Vector2 hp_bar_size = { 300, 13 };

	// プレイヤーのHPバーのファイルパス
	const std::string hp_bar_file_path = "Data/Image/HPBar.png";

	// プレイヤーのHPバーの背景画像のファイルパス
	const std::string hp_bar_back_file_path = "Data/Image/StatusBase.png";

	// プレイヤーのHP文字のファイルパス
	const std::string hp_string_file_path = "Data/Image/Life.png";

	// ゲームオーバー演出のフレーム数
	const int game_over_frame = 60 * 5;
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
	m_dieEffectIntervalTimer(20),
	m_waitFrame(game_over_frame)
{
	// データの読み込み
	auto data = DataReaderFromUnity::GetInstance().GetData(objectDataFileName, "Player");
	m_pos   = data.front().pos;
	m_rot   = data.front().rot;
	m_scale = data.front().scale;

	// 外部ファイルからパラメータを読み込み
	LoadParameter(player_param_file_path);
	m_maxHp = m_parameterTable["maxHp"];
	m_hp    = m_maxHp;
	m_boostEffectScale.x = GetParameter("boostEffectScaleX");
	m_boostEffectScale.y = GetParameter("boostEffectScaleY");
	m_boostEffectScale.z = GetParameter("boostEffectScaleZ");
	m_boostEffectSpeed   = GetParameter("boostEffectSpeed");
	m_playerSize.x		 = GetParameter("playerWidth");	
	m_playerSize.y       = GetParameter("playerHeight");
	m_collisionRadius	 = GetParameter("collisionRadius");
	m_moveSpeedXY		 = GetParameter("moveSpeedXY");
	m_moveSpeedZ		 = GetParameter("moveSpeedZ");

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
		{ m_pos.x, m_pos.y - 30.0f, m_pos.z - 30.0f },
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
	m_moveVec.z = GetParameter("startMoveSpeed");
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
	effectManager.SetEffectPos(m_boostEffectHandle, { m_pos.x, m_pos.y - 30.0f, m_pos.z - 30.0f });
	effectManager.SetEffectRot(m_boostEffectHandle, { m_rot.x + DX_PI_F, m_rot.y, -m_rot.z });
	effectManager.SetEffectScale(m_boostEffectHandle, m_boostEffectScale);
	effectManager.SetEffectSpeed(m_boostEffectHandle, m_boostEffectSpeed);

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
		auto& soundManager = SoundManager::GetInstance();
		soundManager.PlaySE("GageRecovery");

		// プレイヤーUIの背景画像のインスタンスの生成
		m_pBackUI = std::make_shared<StatusBack>();

		// UIの登録
		UIManager::GetInstance().AddUI("PlayerStatusBack", m_pBackUI, 0, { -2, 0 });
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
	Vector3 moveUp = Vector3::Transform(player_vec_up, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveDown = Vector3::Transform(player_vec_down, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveRight = Vector3::Transform(player_vec_right, Matrix::GetRotationY(cameraHorizon));
	Vector3 moveLeft = Vector3::Transform(player_vec_left, Matrix::GetRotationY(cameraHorizon));

	// 移動情報の初期化
	m_isInputLeftStick = false;
	m_moveVec.z = 0;
	m_moveVec *= 0.98f;
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
		m_moveVec *= m_moveSpeedXY;

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
	}

	// 常にZ軸方向に移動
	m_moveVec.z = m_moveSpeedZ;
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
	float rotX = MathUtil::ToRadian(15.0f);
	m_rot = { rotX + (m_moveVec.z * 0.01f) + (-m_moveVec.y * 0.05f), 0.0f, -m_moveVec.x * 0.05f };

	// 不透明度を元に戻す
	m_opacity = 1.0f;

	// エフェクトの設定
	effectManager.SetEffectPos(m_boostEffectHandle, { m_pos.x, m_pos.y - 30.0f, m_pos.z - 30.0f });
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
	m_moveVec.z = m_moveSpeedZ;
	m_pos.z += m_moveVec.z;

	// モデルの設定
	m_pModel->SetOpacity(m_opacity);	// 不透明度
	m_pModel->SetPos(m_pos);			// 位置
	m_pModel->SetRot(m_rot);			// 回転
	m_pModel->Update();					// 更新

	// エフェクトの設定
	auto& effectManager = Effekseer3DEffectManager::GetInstance();
	effectManager.SetEffectPos(m_boostEffectHandle, { m_pos.x, m_pos.y - 30.0f, m_pos.z - 30.0f });
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
	m_moveVec = { 0.0f, -0.5f, 1.0f };
	m_moveVec = m_moveVec.Normalized();
	m_moveVec *= 2.0f;

	// プレイヤーを回転
	m_rot += { 0, 0, MathUtil::ToRadian(5) };

	// 作成した移動ベクトルで座標の移動
	m_pos = m_pos + m_moveVec;


	if (m_waitFrame <= game_over_frame * 0.9)
	{
		static bool isPlay = false;
		if (!isPlay)
		{
			// プレイヤーが死んだ音の再生
			SoundManager::GetInstance().PlaySE("GameOver");
			isPlay = true;
		}
	}

	if (m_waitFrame-- >= 0)
	{
		// タイマーの更新
		m_dieEffectIntervalTimer.Update(1);

		// タイマーの制限時間を超えたら 
		if (m_dieEffectIntervalTimer.IsTimeOut())
		{
			// エフェクトの再生位置をプレイヤーの周りにランダムで設定
			Vector3 pos =
			{
				m_pos.x + (GetRand(150) - 50),
				m_pos.y + (GetRand(150) - 50),
				m_pos.z + (GetRand(150) - 50)
			};

			// エフェクトの拡大率をランダムで設定
			float scale = GetRand(5) + 3;

			// エフェクトを再生
			Effekseer3DEffectManager::GetInstance().PlayEffect(
				m_playerDeadEffectHandle,
				"PlayerDied",
				pos,
				{ scale, scale, scale },
				1.0f);

			// タイマーのリセット
			m_dieEffectIntervalTimer.Reset();
		}
	}
	else
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
		DebugText::Log("PlayerPos", { m_pos.x, m_pos.y, m_pos.z});
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
void Player::OnDamage(int damage)
{
	// HPを減らす
	// 0以下にはならない
	m_hp = (std::max)(m_hp - damage, 0);

	// HPバーの値の設定
	m_pHPbar->SetValue(m_hp);

	static unsigned int count = 0;
	count++;
	if (count % 3 == 0)
	{
		// ダメージ音の再生
		auto& soundManager = SoundManager::GetInstance();
	//	if (!soundManager.IsPlaySound("PlayerDamage"))
		{
			SoundManager::GetInstance().PlaySE("PlayerDamage");
			count = 0;
		}
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

// プレイヤーが生きているか
bool Player::IsLive() const
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

// パラメータの取得
float Player::GetParameter(const std::string& key) const
{
	if (m_parameterTable.find(key) != m_parameterTable.end())
	{
		return m_parameterTable.at(key);
	}
	// キーが存在しない場合
	// エラーメッセージを出力
	assert(!"Playerクラスのパラメータにkeyが存在しません");
	return -1;
}