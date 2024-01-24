#include "BossMatrix.h"
#include "../../../Model.h"
#include "../../../UI/Gauge.h"
#include "../../../UI/UIManager.h"
#include "../../../Math/MathUtil.h"
#include "../../../Application.h"
#include <random>
#include <algorithm>

namespace
{
	// 位置
	const Vector3 init_pos = { 0.0f, 300.0f, 1100.0f };			// 初期位置
	const Vector3 goal_init_pos = { 0.0f, 300.0f, 800.0f };		// 登場時の位置

	// モデル
	const Vector3 model_rot = { MathUtil::ToRadian(20), DX_PI_F, 0.0f};
	const Vector3 model_scale = { 1.0f , 1.0f, 1.0f };			// 拡大率
	const Vector3 init_model_direction = { 0.0f, 0.0f, -1.0f };	// 初期の向き

	// アニメーション番号
	constexpr int idle_anim_no = 0;						// 待機
	constexpr int normal_laser_fire_anim_no = 1;		// 移動
	constexpr int cube_laser_fire_anim_no = 2;			// レーザー発射

	// モデルのマテリアル番号
	constexpr int body_material_no = 0;			// 本体
	constexpr int lense_material_no = 1;		// レンズ
	constexpr int eye_material_no = 2;			// 目
	constexpr int laser_material_no = 3;		// レーザー

	// 移動
	constexpr float move_speed = 10.0f;			// 移動速度
	constexpr float move_error_range = 10.0f;	// 移動誤差範囲
	const Vector3 move_pos[] =					// 移動先の座標
	{
		goal_init_pos,
		{ 930, 300, 800 },
		{ 930, -300, 800 },
		{ -930, 300, 800 },
		{ -930, -300, 800 },
	};

	// 死亡
	constexpr float died_swing_width = 5.0f;	// 死亡時の横揺れの大きさ
	constexpr float died_swing_speed = 1.0f;	// 死亡時の横揺れの速さ
	constexpr int died_continue_frame = 60 * 5;	// 死亡時の演出の継続時間

	// HP
	auto& screenSize = Application::GetInstance().GetWindowSize();
	constexpr int max_hp = 1000;		// 最大HP
	const Vector2 hp_gauge_pos = { screenSize.width / 2, screenSize.height - 300};	// HPゲージの位置
	const Vector2 hp_gauge_size = { 500, 50 };										// HPゲージのサイズ
	const std::string hp_gauge_img_file_path = "Data/Image/HP.png";				// HPゲージの画像ファイルパス
	const std::string hp_gauge_back_img_file_path = "Data/Image/HPBack.png";	// HPゲージの背景画像ファイルパス
	const std::string hp_gauge_frame_img_file_path = "Data/Image/HPFrame.png";	// HPゲージの枠画像ファイルパス

	// 当たり判定の半径
	constexpr float collision_radius = 250.0f;
}

// コンストラクタ
BossMatrix::BossMatrix(int modelHandle, std::shared_ptr<Player> pPlayer, std::shared_ptr<LaserManager> pLaserManager) :
	m_attackStateIndex(0)
{
	// 初期化
	m_pPlayer = pPlayer;
	m_pLaserManager = pLaserManager;
	m_pos = init_pos;
	m_rot = model_rot;
	m_opacity = 0.0f;
	m_hp = max_hp;
	m_collisionRadius = collision_radius * model_scale.x;
	for (auto& point : move_pos)
	{
		m_movePointTable.push_back(point);
	}

	// ステートマシンの設定
	m_stateMachine.AddState(State::ENTRY, {}, [this]() {UpdateEntry(); }, {});
	m_stateMachine.AddState(State::IDLE, {}, [this]() {UpdateIdle(); }, {});
	m_stateMachine.AddState(State::MOVE, {}, [this]() {UpdateMove(); }, {});
	m_stateMachine.AddState(State::DIE, {}, [this]() {UpdateDie(); }, {});
	m_stateMachine.AddState(State::MOVE_NORMAL_LASER_ATTACK, {}, [this]() {UpdateMoveNormalLaserAttack(); }, {});
	m_stateMachine.AddState(State::MOVE_HOMING_LASER_ATTACK, {}, [this]() {UpdateMoveHomingLaserAttack(); }, {});
	m_stateMachine.AddState(State::CUBE_LASER_ATTACK, {}, [this]() {UpdateCubeLaserAttack(); }, {});
	m_stateMachine.SetState(State::ENTRY);

	// 攻撃ステートの追加
	m_attackStateTable.push_back(State::MOVE_NORMAL_LASER_ATTACK);
	m_attackStateTable.push_back(State::MOVE_HOMING_LASER_ATTACK);
	m_attackStateTable.push_back(State::CUBE_LASER_ATTACK);

	// HPゲージの設定
	m_pHpGauge = std::make_unique<Gauge>(
		hp_gauge_img_file_path, hp_gauge_back_img_file_path, hp_gauge_frame_img_file_path, max_hp, 
		hp_gauge_pos, hp_gauge_size, true, max_hp / 100, true, 3);

	// モデル設定
	m_pModel = std::make_shared<Model>(modelHandle);	// インスタンス生成
	m_pModel->SetUseCollision(true);					// 当たり判定設定
	m_pModel->SetOpacity(m_opacity);					// 不透明度	
	m_pModel->SetScale(model_scale);					// 拡大率
	m_pModel->SetRot(m_rot);							// 向き
	m_pModel->SetPos(m_pos);							// 位置
	m_pModel->Update();									// 当たり判定の更新
}

// デストラクタ
BossMatrix::~BossMatrix()
{
}

// 更新
void BossMatrix::Update()
{
	m_stateMachine.Update();
}

// 描画
void BossMatrix::Draw()
{
	// モデルの描画
	m_pModel->Draw();
}

// 攻撃ステートの順序をシャッフル
void BossMatrix::ShuffleAttackState()
{
	std::random_device seed;
	std::mt19937 engine(seed());
	std::shuffle(m_attackStateTable.begin(), m_attackStateTable.end(), engine);
}
