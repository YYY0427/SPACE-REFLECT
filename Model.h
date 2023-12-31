#pragma once
#include <DxLib.h>
#include <string>
#include <vector>

// TODO : 当たり判定関係の関数の追加

/// <summary>
/// モデルの管理クラス
/// </summary>
class Model
{
public:
	/// <summary>
	/// コンストラクタ
	/// ファイル名を指定してロードを行う
	/// </summary>
	/// <param name="fileName">モデルハンドルのファイル名</param>
	Model(std::string fileName);		

	/// <summary>
	/// コンストラクタ
	/// 指定されたハンドルのモデルをコピーして生成する
	/// </summary>
	/// <param name="orgModel">モデルハンドル</param>
	Model(int modelHandle);		

	// デストラクタ
	~Model();

	/// <summary>
	/// 当たり判定設定
	/// </summary>
	/// <param name="isUse">当たり判定をしようするか</param>
	/// <param name="isNeedUpdate">更新が必要かどうか</param>
	void SetUseCollision(bool isUse, bool isNeedUpdate = true);

	// 更新
	void Update();

	// 描画
	void Draw();

	/// <summary>
	/// アニメーションを設定する(ぱっと切り替える)
	/// </summary>
	/// <param name="animNo">変更先アニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	///	<param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
	void SetAnimation(int animNo, bool isLoop, bool isForceChange, float animPlaySpeed = 1.0f);

	/// <summary>
	/// アニメーションを変化させる(数フレームかけて切り替える)
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
	/// <param name="changeFrame">何フレームかけてアニメーションを変更させるか</param>
	void ChangeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame, float animPlaySpeed = 1.0f);

	/// <summary>
	/// 現在のアニメーションが終了しているかどうかを取得する
	/// Loopアニメの場合は取得できない(falseを返す)
	/// </summary>
	/// <returns>true : 終了、false : 再生中</returns>
	bool IsAnimEnd();

	// アニメーションを止める
	void StopAnim();

	// ゲッター
	const int& GetModelHandle() const;		// モデルのハンドルの取得
	int GetColFrameIndex() const;	// 当たり判定に使用するフレームインデックスを取得する
	int GetMaterialNum() const;		// モデルのマテリアルの数の取得

	// モデル設定
	void SetPos(VECTOR pos);			// 表示位置の設定
	void SetRot(VECTOR rot);			// 回転状態の設定
	void SetRotMtx(MATRIX mtx);			// 回転行列の設定
	void SetScale(VECTOR scale);		// 拡大率の設定
	void SetOpacity(float opacity);		// 不透明度の設定(0.0~1.0)

	// 描画モード設定
	void SetMaterialDrawBlendMode(int materialIndex, int blendMode);	// 指定のマテリアルの描画モードの設定
	void SetAllMaterialDrawBlendMode(int blendMode);					// 全てのマテリアルの描画モードの設定
	void SetMaterialDrawBlendParam(int materialIndex, int blendParam);	// 指定のマテリアルの描画ブレンドパラメータの設定
	void SetAllMaterialDrawBlendParam(int blendParam);					// 全てのマテリアルの描画ブレンドパラメータの設定

	// ディフューズカラー設定
	void SetMaterialDifColor(int materialIndex, COLOR_F color);	// 指定のマテリアルのディフューズカラーの設定
	void SetAllMaterialDifColor(COLOR_F color);					// 全てのマテリアルのディフューズカラーの設定
	void InversAllMaterialDifColor();							// 全てのマテリアルのディフューズカラーを反転
	void RestoreAllMaterialDifColor();							// 全てのマテリアルのディフューズカラーを元に戻す


private:
	// アニメーション情報
	struct AnimData
	{
		int animNo;			// アニメーション番号
		int attachNo;		// アタッチ番号
		float totalTime;	// アニメーションの総再生時間
		float playSpeed;	// アニメーションの再生速度
		bool isLoop;		// アニメーションがループするか
	};

private:
	/// <summary>
	/// アニメーションデータの初期化
	/// </summary>
	/// <param name="anim">アニメーションのアタッチ番号</param>
	void InitAnimData(AnimData& anim);

	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="anim">アニメーションのアタッチ番号</param>
	void UpdateAnim(AnimData anim);

	// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
	void UpdateAnimBlendRate();

private:
	// モデルのハンドル
	int m_modelHandle;

	// 当たり判定情報を使用する
	bool m_isUseCollision;

	// 当たり判定情報を毎フレーム更新する
	bool m_isUpdateColision;

	// 当たり判定として使用するフレームのインデックス
	int m_colFrameIndex;

	// アニメーションのアタッチ番号
	AnimData m_animPrev;		// 変更前アニメーション情報
	AnimData m_animNext;		// 変更後アニメーション情報

	// アニメーションの切り替え情報
	int m_animChangeFrame;			// 現在の切り替えフレーム数
	int m_animChangeFrameTotal;		// 切り替えにかける総フレーム数

	// 全ての元のマテリアルのディフューズカラーを保存しているテーブル
	std::vector<COLOR_F> m_matarialColorTable;
};