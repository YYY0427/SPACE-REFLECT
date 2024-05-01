#include "Model.h"
#include <cassert>

namespace
{
	// 当たり判定として使用するフレームの名前
	const std::string collision_frame_name = "Collision";
}

// ファイル名を指定してロードを行う
Model::Model(const std::string& fileName) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	// モデルのロード
	m_modelHandle = MV1LoadModel(fileName.c_str());

	// モデルのロードに失敗したしたら止める
	assert(m_modelHandle != -1);

	// アニメーション情報の初期化
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);

	// モデルのマテリアルのディフューズカラーを保存
	for (int i = 0; i < GetMaterialNum(); i++)
	{
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);
		m_matarialColorTable.push_back(color);
	}
}

// 指定されたハンドルのモデルをコピーして生成する
Model::Model(const int modelHandle) :
	m_isUseCollision(false),
	m_isUpdateColision(false),
	m_colFrameIndex(-1),
	m_animChangeFrame(0),
	m_animChangeFrameTotal(0)
{
	// モデルのコピー
	m_modelHandle = MV1DuplicateModel(modelHandle);

	// モデルのコピーに失敗したら止める
	assert(m_modelHandle != -1);

	// アニメーション情報の初期化
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);

	// モデルのマテリアルのディフューズカラーを保存
	for (int i = 0; i < GetMaterialNum(); i++)
	{
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);
		m_matarialColorTable.push_back(color);
	}
}

// デストラクタ
Model::~Model()
{
	// 当たり判定情報を使用していたら削除
	if (m_isUpdateColision)
	{
		MV1TerminateCollInfo(m_modelHandle, -1);
		m_isUseCollision = false;
	}

	// モデルの削除
	MV1DeleteModel(m_modelHandle);
}

// 当たり判定の設定
void Model::SetUseCollision(const bool isUse, const bool isNeedUpdate)
{
	// 当たり判定を使わないのに当たり判定の更新を毎フレームおこないたい場合をはじく
	assert(isUse || !isNeedUpdate);

	// 当たり判定使用情報に変化があった場合のみ更新する
	if (m_isUpdateColision != isUse)
	{
		if (isUse)
		{
			// 使わない→使う
			// 当たり判定用フレームを検索
			m_colFrameIndex = MV1SearchFrame(m_modelHandle, collision_frame_name.c_str());

			// 見つからなかった or エラー
			if (m_colFrameIndex < 0)	
			{
				m_colFrameIndex = -1;
			}

			// 当たり判定情報を構築する
			MV1SetupCollInfo(m_modelHandle, m_colFrameIndex, 8, 8, 8);
		}
		else
		{
			// 使う→使わない
			MV1TerminateCollInfo(m_modelHandle, -1);
		}
	}

	// 保存
	m_isUseCollision = isUse;
	m_isUpdateColision = isNeedUpdate;
}

// 更新
void Model::Update()
{
	UpdateAnim(m_animPrev);
	UpdateAnim(m_animNext);

	// 指定フレームかけてアニメーションを変更する
	m_animChangeFrame++;
	if (m_animChangeFrame > m_animChangeFrameTotal)
	{
		m_animChangeFrame = m_animChangeFrameTotal;
	}

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();

	// 当たり判定データの更新
	if (m_isUseCollision && m_isUpdateColision)
	{
		MV1RefreshCollInfo(m_modelHandle, m_colFrameIndex);
	}
}

//  描画
void Model::Draw()
{
	int result = MV1DrawModel(m_modelHandle);
	assert(result != -1);
}

// モデルの表示位置の設定
void Model::SetPos(const Vector3& pos)
{
	int result = MV1SetPosition(m_modelHandle, pos.ToDxLibVector3());
	assert(result != -1);
}

// モデルの回転状態の設定
void Model::SetRot(const Vector3& rot)
{
	int result = MV1SetRotationXYZ(m_modelHandle, rot.ToDxLibVector3());
	assert(result != -1);
}

// モデルの回転行列の設定
void Model::SetRotMtx(const Matrix& mtx)
{
	int result = MV1SetRotationMatrix(m_modelHandle, mtx.ToDxLibMatrix());
	assert(result != -1);
}

