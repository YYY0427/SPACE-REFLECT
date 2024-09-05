#pragma once
#include <map>
#include <vector>

namespace Input	
{
	// �{�^���̓��̓^�C�v
	enum class Type
	{
		DECISION,	// ����
		BACK,		// �߂�
		PAUSE,		// �|�[�Y
		SHIELD,		// �V�[���h���o��
		UP,			// ��
		DOWN,		// ��
		RIGHT,		// �E
		LEFT,		// ��
		RIGHT_SHOULDER,	// �E�V�����_�[
		LEFT_SHOULDER,	// ���V�����_�[
		ANY_BUTTON,		// �ǂ̃{�^���ł�����

		// �f�o�b�O�p
		NORMAL_ENEMY_DEBUG,	// �G���G�����R�ɓ�������
		ENEMY_DETH_DEBUG,	// �{�X�̎��S

		// �{�^���̓��̓^�C�v�̍��v�l
		NUM
	};

	// �p�b�h�̃g���K�[�̍����E���܂��̓p�b�h�̃X�e�B�b�N�̍����E��
	enum class PadLR
	{
		LEFT,	// �g���K�[�܂��̓X�e�B�b�N�̍�
		RIGHT,	// �g���K�[�܂��̓X�e�B�b�N�̉E
	};

	// �p�b�h�̃X�e�B�b�N�̓��̓^�C�v
	enum class PadStickInputType
	{
		UP,				// ��
		DOWN,			// ��
		LEFT,			// ��
		RIGHT,			// �E
	};

	// ���͑��u�J�e�S��
	enum class InputCategory
	{
		KEYBORD,	// �L�[�{�[�h
		PAD,		// �Q�[���p�b�h
		MOUSE		// �}�E�X
	};

	// ���͏��
	struct InputInfo
	{
		InputCategory cat;	// ���͑��u�J�e�S��
		int id;				// ����ID(KEY_INPUT_�`,PAD_INPUT_�`,MOUSE_INPUT_�`)
	};

	/// <summary>
	/// ���͏�Ԃ��Ǘ�����
	/// </summary>
	namespace Manager
	{
		/// <summary>
		/// ������
		/// </summary>
		void Init();

		/// <summary>
		/// ���͏��̍X�V 
		/// ���t���[��Update���Ă΂Ȃ��Ɠ��͏�Ԃ͍X�V����Ȃ�
		/// </summary>
		void Update();

		/// <summary>
		/// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
		/// </summary>
		/// <param name="type">�擾���������̓^�C�v</param>
		/// <returns>true : ���́Afalse : �����</returns>
		bool IsTriggered(const Type type);

		/// <summary>
		/// �{�^����������Ă���Ԃ̓��͏��̎擾
		/// </summary>
		/// <param name="type">�擾���������̓^�C�v</param>
		/// <returns>true : ���͒��Afalse : �����</returns>
		bool IsPressed(const Type type);

		/// <summary>
		/// �p�b�h�̃g���K�[�̓��͏��̎擾
		/// </summary>
		/// <param name="type">�擾���������̓^�C�v</param>
		/// <returns>true : ���͒��Afalse : �����</returns>
		bool IsPadTrigger(const PadLR type);

		/// <summary>
		/// �p�b�h�̃X�e�B�b�N�̓��͏����擾
		/// </summary>
		/// <param name="stick">�X�e�B�b�N�̉E������</param>
		/// <param name="input">�X�e�B�b�N�̂ǂ̓��̓^�C�v��</param>
		/// <returns>�X�e�B�b�N�̌X�����킢(0�̂Ƃ��͓��͂Ȃ�)�@�X���Ă���p�x���傫���قǑ傫���������Ԃ��Ă���</returns>
		int IsPadStick(const PadLR stic, const PadStickInputType type);

		// private
		namespace
		{
			// ���z���͏��Ǝ��ۂ̓��͂̃e�[�u��
			std::map<Type, std::vector<InputInfo>> m_inputMapTable;

			// ���݂̓��͏��(�����Ă邩�����ĂȂ���)
			std::vector<bool> m_currentInput;

			// ���O�̓��͏��(���O�����Ă邩�����ĂȂ���)
			std::vector<bool> m_lastInput;
		}
	}
}