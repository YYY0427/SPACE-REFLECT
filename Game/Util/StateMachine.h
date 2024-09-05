#pragma once
#include <functional>
#include <map>
#include <vector>

/// <summary>
/// �X�e�[�g�}�V���N���X
/// </summary>
/// <typeparam name="TState">�X�e�[�g</typeparam>
template <class TState> class StateMachine
{
public:
	typedef std::function<void(void)> Delegate;

private:
	/// <summary>
	/// �X�e�[�g���Ƃ̃f���Q�[�g��ێ�����N���X
	/// </summary>
	class StateDelegateSet
	{
	public:
		// �X�e�[�g
		TState state;

		// �e�X�e�[�g�̃f���Q�[�g
		Delegate enter, update, exit;

		// �ϐ��������x�����o��̂ŁA�f�t�H���g�R���X�g���N�^�ŏ�����
		StateDelegateSet() : state(TState()) {}
	};

	// ���݂̃X�e�[�g
	TState currentState = {};

	// �X�e�[�g���Ƃ̃f���Q�[�g��ێ�����}�b�v
	std::map<TState, StateDelegateSet> stateFuncMap;

	// �S�Ă�exit�ŌĂт����֐�
	std::vector<Delegate> allExitFuncTable_;

	// �������t���O
	bool isInitialized = false;

public:
	/// <summary>
	/// �X�e�[�g�̒ǉ�
	/// </summary>
	/// <param name="state">�X�e�[�g</param>
	/// <param name="enter">���鎞�̏���</param>
	/// <param name="update">�X�V���̏���</param>
	/// <param name="exit">�o�鎞�̏���</param>
	void AddState(const TState state, const Delegate enter, const Delegate update, const Delegate exit)
	{
		// FIXME: �d���l��
		StateDelegateSet set{};
		set.state = state;
		set.enter = enter;
		set.update = update;
		set.exit = exit;
		stateFuncMap.emplace(state, set);
	}

	/// <summary>
	/// �X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="state">�ݒ肵�����X�e�[�g</param>
	void SetState(const TState state)
	{
		// �����enter�����Ă�
		if (isInitialized == false)
		{
			// �������t���O�𗧂Ă�
			isInitialized = true;

			// �X�e�[�g��ݒ�
			currentState = state;

			// enter�Ɋ֐�������ΌĂ�
			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
		// ���O��state��exit���Ă�ŃX�e�[�g���X�V
		else if (currentState != state)
		{
			// �S�Ă�exit�֐����Ă�
			for (auto& func : allExitFuncTable_)
			{
				func();
			}

			// exit�Ɋ֐�������ΌĂ�
			if (stateFuncMap[currentState].exit != nullptr)
			{
				stateFuncMap[currentState].exit();
			}

			// �X�e�[�g��ݒ�
			currentState = state;

			// enter�Ɋ֐�������ΌĂ�
			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
	}

	/// <summary>
	/// �X�e�[�g�̍X�V
	/// </summary>
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	/// <summary>
	/// �S�Ă�exit�ŌĂт����֐��̐ݒ� 
	/// </summary>
	/// <param name="exit">�ݒ肵�����֐�</param>
	void SetAllExitFunction(const Delegate exit)
	{
		allExitFuncTable_.push_back(exit);
	}

	/// <summary>
	/// �X�e�[�g�̎擾 
	/// </summary>
	/// <returns>���݂̃X�e�[�g</returns>
	TState GetCurrentState() const { return currentState; }
};