// モデルの拡大率の設定
void Model::SetScale(const Vector3& scale)
{
	int result = MV1SetScale(m_modelHandle, scale.ToDxLibVector3());
	assert(result != -1);
}

// モデルの不透明度の設定
void Model::SetOpacity(const float opacity)
{
	int result = MV1SetOpacityRate(m_modelHandle, opacity);
	assert(result != -1);
}

// モデルの指定のマテリアルの描画ブレンドモードを設定
void Model::SetMaterialDrawBlendMode(const int materialIndex, const int blendMode)
{
	int result = MV1SetMaterialDrawBlendMode(m_modelHandle, materialIndex, blendMode);
	assert(result != -1);
}

// モデルの全てのマテリアルの描画モードの設定
void Model::SetAllMaterialDrawBlendMode(const int blendMode)
{
	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	// マテリアルの数の分、描画モードの設定
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDrawBlendMode(i, blendMode);
	}
}

// 指定のマテリアルの描画ブレンドパラメータの設定
void Model::SetMaterialDrawBlendParam(const int materialIndex, const int blendParam)
{
	int result = MV1SetMaterialDrawBlendParam(m_modelHandle, materialIndex, blendParam);
	assert(result != -1);
}

// 全てのマテリアルの描画ブレンドパラメータの設定
void Model::SetAllMaterialDrawBlendParam(const int blendParam)
{
	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	// マテリアルの数の分、描画ブレンドパラメータの設定
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDrawBlendParam(i, blendParam);
	}
}

// 指定のマテリアルのディフューズカラーの設定
void Model::SetMaterialDifColor(const int materialIndex, const COLOR_F color)
{
	int result = MV1SetMaterialDifColor(m_modelHandle, materialIndex, color);
	assert(result != -1);
}

// 全てのマテリアルのディフューズカラーの設定
void Model::SetAllMaterialDifColor(const COLOR_F color)
{
	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	// マテリアルの数の分、描画ブレンドパラメータの設定
	for (int i = 0; i < materialNum; i++)
	{
		SetMaterialDifColor(i, color);
	}
}

// 全てのマテリアルのディフューズカラーを反転
void Model::InversAllMaterialDifColor()
{
	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	for (int i = 0; i < materialNum; i++)
	{
		// マテリアルの色を取得
		COLOR_F color = MV1GetMaterialDifColor(m_modelHandle, i);

		// 色を反転
		color = { 1.0f - color.r, 1.0f - color.g, 1.0f - color.b, 1.0f };

		// マテリアルに色を設定
		SetMaterialDifColor(i, color);
	}
}

// 全てのマテリアルのディフューズカラーを元に戻す
void Model::RestoreAllMaterialDifColor()
{
	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	// マテリアルの数の分、ディフューズカラーを元に戻す
	for (int i = 0; i < materialNum; i++)
	{
		COLOR_F color = m_matarialColorTable[i];
		SetMaterialDifColor(i, color);
	}
}

// 全てのマテリアルのアウトラインを設定する
void Model::SetAllMaterialOutLine(const COLOR_F color, const float size)
{

	/*int result = MV1SetMaterialOutLineWidthAll(m_modelHandle, size);
	assert(result != -1);	

	result = MV1SetMaterialOutLineColorAll(m_modelHandle, color);
	assert(result != -1);*/


	// マテリアルの数の取得
	int materialNum = GetMaterialNum();

	// マテリアルの数の分、アウトラインを設定する
	for (int i = 0; i < materialNum; i++)
	{
		// マテリアルの元々のアウトラインの太さを取得
		float initWidth = MV1GetMaterialOutLineDotWidth(m_modelHandle, i);

		// アウトラインの太さを元々の太さ + 指定の太さに設定
		int result = MV1SetMaterialOutLineDotWidth(m_modelHandle, i, size);
		assert(result != -1);

		// アウトラインの色を設定
		result = MV1SetMaterialOutLineColor(m_modelHandle, i, color);
		assert(result != -1);
	}
}

