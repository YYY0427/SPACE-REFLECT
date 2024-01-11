#pragma once
#include <functional>
#include <map>
#include <vector>

/// <summary>
/// ステートマシンクラス
/// </summary>
/// <typeparam name="TState">ステート</typeparam>
template <class TState> class StateMachine
{
public:
	typedef std::function<void(void)> Delegate;

private:
	class StateDelegateSet
	{
	public:
		TState state;
		Delegate enter, update, exit;

		// 変数初期化警告が出るので、デフォルトコンストラクタで初期化
		StateDelegateSet() : state(TState()) {}
	};

	TState currentState = {};
	std::map<TState, StateDelegateSet> stateFuncMap;
	std::vector<Delegate> allExitFuncTable_;
	bool isInitialized = false;

public:
	// ステートの追加
	void AddState(TState state, Delegate enter, Delegate update, Delegate exit)
	{
		// FIXME: 重複考慮
		StateDelegateSet set{};
		set.state = state;
		set.enter = enter;
		set.update = update;
		set.exit = exit;
		stateFuncMap.emplace(state, set);
	}

	/// <summary>
	/// ステートの設定
	/// </summary>
	/// <param name="state">設定したいステート</param>
	void SetState(TState state)
	{
		// 初回はenterだけ呼ぶ
		if (isInitialized == false)
		{
			isInitialized = true;
			currentState = state;

			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
		// 直前のstateのexitを呼んでステートを更新
		else if (currentState != state)
		{
			for (auto& func : allExitFuncTable_)
			{
				func();
			}

			if (stateFuncMap[currentState].exit != nullptr)
			{
				stateFuncMap[currentState].exit();
			}

			currentState = state;

			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
	}

	// 更新
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	// 全てのexitで呼びたい関数の設定
	void SetAllExitFunction(Delegate exit)
	{
		allExitFuncTable_.push_back(exit);
	}

	// ステートの取得
	TState GetCurrentState() const { return currentState; }
};