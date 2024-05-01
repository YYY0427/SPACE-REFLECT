#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include "Math/Vector3.h"
#include "Math/Matrix.h"

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
	Model(const std::string& fileName);		

	/// <summary>
	/// コンストラクタ
	/// 指定されたハンドルのモデルをコピーして生成する
	/// </summary>
	/// <param name="orgModel">モデルハンドル</param>
	Model(const int modelHandle);		

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model();

	/// <summary>
	/// 当たり判定設定
	/// </summary>
	/// <param name="isUse">当たり判定をしようするか</param>
	/// <param name="isNeedUpdate">更新が必要かどうか</param>
	void SetUseCollision(const bool isUse, const bool isNeedUpdate = true);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// アニメーションを設定する(ぱっと切り替える)
	/// </summary>
	/// <param name="animNo">変更先アニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	///	<param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
	void SetAnimation(const int animNo, const bool isLoop, const bool isForceChange, const float animPlaySpeed = 1.0f);

	/// <summary>
	/// アニメーションを変化させる(数フレームかけて切り替える)
	/// </summary>
	/// <param name="animNo">アニメーション番号</param>
	/// <param name="isLoop">アニメーションをループさせるか</param>
	/// <param name="isForceChange">すでに指定されたアニメが再生されている場合も変更するか</param>
	/// <param name="changeFrame">何フレームかけてアニメーションを変更させるか</param>
	void ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame, const float animPlaySpeed = 1.0f);

	/// <summary>
	/// 現在のアニメーションが終了しているかどうかを取得する
	/// Loopアニメの場合は取得できない(falseを返す)
	/// </summary>
	/// <returns>true : 終了、false : 再生中</returns>
	bool IsAnimEnd();

	/// <summary>
	/// アニメーションを止める 
	/// </summary>
	void StopAnim();


	//// ゲッター ////

	/// <summary>
	/// モデルハンドルの取得
	/// </summary>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle() const;		

	/// <summary>
	/// 当たり判定に使用するフレームインデックスを取得
	/// </summary>
	/// <returns>フレームインデックス</returns>
	int GetColFrameIndex() const;	

	/// <summary>
	/// モデルのマテリアルの数の取得
	/// </summary>
	/// <returns>マテリアルの数</returns>
	int GetMaterialNum() const;		


	//// セッター ////

	/// <summary>
	/// モデルの位置を設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetPos(const Vector3& pos);	

	/// <summary>
	/// モデルの回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRot(const Vector3& rot);	

	/// <summary>
	/// モデルの拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetRotMtx(const Matrix& mtx);		

	/// <summary>
	/// モデルの拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale);	

	/// <summary>
	/// モデルの不透明度の設定(0.0~1.0)
	/// </summary>
	/// <param name="opacity">不透明度</param>
	void SetOpacity(const float opacity);	


	//// 描画モード設定 ////

	/// <summary>
	/// 指定のマテリアルの描画モードの設定
	/// </summary>
	/// <param name="materialIndex">マテリアルのインデックス</param>
	/// <param name="blendMode">ブレンドモード</param>
	void SetMaterialDrawBlendMode(const int materialIndex, const int blendMode);

	/// <summary>
	/// 全てのマテリアルの描画モードの設定
	/// </summary>
	/// <param name="blendMode">ブレンドモード</param>
	void SetAllMaterialDrawBlendMode(const int blendMode);					

	/// <summary>
	/// 指定のマテリアルの描画ブレンドパラメータの設定
	/// </summary>
	/// <param name="materialIndex">マテリアルのインデックス</param>
	/// <param name="blendParam">ブレンドのパラメーター</param>
	void SetMaterialDrawBlendParam(const int materialIndex, const int blendParam);	

	/// <summary>
	/// 全てのマテリアルの描画ブレンドパラメータの設定
	/// </summary>
	/// <param name="blendParam">ブレンドのパラメーター</param>
	void SetAllMaterialDrawBlendParam(const int blendParam);				


	//// ディフューズカラー設定 ////

	/// <summary>
	/// 指定のマテリアルのディフューズカラーの設定
	/// </summary>
	/// <param name="materialIndex">マテリアルのインデックス</param>
	/// <param name="color">ディフューズカラー</param>
	void SetMaterialDifColor(const int materialIndex, const COLOR_F color);	

	/// <summary>
	/// 全てのマテリアルのディフューズカラーの設定
	/// </summary>
	/// <param name="color">ディフューズカラー</param>
	void SetAllMaterialDifColor(const COLOR_F color);					

	/// <summary>
	/// 全てのマテリアルのディフューズカラーを反転
	/// </summary>
	void InversAllMaterialDifColor();				
	
	/// <summary>
	/// 全てのマテリアルのディフューズカラーを元に戻す
	/// </summary>
	void RestoreAllMaterialDifColor();					 

	/// <summary>
	/// 全てのマテリアルのアウトラインを設定する 
	/// </summary>
	/// <param name="color">アウトラインの色</param>
	/// <param name="size">アウトラインの太さ</param>
	void SetAllMaterialOutLine(const COLOR_F color, const float size);	

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
	void UpdateAnim(const AnimData& anim);

	/// <summary>
	/// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する 
	/// </summary>
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