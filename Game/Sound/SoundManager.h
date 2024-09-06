#pragma once
#include "../Math/Vector3.h"
#include "../Util/SingletonBase.h"
#include <map>
#include <string>
#include <DxLib.h>

namespace Sound
{
	/// <summary>
	/// �T�E���h�̊Ǘ����s���V���O���g���N���X
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������
		friend class SingletonBase<Manager>;

	private:
		// �T�E���h�̎��
		enum class SoundType
		{
			BGM,
			SE2D,
			SE3D,
		};

		// �T�E���h�̃f�[�^�̎��
		enum class SoundFileDataType
		{
			FILE_NAME,	// �t�@�C����
			EXTENSION,	// �g���q
			SOUND_TYPE,	// �T�E���h�̎��
			VOLUM_RATE,	// ���ʂ̓������[�g
		};

	private:
		// �T�E���h�t�F�[�h�f�[�^
		struct SoundFadeData
		{
			int  currentFadeFrame;	// ���݂̃t�F�[�h�t���[��
			int  fadeFrame;			// �t�F�[�h�̃t���[����
			int  initVolume;		// ��������
			int  afterVolume;		// �ŏI�I�ȉ���
			bool isFade;			// �t�F�[�h����
		};

		// �T�E���h�̃f�[�^
		struct SoundData
		{
			SoundFadeData fadeData;		// �t�F�[�h�f�[�^
			SoundType     type;			// BGM��3D��SE��2D��SE��
			std::string   extension;	// �T�E���h�t�@�C���̊g���q
			float		  volumeRate;	// �{�����[������
			int 		  volume;		// �{�����[��
			int			  handle;		// �n���h��
		};

	public:
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Manager();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �I������
		/// </summary>
		void End();

		/// <summary>
		/// �T�E���h��CSV�t�@�C����ǂݍ���
		/// </summary>
		void LoadSoundFromCsv();

		/// <summary>
		/// �w���2DSE��炷
		/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�A2DSE�ȊO�̏ꍇ�͎~�܂�
		/// </summary>
		/// <param name="name">�t�@�C����</param>
		void PlaySE(const std::string& fileName);

		/// <summary>
		/// �w���2DSE�̃��[�v�Đ�
		/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�A2DSE�ȊO�̏ꍇ�͎~�܂�</summary>
		/// <param name="fileName"></param>
		void PlaySELoop(const std::string& fileName);

		/// <summary>
		/// �w���3DSE��炷
		/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�A3DSE�ȊO�̏ꍇ�͎~�܂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="soundPos">�Đ��ʒu</param>
		/// <param name="soundRadius">�������鋗��</param>
		void PlaySE3D(const std::string& fileName, const Math::Vector3& soundPos, const float soundRadius);

		/// <summary>
		/// �w���BGM��炷
		/// �T�E���h�����[�h����Ă��Ȃ��ꍇ�ABGM�ȊO�̏ꍇ�͎~�܂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		void PlayBGM(const std::string& fileName);

		/// <summary>
		/// ����̃T�E���h���Đ������`�F�b�N
		/// �T�E���h�����[�h����Ă��Ȃ�������~�߂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
		bool IsPlaySound(const std::string& fileName);

		/// <summary>
		/// ����̃T�E���h���~�߂�
		/// �T�E���h�����[�h����Ă��Ȃ�������~�߂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		void StopSound(const std::string& fileName);

		/// <summary>
		/// ���ׂẴT�E���h���~�߂�
		/// </summary>
		void StopAllSound();

		/// <summary>
		/// BGM���~�߂�
		/// </summary>
		void StopBGM();

		/// <summary>
		/// ���ʒ���
		/// �ݒ肵�������ʂƃT�E���h�ɐݒ肳�ꂽ���ʂƃR���t�B�O�Őݒ肳�ꂽ���ʂ��狁�߂��ŏI�I�ȉ��ʂɒ���
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="volume">�ݒ肵��������(0~255)</param>
		void SetVolume(const std::string& fileName, const int volume);

		/// <summary>
		/// �T�E���h�t�F�[�h�̐ݒ�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="fadeFrame">�t���[����</param>
		/// <param name="initVolume">��������</param>
		/// <param name="afterVolume">�ŏI����</param>
		void SetFadeSound(const std::string& fileName, const int fadeFrame, const int initVolume, const int afterVolume);

		/// <summary>
		/// 3D�T�E���h�̃��X�i�[�̈ʒu�ƃ��X�i�[�̑O���ʒu��ݒ肷��
		/// </summary>
		/// <param name="pos">���X�i�[�̍��W</param>
		/// <param name="angle">���X�i�[�̌����Ă���p�x(���W�A��)</param>
		void Set3DSoundListenerPosAndFrontPos_UpVecY(const Math::Vector3& pos, const Math::Vector3& angle);

		/// <summary>
		/// �T�E���h�̃��[�v�͈͂�ݒ�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="startTime">�J�n����</param>
		/// <param name="endTime">�I������</param>
		void SetLoopAreaTimePos(const std::string& fileName, LONGLONG startTime, LONGLONG endTime);

		/// <summary>
		/// �T�E���h�̍Đ����Ԃ��擾
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�Đ�����</returns>
		LONGLONG GetSoundTotalTime(const std::string& fileName);

		/// <summary>
		/// �T�E���h�̉��ʂ��擾
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <returns>�T�E���h����</returns>
		int GetSoundVolume(const std::string& fileName);

		/// <summary>
		/// �Đ�����BGM�̃t�@�C�����̎擾
		/// �Đ�����BGM���Ȃ��ꍇ�͋󕶎���Ԃ�
		/// </summary>
		/// <returns>�Đ�����BGM�̃t�@�C����</returns>
		std::string GetPlayBGMFileName() const;

		/// <summary>
		/// BGM���Đ������`�F�b�N
		/// </summary>
		/// <returns>true : �Đ����Afalse : �Đ����Ă��Ȃ�</returns>
		bool IsPlayBGM() const;

		/// <summary>
		/// �ő�̉��ʂ��擾
		/// </summary>
		/// <returns>�ő�̉���</returns>
		int GetMaxVolume() const;

	private:
		/// <summary>
		/// �R���X�g���N�^
		/// �V���O���g���p�^�[���Ȃ̂�private</summary>
		Manager();

		/// <summary>
		/// 2D�T�E���h�̃��[�h
		/// ���[�h�Ɏ��s������~�߂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="extension">�g���q</param>
		void LoadSoundFile2D(const std::string& fileName, const std::string& extension, SoundType type);

		/// <summary>
		/// 3D�T�E���h�̃��[�h
		/// ���[�h�Ɏ��s������~�߂�
		/// </summary>
		/// <param name="fileName">�t�@�C����</param>
		/// <param name="extension">�g���q</param>
		void LoadSoundFile3D(const std::string& fileName, const std::string& extension);

	private:
		// ���[�h�����T�E���h�̃t�@�C������ID�Ƃ����T�E���h�f�[�^�̃e�[�u��
		std::map<std::string, SoundData> m_soundDataTable;
	};
}