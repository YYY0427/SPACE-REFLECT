#include "EnemyManager.h"
#include "EnemyBase.h"
#include "Normal/Mosquito.h"
#include "Boss/BossMosquito.h"
#include "Boss/BossMatrix.h"
#include "../../String/StringUtil.h"
#include "../../Util/DrawFunctions.h"
#include "../../UI/Warning.h"
#include "../../UI/UIManager.h"
#include "../../MyDebug/DebugText.h"
#include "../../Input/InputManager.h"
#include "../../Util/FileUtil.h"
#include "../Player.h"
#include "../../Sound/SoundManager.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <DxLib.h>

namespace
{
	// �t�@�C���̊K�w
	const std::string wave_file_hierarchy = "Data/Csv/EnemyStage/";					// �X�e�[�W
	const std::string enemy_file_hierarchy = "Data/Csv/EnemyWave/";					// �E�F�[�u
	const std::string enemy_action_file_hierarchy = "Data/Csv/EnemyAction/";	// �G�̍s��

	// �t�@�C���̊g���q
	const std::string file_extension = ".csv";				

	// �{�X�G�o�ꎞ�̌x���̃t���[��
	constexpr int warning_frame = 180;

	// �f�o�b�O�p�̓G�ɗ^����_���[�W
	constexpr int debug_damage = 100000;

	// �{�X�o�ꎞ��BGM�̃t�F�[�h�C���̃t���[��
	constexpr int boss_bgm_fade_frame = 120;

	// �x��UI�̕`��D��x
	constexpr int warning_draw_priority = 0;

	// �x��UI�̊i�[���̃x�N�g��
	const Math::Vector2 warning_ui_store_vec = { 0, 0 };
}

// �R���X�g���N�^
EnemyManager::EnemyManager(const std::shared_ptr<Player>& pPlayer, 
						   const std::shared_ptr<LaserManager>& pLaserManager, 
						   const std::shared_ptr<Effect::ScreenShaker>& pScreenShaker) :
	m_waveNow(0),
	m_isNextWave(false),
	m_isLoadWave(false),
	m_isDeadBoss(false),
	m_isDeadEffectSound(false),
	m_isBoss(false),
	m_pPlayer(pPlayer),
	m_pLaserManager(pLaserManager),
	m_pScreenShaker(pScreenShaker),
	m_bossType(BossEnemyType::NONE),
	m_isStartWave(false)
{
	// �X�e�[�g�}�V���̐ݒ�
	m_stateMachine.AddState(State::NORMAL, {}, [this]() {UpdateNormal(); }, {});
	m_stateMachine.AddState(State::WARNING, {}, [this]() {UpdateWarning(); }, {});
	m_stateMachine.SetState(State::NORMAL);
}

// �f�X�g���N�^
EnemyManager::~EnemyManager()
{
}

// �X�V
void EnemyManager::Update()
{
	// �f�o�b�O�p
#ifdef _DEBUG
	// �G�̑S�Ńf�o�b�O
	if (Input::Manager::IsTriggered(Input::Type::ENEMY_DETH_DEBUG))
	{
		for (auto& enemy : m_pEnemyList)
		{
			enemy->OnDamage(debug_damage, Math::Vector3());
		}
		if (m_pBossEnemy)
		{
			m_pBossEnemy->OnDamage(debug_damage, Math::Vector3());
		}
	}
#endif

	// �{�X�̎��S���o����������
	if (IsDeadBossAnim() && !m_isDeadEffectSound)
	{
		// �{�X���S�G�t�F�N�g���̍Đ�
		Sound::Manager::GetInstance()->PlaySE("BossDeathEffect");

		// BGM�̒�~
		Sound::Manager::GetInstance()->StopBGM();

		// �t���O�𗧂Ă�
		m_isDeadEffectSound = true;
	}

	// �X�e�[�g�}�V���̍X�V
	m_stateMachine.Update();
}

// �ʏ펞�̍X�V
void EnemyManager::UpdateNormal()
{
	// ���݃t���O���������Ă���G�̍폜
	m_pEnemyList.remove_if([](std::shared_ptr<EnemyBase> data) { return !data->IsEnabled(); });

	// �G���G�̍X�V
	for (auto& enemy : m_pEnemyList)
	{
		enemy->Update();
	}

	// �{�X�G���o�����Ă�����
	if (m_pBossEnemy)
	{
		// �{�X�G���o�����Ă��āA���{�X�G���|����Ă�����
		if (!m_pBossEnemy->IsEnabled())
		{
			// �{�X�G���|���ꂽ���Ƃɂ���
			m_isDeadBoss = true;
		}

		// �X�V
		m_pBossEnemy->Update();
	}
}

