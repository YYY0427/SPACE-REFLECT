#include "InputState.h"
#include <DxLib.h>
#include <map>
#include <vector>
#include <string>
#include <cmath>

// 静的メンバ変数の実体を定義
std::map<InputType, std::vector<InputInfo>> InputState::m_inputMapTable;
std::vector<bool> InputState::m_currentInput;
std::vector<bool> InputState::m_lastInput;

// 初期化
void InputState::Init()
{
	// ボタンの入力タイプをどのボタンにするかの設定
	m_inputMapTable[InputType::DECISION] = { {InputCategory::PAD, XINPUT_BUTTON_A },			// Aボタン	
											 {InputCategory::KEYBORD, KEY_INPUT_RETURN} };
	m_inputMapTable[InputType::BACK] = { {InputCategory::PAD, XINPUT_BUTTON_B} };				// Bボタン
	m_inputMapTable[InputType::PAUSE] = { {InputCategory::PAD, XINPUT_BUTTON_START } };			// スタートボタン
	m_inputMapTable[InputType::SLOW] = { {InputCategory::PAD, XINPUT_BUTTON_B } };				// スロー
	m_inputMapTable[InputType::BOOST] = { {InputCategory::PAD, XINPUT_BUTTON_A } };				// ブースト
	m_inputMapTable[InputType::SHIELD] = { {InputCategory::PAD, XINPUT_BUTTON_RIGHT_SHOULDER} }; // シールド
	m_inputMapTable[InputType::UP] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_UP } };			// ↑
	m_inputMapTable[InputType::DOWN] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_DOWN } };		// ↓
	m_inputMapTable[InputType::RIGHT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_RIGHT } };		// →
	m_inputMapTable[InputType::LEFT] = { {InputCategory::PAD, XINPUT_BUTTON_DPAD_LEFT } };		// ←

#ifdef _DEBUG
	m_inputMapTable[InputType::NORMAL_ENEMY_DEBUG] = { {InputCategory::KEYBORD, KEY_INPUT_E} };	// デバッグ用　雑魚敵を自由に動かせる
	m_inputMapTable[InputType::BOSS_DETH_DEBUG] = { {InputCategory::KEYBORD, KEY_INPUT_R} };		// デバッグ用　ボスの死亡
#endif

	// 設定したボタンの数によって配列の数を変更
	m_currentInput.resize(static_cast<int>(InputType::TOTAL_VALUE));
	m_lastInput.resize(static_cast<int>(InputType::TOTAL_VALUE));
}

void InputState::Update()
{
	// 直前の入力情報を記憶しておく
	m_lastInput = m_currentInput;

	// パッドの情報の取得
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// キー情報の取得
	char keystate[256];
	GetHitKeyStateAll(keystate);

	// マウス情報の取得
	int mouseState = GetMouseInput();

	// マップの全情報をループ
	for (const auto& keymap : m_inputMapTable)
	{
		// 入力情報配列をループ
		for (const auto& input : keymap.second)
		{
			// 入力情報からどのボタンが入力されているか確認
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
			// 3つの入力のうちどれかがtrueだったら入力されてるからbreak
			if (m_currentInput[static_cast<int>(keymap.first)])
			{
				break;
			}
		}
	}
}

bool InputState::IsTriggered(InputType type)
{
	return IsPressed(type) && !m_lastInput[static_cast<int>(type)];
}

bool InputState::IsPressed(InputType type)
{
	return m_currentInput[static_cast<int>(type)];
}

bool InputState::IsPadTrigger(PadLR type)
{
	// パッドの情報の取得
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// トリガーの左か右か
	if (type == PadLR::LEFT)
	{
		// トリガーが半分以上押されていたらtrue
		return 	padState.LeftTrigger > padState.LeftTrigger / 2;
	}
	else
	{
		// トリガーが半分以上押されていたらtrue
		return 	padState.RightTrigger > padState.RightTrigger / 2;
	}
}

int InputState::IsPadStick(PadLR stic, PadStickInputType type)
{
	// パッドの情報の取得
	XINPUT_STATE  padState;
	GetJoypadXInputState(DX_INPUT_PAD1, &padState);

	// 右スティックか左スティックか
	float x, y;
	if (stic == PadLR::LEFT)
	{
		// padStateから取得した値を0.0~10.0に変換
		x = static_cast<float>(padState.ThumbLX / 32767.0f * 10);
		y = static_cast<float>(padState.ThumbLY / 32767.0f * 10);
	}
	else
	{
		// padStateから取得した値を0.0~10.0に変換
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

	// 入力なし
	return 0;
}