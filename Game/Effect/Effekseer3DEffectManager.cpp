#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "../Util/DrawFunctions.h"
#include "../Util/FileUtil.h"
#include <cassert>
#include <DxLib.h>

namespace
{
	// �G�t�F�N�g�̃t�@�C���p�X
	const std::string effect_list_file_path = "Data/Csv/Effect.csv";

	// Effekseer�p�̉摜�t�@�C���p�X
	const std::string img_file_path = "Data/Effect/background.png";

	// �G�t�F�N�g�̍ő�p�[�e�B�N����
	constexpr int max_particle = 8000;
}

namespace Effect
{
	// �R���X�g���N�^
	Effekseer3DManager::Effekseer3DManager() :
		m_imgHandle(-1)
	{
	}

	// �f�X�g���N�^
	Effekseer3DManager::~Effekseer3DManager()
	{
		// �I������
		End();
	}

	// Effekseer�̏������ƃG�t�F�N�g�̃��[�h
	void Effekseer3DManager::Init()
	{
		// Effekseer������������
		// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��
		if (Effkseer_Init(max_particle) == -1)
		{
			// �������̎��s
			assert(!"Effekseer�̏������Ɏ��s");
		}

		// �摜�̃��[�h
		m_imgHandle = my::MyLoadGraph(img_file_path.c_str());
	}

	// �X�V
	void Effekseer3DManager::Update()
	{
		for (auto& effect : m_effectDataTable)
		{
			// �Đ��^�C�v�����[�v�����ʂ�Ȃ�
			if (effect.type != PlayType::LOOP) continue;

			// �Đ�������Ȃ��ꍇ�����ʂ�Ȃ�
			if (IsPlayingEffect(*effect.playingEffectHandle)) continue;

			// �G�t�F�N�g�̍čĐ�
			PlayEffectLoop(*effect.playingEffectHandle, effect.effectId);

			// �G�t�F�N�g�̃p�����[�^�[�̐ݒ�
			SetEffectAllParam(*effect.playingEffectHandle, effect.pos, effect.scale, effect.speed, effect.rot);
		}
		for (auto& effect : m_followEffectDataTable)
		{
			// �Đ��^�C�v�����[�v��
			bool isLoop = effect.type == PlayType::LOOP_AND_FOLLOW;

			// �Đ�����
			bool isPlay = IsPlayingEffect(*effect.playingEffectHandle);

			// �Đ��^�C�v�����[�v�A�Đ����ł͂Ȃ��ꍇ
			if (isLoop && !isPlay)
			{
				// �G�t�F�N�g�̍čĐ�
				PlayEffectLoop(*effect.playingEffectHandle, effect.effectId);
			}

			// �Đ����̏ꍇ�G�t�F�N�g�̃p�����[�^�[�̍X�V
			if (isPlay)
			{
				SetEffectAllParam(*effect.playingEffectHandle, *effect.pos, effect.scale, effect.speed, effect.rot);
			}
		}

		// �Đ����I�������G�t�F�N�g���������ꍇ�폜
		m_effectDataTable.remove_if([this](EffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });
		m_followEffectDataTable.remove_if([this](FollowEffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });

		// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����
		Effekseer_Sync3DSetting();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����
		UpdateEffekseer3D();
	}

	// �`��
	void Effekseer3DManager::Draw()
	{
		// ���ł������̂ŉ摜��`�悷��
		// �������ĕ`�悵����łȂ��ƁAEffekseer�͕`��ł��Ȃ�
		DrawGraph(0, 0, m_imgHandle, true);

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷�� 
		DrawEffekseer3D();

#if false
		// �G�t�F�N�g�̕`��J�n
		DrawEffekseer3D_Begin();

		for (auto& effect : m_effectDataTable)
		{
			// Z�o�b�t�@���g�����ǂ���
			SetUseZBuffer3D(effect.isZBuffer);

			// Effekseer�ɂ��Đ����̃G�t�F�N�g��`��
			int result = DrawEffekseer3D_Draw(*effect.playingEffectHandle);
			assert(result != -1);

			// Z�o�b�t�@�ݒ�����ɖ߂�
			SetUseZBuffer3D(true);
		}
		for (auto& effect : m_followEffectDataTable)
		{
			// Z�o�b�t�@���g�����ǂ���
			SetUseZBuffer3D(effect.isZBuffer);

			// Effekseer�ɂ��Đ����̃G�t�F�N�g��`��
			int result = DrawEffekseer3D_Draw(*effect.playingEffectHandle);
			assert(result != -1);

			// Z�o�b�t�@�ݒ�����ɖ߂�
			SetUseZBuffer3D(true);
		}
		// �G�t�F�N�g�̕`��I��
		DrawEffekseer3D_End();
#endif
	}