// アニメーションを設定する(ぱっと切り替える)
void Model::SetAnimation(const int animNo, const bool isLoop, const bool isForceChange, const float animPlaySpeed)
{
	if (!isForceChange)
	{
		// すでに再生されているアニメは再生しない
		if (m_animNext.animNo == animNo)	return;
	}

	// 以前のアニメーションが残っていれば終了
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		InitAnimData(m_animPrev);
	}
	if (m_animNext.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animNext.attachNo);
		InitAnimData(m_animNext);
	}

	// 新しくアニメーションを設定
	m_animNext.animNo = animNo;
	m_animNext.playSpeed = animPlaySpeed;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	m_animChangeFrameTotal = 1;
	m_animChangeFrame = 1;
}

// アニメーションを変化させる(数フレームかけて切り替える)
void Model::ChangeAnimation(const int animNo, const bool isLoop, const bool isForceChange, const int changeFrame, const float animPlaySpeed)
{
	if (!isForceChange)
	{
		// すでに再生されているアニメは再生しない
		if (m_animNext.animNo == animNo)	return;
	}

	// 以前のアニメーションが残っていれば終了
	if (m_animPrev.attachNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_animPrev.attachNo);
		InitAnimData(m_animPrev);
	}

	// 現在再生しているアニメーションを古いアニメとする
	m_animPrev = m_animNext;

	// 新しくアニメーションを設定
	m_animNext.animNo = animNo;
	m_animNext.playSpeed = animPlaySpeed;
	m_animNext.attachNo = MV1AttachAnim(m_modelHandle, animNo, -1, false);
	m_animNext.totalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animNext.attachNo);
	m_animNext.isLoop = isLoop;

	// 変更にかけるフレーム数を覚えておく
	m_animChangeFrameTotal = changeFrame;
	m_animChangeFrame = 0;

	// アニメーションのブレンド率を設定する
	UpdateAnimBlendRate();
}

// 現在のアニメーションが終了しているかどうかを取得する
bool Model::IsAnimEnd()
{
	// Looppアニメの場合はfalseを返す
	if (m_animNext.isLoop)
	{
		return false;
	}

	// アタッチしているアニメーションの再生時間を取得する
	float time = MV1GetAttachAnimTime(m_modelHandle, m_animNext.attachNo);

	// アニメーションを再生した時間がアニメーションの総時間を超えていたら
	// アニメーションが終了しているのでtrueを返す
	if (time >= m_animNext.totalTime)
	{
		return true;
	}
	return false;
}

void Model::StopAnim()
{
	InitAnimData(m_animPrev);
	InitAnimData(m_animNext);
}

// モデルのハンドルの取得
int Model::GetModelHandle() const
{
	return m_modelHandle;		
}

// 当たり判定に使用するフレームインデックスを取得する
int Model::GetColFrameIndex() const
{
	return m_colFrameIndex;
}

int Model::GetMaterialNum() const
{
	int result = MV1GetMaterialNum(m_modelHandle);
	return result;
}

// アニメーションデータの初期化
void Model::InitAnimData(AnimData& anim)
{
	anim.animNo = -1;
	anim.attachNo = -1;
	anim.totalTime = 0.0f;
	anim.playSpeed = 1.0f;
	anim.isLoop = false;
}

// アニメーションの更新
void Model::UpdateAnim(const AnimData& anim)
{
	// アニメーションが設定されていない場合は何もしない
	if (anim.attachNo == -1) return;

	// アニメーションの更新
	float time = MV1GetAttachAnimTime(m_modelHandle, anim.attachNo);
	time += anim.playSpeed;

	if (time > anim.totalTime)
	{
		if (anim.isLoop)
		{
			// アニメーションのループ
			time -= anim.totalTime;
		}
		else
		{
			// 最後の状態で止まる
			time = anim.totalTime;
		}
	}
	// アタッチしているアニメーションの再生時間を設定する
	MV1SetAttachAnimTime(m_modelHandle, anim.attachNo, time);
}

// 現在のアニメーション切り替わり情報からアニメーションのブレンド率を設定する
void Model::UpdateAnimBlendRate()
{
	// アニメーション変化のフレーム数に応じたブレンド率を設定する
	float rate = static_cast<float> (m_animChangeFrame) / static_cast<float>(m_animChangeFrameTotal);
	if (rate > 1.0f)
	{
		rate = 1.0f;
	}
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animPrev.attachNo, 1.0f - rate);
	MV1SetAttachAnimBlendRate(m_modelHandle, m_animNext.attachNo, rate);
}