// �x�����̍X�V
void EnemyManager::UpdateWarning()
{
	// �x�����I��������
	if (m_pWarning->IsEnd())
	{
		// �x���̍폜
		UIManager::GetInstance().DeleteUI("Warning");

		// �{�X�G�̐���
		AddBossEnemy(m_bossType);

		// �T�E���h�}�l�[�W���̎擾
		const auto& soundManager = Sound::Manager::GetInstance();

		// ���ݗ���Ă���BGM�̃t�F�[�h�A�E�g�̐ݒ�
		//soundManager->SetFadeSound(soundManager->GetPlayBGMFileName(), boss_bgm_fade_frame, soundManager->GetMaxVolume(), 0);

		// �{�X�G��BGM�̍Đ�
		soundManager->PlayBGM("BossBatleBgm");

		// �{�X�G��BGM�̃t�F�[�h�C���̐ݒ�
		soundManager->SetFadeSound("BossBatleBgm", boss_bgm_fade_frame, 0, soundManager->GetMaxVolume());

		// �X�e�[�g��ʏ�ɑJ��
		m_stateMachine.SetState(State::NORMAL);
	}
}

// �`��
void EnemyManager::Draw()
{
	// �G���G�̕`��
	for (auto& enemy : m_pEnemyList)
	{
		enemy->Draw();
	}

	// �{�X�G���o�����Ă�����`��
	if (m_pBossEnemy)
	{
		m_pBossEnemy->Draw();
	}

	// �G���G�̍��W�̕`��
	for (int i = 0; i < m_pEnemyList.size(); i++)
	{
		auto itr = m_pEnemyList.begin();
		std::advance(itr, i);

		Debug::Text::AddLog("NormalEnemyPos", { itr->get()->GetPos().x, itr->get()->GetPos().y, itr->get()->GetPos().z });
	}
}

// �E�F�[�u�̃X�^�[�g
void EnemyManager::StartWave()
{
	// ���ɃE�F�[�u���n�܂��Ă�����Ȃɂ����Ȃ�
	if (m_isStartWave) return;

	// �t���O�𗧂Ă�
	m_isStartWave = true;

	// �E�F�[�u�f�[�^��ǂݍ���ł��Ȃ�������~�߂�
	if (!m_isLoadWave)
	{
		assert(!"�E�F�[�u�f�[�^��ǂݍ���ł�������");
	}

	// �E�F�[�u�f�[�^���Ȃ�������
	if (m_waveTable.empty())
	{
		// �{�X�G�����Ȃ�������
		if (m_bossType == BossEnemyType::NONE)
		{
			// �{�X�G���|���ꂽ���Ƃɂ��ĂȂɂ����Ȃ�
			m_isBoss     = true;
			m_isDeadBoss = true;
			return;
		}

		// �x����ԂɑJ��
		m_stateMachine.SetState(State::WARNING);

		// �x���̐���
		m_pWarning = std::make_shared<Warning>(warning_frame);
		UIManager::GetInstance().AddUI("Warning", m_pWarning, warning_draw_priority, warning_ui_store_vec);
	}
	else
	{
		// �ŏ��̃E�F�[�u�f�[�^�����s
		auto& waveData = m_waveTable.front();

		// �G���G�̐���
		for (auto& data : waveData.enemyDataList)
		{
			AddEnemy(data);
		}
	}
}

// ���̃E�F�[�u��
void EnemyManager::NextWave()
{
	// �E�F�[�u���n�܂��Ă��Ȃ�������Ȃɂ����Ȃ�
	if(!m_isStartWave) return;

	// �E�F�[�u���I����Ă��Ȃ�������Ȃɂ����Ȃ�
	if (!IsEndWave())	return;

	// �f�[�^���Ȃ�������Ȃɂ����Ȃ�
	if(m_waveTable.empty()) return;

	// �Ō�̃E�F�[�u��������{�X�G�𐶐�
	if(m_waveNow == m_waveTable.size() - 1)
	{
		// �{�X�G�����Ȃ�������
		if (m_bossType == BossEnemyType::NONE)
		{
			// �{�X�G���|���ꂽ���Ƃɂ���
			m_isBoss     = true;
			m_isDeadBoss = true;
		}
		// �܂��x�����o�Ă��Ȃ�������
		else if(!m_pWarning)
		{
			// �x����ԂɑJ��
			m_stateMachine.SetState(State::WARNING);

			// �x���̐���
			m_pWarning = std::make_shared<Warning>(warning_frame);
			UIManager::GetInstance().AddUI("Warning", m_pWarning, warning_draw_priority, warning_ui_store_vec);
		}
	}
	else
	{
		// �E�F�[�u��i�߂�
		m_waveNow++;
		auto& waveData = m_waveTable[m_waveNow];

		// �G���G�̐���
		for (auto& data : waveData.enemyDataList)
		{
			AddEnemy(data);
		}
	}
}