	// �I������
	void Effekseer3DManager::End()
	{
		// �摜�̍폜
		DeleteGraph(m_imgHandle);

		// �G�t�F�N�g���\�[�X���폜����(Effekseer�I�����ɔj�������̂ō폜���Ȃ��Ă��������ꉞ)
		for (auto& effectResource : m_effectResourceTable)
		{
			DeleteEffekseerEffect(effectResource.second.effectHandle);
		}

		// �z��̍폜
		m_effectResourceTable.clear();
		m_effectDataTable.clear();
		m_followEffectDataTable.clear();

		// Effekseer���I������
		Effkseer_End();
	}

	// �G�t�F�N�g�̃��[�h
	void Effekseer3DManager::LoadEffectsFromCsv()
	{
		// �t�@�C���̓ǂݍ���
		auto data = FileUtil::LoadCsvFile(effect_list_file_path);

		for (auto& list : data)
		{
			auto& id = list[static_cast<int>(EffectFileType::ID)];
			auto& path = list[static_cast<int>(EffectFileType::FILE_PATH)];

			// �����t�@�C���p�X���o�^����Ă���ꍇ�͂��̃n���h�����g��
			for (auto& effect : m_effectResourceTable)
			{
				if (effect.second.effectFilePath == path)
				{
					m_effectResourceTable[id] = effect.second;
					continue;
				}
			}

			// �G�t�F�N�g�̃t�@�C���p�X��ۑ�
			m_effectResourceTable[id].effectFilePath = path;

			// �Ȃ�Effekseer�̔񓯊����[�h�����܂������Ȃ��̂œ������[�h�ɂ���
			SetUseASyncLoadFlag(false);

			// �G�t�F�N�g�̃��[�h
			m_effectResourceTable[id].effectHandle = LoadEffekseerEffect(path.c_str());

			// �񓯊����[�h�ɖ߂�
			SetUseASyncLoadFlag(true);

			// �G�t�F�N�g�̃��[�h�Ɏ��s�����ꍇ�͎~�߂�
			assert(m_effectResourceTable[list[static_cast<int>(EffectFileType::ID)]].effectHandle != -1 && "�G�t�F�N�g�̃��[�h�Ɏ��s������");
		}
	}

