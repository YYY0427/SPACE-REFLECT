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
	/// <summary>
	/// ステートごとのデリゲートを保持するクラス
	/// </summary>
	class StateDelegateSet
	{
	public:
		// ステート
		TState state;

		// 各ステートのデリゲート
		Delegate enter, update, exit;

		// 変数初期化警告が出るので、デフォルトコンストラクタで初期化
		StateDelegateSet() : state(TState()) {}
	};

	// 現在のステート
	TState currentState = {};

	// ステートごとのデリゲートを保持するマップ
	std::map<TState, StateDelegateSet> stateFuncMap;

	// 全てのexitで呼びたい関数
	std::vector<Delegate> allExitFuncTable_;

	// 初期化フラグ
	bool isInitialized = false;

public:
	/// <summary>
	/// ステートの追加
	/// </summary>
	/// <param name="state">ステート</param>
	/// <param name="enter">入る時の処理</param>
	/// <param name="update">更新時の処理</param>
	/// <param name="exit">出る時の処理</param>
	void AddState(const TState state, const Delegate enter, const Delegate update, const Delegate exit)
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
	void SetState(const TState state)
	{
		// 初回はenterだけ呼ぶ
		if (isInitialized == false)
		{
			// 初期化フラグを立てる
			isInitialized = true;

			// ステートを設定
			currentState = state;

			// enterに関数があれば呼ぶ
			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
		// 直前のstateのexitを呼んでステートを更新
		else if (currentState != state)
		{
			// 全てのexit関数を呼ぶ
			for (auto& func : allExitFuncTable_)
			{
				func();
			}

			// exitに関数があれば呼ぶ
			if (stateFuncMap[currentState].exit != nullptr)
			{
				stateFuncMap[currentState].exit();
			}

			// ステートを設定
			currentState = state;

			// enterに関数があれば呼ぶ
			if (stateFuncMap[currentState].enter != nullptr)
			{
				stateFuncMap[currentState].enter();
			}
		}
	}

	/// <summary>
	/// ステートの更新
	/// </summary>
	void Update()
	{
		stateFuncMap[currentState].update();
	}

	/// <summary>
	/// 全てのexitで呼びたい関数の設定 
	/// </summary>
	/// <param name="exit">設定したい関数</param>
	void SetAllExitFunction(const Delegate exit)
	{
		allExitFuncTable_.push_back(exit);
	}

	/// <summary>
	/// ステートの取得 
	/// </summary>
	/// <returns>現在のステート</returns>
	TState GetCurrentState() const { return currentState; }
};