// �G�̒ǉ�
void EnemyManager::AddEnemy(const EnemyData& data)
{
	// �G�̎�ނɂ���Đ�������G��ς���
	switch (data.type)
	{
	// ��
	case EnemyType::MOSQUITO:
		m_pEnemyList.push_back(std::make_shared<Mosquito>(
			data,
			m_pPlayer,
			m_pLaserManager));
		break;

	default:
		// �����ɗ�����G�̎�ނ��ǉ�����Ă��Ȃ�
		assert(!"�G�̎�ނ�����܂���");
	}
}

// �{�X�G�̒ǉ�
void EnemyManager::AddBossEnemy(const BossEnemyType& type)
{
	// ���Ƀ{�X�G����������Ă�����
	if(m_pBossEnemy || m_isBoss) return;

	// �t���O�𗧂Ă�
	m_isBoss = true;

	// ��ނɂ���ă{�X�G�̐���
	switch (type)
	{	
	case BossEnemyType::MOSQUITO:
		m_pBossEnemy = std::make_shared<BossMosquito>(
			m_pPlayer,
			m_pLaserManager);
		break;

	case BossEnemyType::MATRIX:
		m_pBossEnemy = std::make_shared<BossMatrix>(
			m_pPlayer,
			m_pLaserManager,
			m_pScreenShaker);
		break;

	case BossEnemyType::NONE:
		break;
	default:
		// �����ɗ�����G�̎�ނ��ǉ�����Ă��Ȃ�
		assert(!"�{�X�G�̎�ނ�����܂���");
	}
}

// �E�F�[�u�̃f�[�^�̓ǂݍ���
void EnemyManager::LoadEnemyStageFileData(const std::string& filePath)
{
	// �t���O�𗧂Ă�
	m_isLoadWave = true;

	// �t�@�C���̓ǂݍ���
	auto data = FileUtil::LoadCsvFile(wave_file_hierarchy + filePath + file_extension);
	for (auto& line : data)
	{
		// �G�̃f�[�^�̓ǂݍ���
		WaveData waveData{};
		for (int i = 0; i < line.size(); i++)
		{
			if (i == line.size() - 1)
			{
				// �{�X�G�̎�ނ̓ǂݍ���
				m_bossType = static_cast<BossEnemyType>(std::stoi(line[i]));
			}
			else
			{
				// �G�̃f�[�^�̒ǉ�
				waveData.enemyDataList = LoadEnemyWaveFileData(line[i]);

				// �E�F�[�u�̃f�[�^�̒ǉ�
				m_waveTable.push_back(waveData);
			}
		}
	}
}

// �G�̃f�[�^�̓ǂݍ���
std::vector<EnemyData> EnemyManager::LoadEnemyWaveFileData(const std::string& filePath)
{
	// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
	auto data = FileUtil::LoadCsvFile(enemy_file_hierarchy + filePath + file_extension);
	std::vector<EnemyData> dataTable;
	for (auto& line : data)
	{
		// ���W�̓ǂݍ���
		EnemyData enemyData{};
		enemyData.pos.x = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_X)]);
		enemyData.pos.y = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_Y)]);
		enemyData.pos.z = std::stof(line[static_cast<int>(EnemyWaveIndex::POS_Z)]);

		// ��ނ̓ǂݍ���
		enemyData.type = static_cast<EnemyType>(std::stoi(line[static_cast<int>(EnemyWaveIndex::TYPE)]));

		// HP�̓ǂݍ���
		enemyData.hp = std::stoi(line[static_cast<int>(EnemyWaveIndex::HP)]);

		// �U���͂̓ǂݍ���
		enemyData.attack = std::stoi(line[static_cast<int>(EnemyWaveIndex::ATTACK)]);

		// �傫���̓ǂݍ���
		enemyData.scale = std::stof(line[static_cast<int>(EnemyWaveIndex::SCALE)]);

		// �ړ����x�̓ǂݍ���
		enemyData.speed = std::stof(line[static_cast<int>(EnemyWaveIndex::SPEED)]);

		// �s���f�[�^�̓ǂݍ���
		enemyData.actionDataList = LoadEnemyActionFileData(line[static_cast<int>(EnemyWaveIndex::ACTION)]);

		// �f�[�^�̒ǉ�
		dataTable.push_back(enemyData);
	}
	return dataTable;
}