	// ���[�v�G�t�F�N�g�̍Đ�
	void Effekseer3DManager::PlayEffectLoop(int& playingEffectHandle, const std::string& id)
	{
		// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
		assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

		// �G�t�F�N�g�̍čĐ�
		playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);
	}

	// �w��̃G�t�F�N�g�̍Đ�
	void Effekseer3DManager::PlayEffect(int& playingEffectHandle, const std::string& id, const Math::Vector3& pos, const Math::Vector3& scale, const float speed, const Math::Vector3& rot, const bool isZbuffer)
	{
		// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
		assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

		// �G�t�F�N�g�̍Đ�
		playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

		// �G�t�F�N�g�̃p�����[�^�[�̐ݒ�
		SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

		// �f�[�^��ۑ�
		EffectData effect{};
		effect.playingEffectHandle = &playingEffectHandle;
		effect.type = PlayType::NORMAL;
		effect.isZBuffer = isZbuffer;
		m_effectDataTable.push_back(effect);
	}

	// �w��̃G�t�F�N�g�̃��[�v�Đ�
	void Effekseer3DManager::PlayEffectLoop(int& playingEffectHandle, const std::string& id, const Math::Vector3& pos, const Math::Vector3& scale, const float speed, const Math::Vector3& rot, const bool isZbuffer)
	{
		// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
		assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

		// �G�t�F�N�g�̍Đ�
		playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

		// �G�t�F�N�g�̃p�����[�^�[�̐ݒ�
		SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

		// �f�[�^��ۑ�
		EffectData effect{};
		effect.playingEffectHandle = &playingEffectHandle;
		effect.effectId = id;
		effect.pos = pos;
		effect.scale = scale;
		effect.speed = speed;
		effect.rot = rot;
		effect.type = PlayType::LOOP;
		effect.isZBuffer = isZbuffer;
		m_effectDataTable.push_back(effect);
	}

	// �w��̃G�t�F�N�g�̒Ǐ]�Đ�
	void Effekseer3DManager::PlayEffectFollow(int& playingEffectHandle, const std::string& id, Math::Vector3* pos, const Math::Vector3& scale, const float speed, const Math::Vector3& rot, const bool isZbuffer)
	{
		// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
		assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

		// �G�t�F�N�g�̍Đ�
		playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

		// �G�t�F�N�g�̃p�����[�^�[�̐ݒ�
		SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

		// �f�[�^��ۑ�
		FollowEffectData effect{};
		effect.playingEffectHandle = &playingEffectHandle;
		effect.effectId = id;
		effect.pos = pos;
		effect.scale = scale;
		effect.speed = speed;
		effect.rot = rot;
		effect.type = PlayType::FOLLOW;
		effect.isZBuffer = isZbuffer;
		m_followEffectDataTable.push_back(effect);
	}

	// �w��̃G�t�F�N�g�̒Ǐ]�ƃ��[�v�Đ�
	void Effekseer3DManager::PlayEffectLoopAndFollow(int& playingEffectHandle, const std::string& id, Math::Vector3* pos, const Math::Vector3& scale, const float speed, const Math::Vector3& rot, const bool isZbuffer)
	{
		// �G�t�F�N�g���\�[�X�Ɏw�肵���G�t�F�N�g�����[�h����Ă��Ȃ��ꍇ�~�߂�
		assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

		// �G�t�F�N�g�̍Đ�
		playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

		// �G�t�F�N�g�̃p�����[�^�[�̐ݒ�
		SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

		// �f�[�^��ۑ�
		FollowEffectData effect{};
		effect.playingEffectHandle = &playingEffectHandle;
		effect.effectId = id;
		effect.pos = pos;
		effect.scale = scale;
		effect.speed = speed;
		effect.rot = rot;
		effect.type = PlayType::LOOP_AND_FOLLOW;
		effect.isZBuffer = isZbuffer;
		m_followEffectDataTable.push_back(effect);
	}

	// �G�t�F�N�g�̑S�Ă̒l�̕ύX
	void Effekseer3DManager::SetEffectAllParam(const int playingEffectHandle, const Math::Vector3& pos, const Math::Vector3& scale, const float speed, const Math::Vector3& rot)
	{
		// �Đ����x
		SetEffectSpeed(playingEffectHandle, speed);

		// �g�嗦
		SetEffectScale(playingEffectHandle, scale);

		// ��]��
		SetEffectRot(playingEffectHandle, rot);

		// �ʒu
		SetEffectPos(playingEffectHandle, pos);
	}

	// �G�t�F�N�g�̊g�嗦�̕ύX
	void Effekseer3DManager::SetEffectScale(const int playingEffectHandle, const Math::Vector3& scale)
	{
		// �G�t�F�N�g�̊g�嗦�̐ݒ�
		int result = SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale.x, scale.y, scale.z);

		// ���s������~�߂�
		assert(result != -1);

		// �G�t�F�N�g�̃f�[�^��ۑ����Ă���e�[�u���̒l��ύX����
		// �ǂ����̃e�[�u���Ɋi�[����Ă��邩������Ȃ����ߗ����̃e�[�u�����m�F����
		for (auto& effect : m_effectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.scale = scale;
			}
		}
		for (auto& effect : m_followEffectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.scale = scale;
			}
		}
	}

	// �G�t�F�N�g�̍Đ����x�̕ύX
	void Effekseer3DManager::SetEffectSpeed(const int playingEffectHandle, const float speed)
	{
		// �G�t�F�N�g�̍Đ����x�̐ݒ�
		int result = SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);

		// ���s������~�߂�
		assert(result != -1);

		// �G�t�F�N�g�̃f�[�^��ۑ����Ă���e�[�u���̒l��ύX����
		// �ǂ����̃e�[�u���Ɋi�[����Ă��邩������Ȃ����ߗ����̃e�[�u�����m�F����
		for (auto& effect : m_effectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.speed = speed;
			}
		}
		for (auto& effect : m_followEffectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.speed = speed;
			}
		}
	}

	// �G�t�F�N�g�̉�]���̕ύX
	void Effekseer3DManager::SetEffectRot(const int playingEffectHandle, const Math::Vector3& rot)
	{
		// �G�t�F�N�g�̉�]���̐ݒ�
		int result = SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);

		// ���s������~�߂�
		assert(result != -1);

		// �G�t�F�N�g�̃f�[�^��ۑ����Ă���e�[�u���̒l��ύX����
		// �ǂ����̃e�[�u���Ɋi�[����Ă��邩������Ȃ����ߗ����̃e�[�u�����m�F����
		for (auto& effect : m_effectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.rot = rot;
			}
		}
		for (auto& effect : m_followEffectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.rot = rot;
			}
		}
	}

	// �G�t�F�N�g�̈ʒu�̕ύX
	void Effekseer3DManager::SetEffectPos(const int playingEffectHandle, const Math::Vector3& pos)
	{
		// �G�t�F�N�g�̈ʒu�̐ݒ�
		int result = SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);

		// ���s������~�߂�
		assert(result != -1);

		// �G�t�F�N�g�̃f�[�^��ۑ����Ă���e�[�u���̒l��ύX����
		// �Ǐ]�G�t�F�N�g�͕K�v�Ȃ��̂ŕύX���Ȃ�
		for (auto& effect : m_effectDataTable)
		{
			if (*effect.playingEffectHandle == playingEffectHandle)
			{
				effect.pos = pos;
			}
		}
	}

	// ����̃G�t�F�N�g���Đ�����
	bool Effekseer3DManager::IsPlayingEffect(const int playingEffectHandle)
	{
		if (IsEffekseer3DEffectPlaying(playingEffectHandle) == -1)
		{
			return false;
		}
		return true;
	}

	// ����̃G�t�F�N�g�̍Đ����X�g�b�v
	void Effekseer3DManager::StopEffect(const int playingEffectHandle)
	{
		int result = StopEffekseer3DEffect(playingEffectHandle);
		assert(result != -1);
	}

	// �G�t�F�N�g�S�Ă̍Đ����X�g�b�v
	void Effekseer3DManager::StopAllEffect()
	{
		for (auto& effect : m_effectDataTable)
		{
			StopEffect(*effect.playingEffectHandle);
		}
		for (auto& effect : m_followEffectDataTable)
		{
			StopEffect(*effect.playingEffectHandle);
		}
	}

	// ����̃G�t�F�N�g�̍폜
	void Effekseer3DManager::DeleteEffect(const int playingEffectHandle)
	{
		// �G�t�F�N�g�̍Đ����~�߂�
		StopEffect(playingEffectHandle);

		// �ǂ����̃e�[�u���Ɋi�[����Ă��邩������Ȃ����ߗ����̃e�[�u�����m�F���n���h������v�����f�[�^���폜
		m_effectDataTable.remove_if(
			[playingEffectHandle](EffectData data)
			{ return *data.playingEffectHandle == playingEffectHandle; });
		m_followEffectDataTable.remove_if(
			[playingEffectHandle](FollowEffectData data)
			{ return *data.playingEffectHandle == playingEffectHandle; });
	}

	// �S�ẴG�t�F�N�g�̍폜
	void Effekseer3DManager::DeleteAllEffect()
	{
		// �S�ẴG�t�F�N�g�̍Đ����~�߂�
		StopAllEffect();

		// �S�Ẵf�[�^�̍폜
		m_effectDataTable.clear();
		m_followEffectDataTable.clear();
	}
}