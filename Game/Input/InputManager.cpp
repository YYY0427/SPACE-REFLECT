#include "InputManager.h"
#include <DxLib.h>
#include <map>
#include <vector>
#include <string>
#include <cmath>

namespace Input::Manager
{
	// ������
	void Input::Manager::Init()
	{
		// �{�^���̓��̓^�C�v���ǂ̃{�^���ɂ��邩�̐ݒ�
		m_inputMapTable[Type::DECISION] = { {InputCategory::PAD, XINPUT_BUTTON_A },				// A�{�^��
												 {InputCategory::KEYBORD, KEY_INPUT_RETURN} };			// Enter�L�[
		m_inputMapTable[Type::BACK]     = { {InputCategory::PAD, XINPUT_BUTTON_B},					// B�{�^��
												 {InputCategory::KEYBORD, KEY_INPUT_SPACE} };			// SPACE�L�[	
		m_inputMapTable[Type::PAUSE]    = { {InputCategory::PAD, XINPUT_BUTTON_START },			// START�{�^��
												 {InputCategory::KEYBORD, KEY_INPUT_P} };				// P�L�[
		m_inputMapTable[Type::SHIELD]   = { {InputCategory::PAD, XINPUT_BUTTON_RIGHT_SHOULDER},	// RB�{�^��
											     {InputCategory::PAD, XINPUT_BUTTON_LEFT_SHOULDER} };	// LB�{�^��
		m_inputMapTable[Type::UP]       = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_UP },			// PAD��
												 {InputCategory::KEYBORD, KEY_INPUT_UP},				// KEY��
												 {InputCategory::KEYBORD, KEY_INPUT_W} };				// W�L�[
		m_inputMapTable[Type::DOWN]     = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_DOWN },		// PAD��
												 {InputCategory::KEYBORD, KEY_INPUT_DOWN},				// KEY��
												 {InputCategory::KEYBORD, KEY_INPUT_S} };				// S�L�[
		m_inputMapTable[Type::RIGHT]    = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_RIGHT },		// PAD��
												 {InputCategory::KEYBORD, KEY_INPUT_RIGHT},				// KEY��
												 {InputCategory::KEYBORD, KEY_INPUT_D} };				// D�L�[
		m_inputMapTable[Type::LEFT]     = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_LEFT },		// PAD��
												 {InputCategory::KEYBORD, KEY_INPUT_LEFT},				// KEY��
												 {InputCategory::KEYBORD, KEY_INPUT_A} };				// A�L�[
		m_inputMapTable[Type::RIGHT_SHOULDER] = { {InputCategory::PAD, XINPUT_BUTTON_RIGHT_SHOULDER} };// RB�{�^��
		m_inputMapTable[Type::LEFT_SHOULDER]  = { {InputCategory::PAD, XINPUT_BUTTON_LEFT_SHOULDER} };	// LB�{�^��
		m_inputMapTable[Type::ANY_BUTTON] = { {InputCategory::PAD, XINPUT_BUTTON_A },
													{InputCategory::PAD, XINPUT_BUTTON_B },
													{InputCategory::PAD, XINPUT_BUTTON_X },
													{InputCategory::PAD, XINPUT_BUTTON_Y } };

#ifdef _DEBUG
		// �f�o�b�O�p�@
		m_inputMapTable[Type::ENEMY_DETH_DEBUG] = { {InputCategory::KEYBORD, KEY_INPUT_R} };	// �G�̎��S
#endif

		// �ݒ肵���{�^���̐��ɂ���Ĕz��̐���ύX
		m_currentInput.resize(static_cast<int>(Type::NUM));
		m_lastInput.resize(static_cast<int>(Type::NUM));
	}

	// �X�V
	void Input::Manager::Update()
	{
		// ���O�̓��͏����L�����Ă���
		m_lastInput = m_currentInput;

		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �L�[���̎擾
		char keystate[256];
		GetHitKeyStateAll(keystate);

		// �}�E�X���̎擾
		int mouseState = GetMouseInput();

		// �}�b�v�̑S�������[�v
		for (const auto& keymap : m_inputMapTable)
		{
			// ���͏��z������[�v
			for (const auto& input : keymap.second)
			{
				// ���͏�񂩂�ǂ̃{�^�������͂���Ă��邩�m�F
				if (input.cat == InputCategory::KEYBORD)
				{
					m_currentInput[static_cast<int>(keymap.first)] = keystate[input.id];
				}
				else if (input.cat == InputCategory::PAD)
				{
					m_currentInput[static_cast<int>(keymap.first)] = padState.Buttons[input.id];
				}
				else if (input.cat == InputCategory::MOUSE)
				{
					m_currentInput[static_cast<int>(keymap.first)] = mouseState & input.id;
				}
				// 3�̓��͂̂����ǂꂩ��true����������͂���Ă邩��break
				if (m_currentInput[static_cast<int>(keymap.first)])
				{
					break;
				}
			}
		}
	}

	// �{�^���������ꂽ�u�Ԃ̓��͏��̎擾
	bool Input::Manager::IsTriggered(const Type type)
	{
		return IsPressed(type) && !m_lastInput[static_cast<int>(type)];
	}

	// �{�^����������Ă���Ԃ̓��͏��̎擾
	bool Input::Manager::IsPressed(const Type type)
	{
		return m_currentInput[static_cast<int>(type)];
	}

	// �p�b�h�̃g���K�[�̓��͏��̎擾
	bool Input::Manager::IsPadTrigger(const PadLR type)
	{
		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �g���K�[�̍����E��
		if (type == PadLR::LEFT)
		{
			// �g���K�[�������ȏ㉟����Ă�����true
			return 	padState.LeftTrigger > padState.LeftTrigger / 2;
		}
		else
		{
			// �g���K�[�������ȏ㉟����Ă�����true
			return 	padState.RightTrigger > padState.RightTrigger / 2;
		}
	}

	// �p�b�h�̃X�e�B�b�N�̓��͏����擾
	int Input::Manager::IsPadStick(const PadLR stic, const PadStickInputType type)
	{
		// �p�b�h�̏��̎擾
		XINPUT_STATE  padState;
		GetJoypadXInputState(DX_INPUT_PAD1, &padState);

		// �E�X�e�B�b�N�����X�e�B�b�N��
		float x, y;
		if (stic == PadLR::LEFT)
		{
			// padState����擾�����l��0.0~10.0�ɕϊ�
			x = static_cast<float>(padState.ThumbLX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbLY / 32767.0f * 10);
		}
		else
		{
			// padState����擾�����l��0.0~10.0�ɕϊ�
			x = static_cast<float>(padState.ThumbRX / 32767.0f * 10);
			y = static_cast<float>(padState.ThumbRY / 32767.0f * 10);
		}
		if (type == PadStickInputType::LEFT && x < -0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (type == PadStickInputType::RIGHT && x > 0.1f)
		{
			return abs(static_cast<int>(x));
		}
		if (type == PadStickInputType::UP && y > 0.1f)
		{
			return abs(static_cast<int>(y));
		}
		if (type == PadStickInputType::DOWN && y < -0.1f)
		{
			return abs(static_cast<int>(y));
		}

		// ���͂Ȃ�
		return 0;
	}
}