// �G�̍s���̃f�[�^�̓ǂݍ���
std::vector<EnemyActionData> EnemyManager::LoadEnemyActionFileData(const std::string& filePath)
{
	auto data = FileUtil::LoadCsvFile(enemy_action_file_hierarchy + filePath + file_extension);
	std::vector<EnemyActionData> dataTable;
	for (auto& line : data)
	{
		EnemyActionData enemyActionData{};

		// �ړI�n�̓ǂݍ���
		enemyActionData.goalPos.x = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_X)]);
		enemyActionData.goalPos.y = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_Y)]);
		enemyActionData.goalPos.z = std::stof(line[static_cast<int>(EnemyActionIndex::GOAL_POS_Z)]);

		// �ړI�n�ɓ��B���Ă��玟�̖ړI�n�Ɍ������܂ł̑ҋ@�t���[���̓ǂݍ���
		enemyActionData.idleFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::IDLE_FRAME)]);

		// �ړI�n�ɓ��B�����烌�[�U�[�𔭎˂��邩�ǂ����̃t���O�̓ǂݍ���
		enemyActionData.isLaser = std::stoi(line[static_cast<int>(EnemyActionIndex::IS_LASER)]);

		// ���[�U�[�𔭎˂���ꍇ
		if (enemyActionData.isLaser)
		{
			// ���[�U�[�̎�ނ̓ǂݍ���
			enemyActionData.laserType = static_cast<LaserType>(std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_TYPE)]));

			// ���[�U�[�𔭎˂���܂ł̃`���[�W�t���[���̓ǂݍ���
			enemyActionData.laserChargeFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_CHARGE_FRAME)]);

			// ���[�U�[�𔭎˂���܂ł̑ҋ@�t���[���̓ǂݍ���
			enemyActionData.laserIdleFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_IDLE_FRAME)]);

			// ���[�U�[�̑��x�̓ǂݍ���
			enemyActionData.laserSpeed = std::stof(line[static_cast<int>(EnemyActionIndex::LASER_SPEED)]);

			// ���[�U�[�����t���[���̊ԁA���˂��邩�̓ǂݍ���
			enemyActionData.laserFireFrame = std::stoi(line[static_cast<int>(EnemyActionIndex::LASER_FIRE_FRAME)]);

			// ���[�U�[���v���C���[��Ǐ]���邩�ǂ����̃t���O�̓ǂݍ���
			enemyActionData.isPlayerFollowing = std::stoi(line[static_cast<int>(EnemyActionIndex::IS_PLAYER_FOLLOWING)]);

		}
		// �f�[�^�̒ǉ�
		dataTable.push_back(enemyActionData);
	}
	return dataTable;
}

// �{�X�G���|����āA���S���o���I���������ǂ���
bool EnemyManager::IsDeadBoosEndAnim() const
{
	return m_isBoss && m_isDeadBoss;
}

// ���݂̃E�F�[�u���I���������ǂ���
bool EnemyManager::IsEndWave() const
{
	// �܂��G���c���Ă�����
	for (auto& enemy : m_pEnemyList)
	{
		if (enemy->IsEnabled())
		{
			return false;
		}
	}

	// �{�X�G���o�����Ă�����
	if (m_pBossEnemy)
	{
		return false;
	}

	// ����ȊO�̓E�F�[�u���I�����Ă���
	return true;
}

// �G���G�̃��X�g�̎擾
const std::list<std::shared_ptr<EnemyBase>>& EnemyManager::GetEnemyList() const
{
	return m_pEnemyList;
}

// �{�X�G�̎擾
const std::shared_ptr<EnemyBase>& EnemyManager::GetBossEnemy() const
{
	return m_pBossEnemy;
}

// �{�X�G���������Ă��邩�ǂ���
bool EnemyManager::IsBossAlive() const
{
	return (m_pBossEnemy && m_isBoss);
}

// �{�X���|����A���S���o�����ǂ����̎擾
bool EnemyManager::IsDeadBossAnim() const
{
	// �{�X�G��������
	if (m_pBossEnemy)
	{
		return m_pBossEnemy->IsDeadAnim();
	}
	return false;
}