#pragma once

enum class TransitDirection
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

/// <summary>
/// シーン遷移演出の基底クラス
/// </summary>
class TransitorBase
{
public:
	// コンストラクタ
	TransitorBase(int interval = 60);

	// デストラクタ
	virtual ~TransitorBase();

	// 演出の更新と描画
	virtual void Update() = 0;
	virtual void Draw() = 0;

	// 演出を開始
	void Start();

	// 演出が終了したかどうか
	virtual bool IsEnd() const;

	// セッター
	void SetFrame(int frame);
	void SetOldScene();

protected:
	// 切り替えにかかるフレーム数
	const int m_interval;

	// 開始から現在までのフレーム数
	int m_frame;

	// シーン遷移前の画面
	int m_oldScene;

	// シーン遷移後の画面
	int m_nextScene;

	// 前のシーンを使うかどうか
	bool m_isOldScene;
};