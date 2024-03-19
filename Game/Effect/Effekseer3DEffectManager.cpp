#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "../Util/DrawFunctions.h"
#include "../Util/FileUtil.h"
#include <cassert>
#include <DxLib.h>

namespace
{
	// エフェクトのファイルパス
	const std::string effect_list_file_path = "Data/Csv/Effect.csv";

	// Effekseer用の画像ファイルパス
	const std::string img_file_path = "Data/Effect/background.png";

	// エフェクトの最大パーティクル数
	constexpr int max_particle = 8000;
}

// コンストラクタ
Effekseer3DEffectManager::Effekseer3DEffectManager():
	m_imgHandle(-1)
{
}

// デストラクタ
Effekseer3DEffectManager::~Effekseer3DEffectManager()
{
}

// Effekseer3DEffectManagerの唯一のインスタンスを返す
Effekseer3DEffectManager& Effekseer3DEffectManager::GetInstance()
{
	// 唯一の実態参照を返す
	static Effekseer3DEffectManager instance;
	return instance;
}

// Effekseerの初期化とエフェクトのロード
void Effekseer3DEffectManager::Init()
{
	// Effekseerを初期化する
	// 引数には画面に表示する最大パーティクル数を設定する
	if (Effkseer_Init(max_particle) == -1)
	{
		// 初期化の失敗
		assert(!"Effekseerの初期化に失敗");
	}

	// 画像のロード
	m_imgHandle = my::MyLoadGraph(img_file_path.c_str());

	// エフェクトのロード
	LoadCsvEffectFile(effect_list_file_path);
}

// 更新
void Effekseer3DEffectManager::Update()
{
	for (auto& effect : m_effectDataTable)
	{
		// 再生タイプがループしか通らない
		if (effect.type != PlayType::LOOP) continue;

		// 再生中じゃない場合しか通らない
		if (IsPlayingEffect(*effect.playingEffectHandle)) continue;

		// エフェクトの再再生
		PlayEffectLoop(*effect.playingEffectHandle, effect.effectId);

		// エフェクトのパラメーターの設定
		SetEffectAllParam(*effect.playingEffectHandle, effect.pos, effect.scale, effect.speed, effect.rot);
	}
	for (auto& effect : m_followEffectDataTable)
	{
		// 再生タイプがループか
		bool isLoop = effect.type == PlayType::LOOP_AND_FOLLOW;

		// 再生中か
		bool isPlay = IsPlayingEffect(*effect.playingEffectHandle);

		// 再生タイプがループ、再生中ではない場合
		if (isLoop && !isPlay)
		{
			// エフェクトの再再生
			PlayEffectLoop(*effect.playingEffectHandle, effect.effectId);
		}

		// 再生中の場合エフェクトのパラメーターの更新
		if (isPlay)
		{
			SetEffectAllParam(*effect.playingEffectHandle, *effect.pos, effect.scale, effect.speed, effect.rot);
		}
	}

	// 再生が終了したエフェクトがあった場合削除
	m_effectDataTable.remove_if([this](EffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });
	m_followEffectDataTable.remove_if([this](FollowEffectData data) { return !IsPlayingEffect(*data.playingEffectHandle); });

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();
}

// 描画
void Effekseer3DEffectManager::Draw()
{
	// 何でもいいので画像を描画する
	// こうして描画した後でないと、Effekseerは描画できない
	DrawGraph(0, 0, m_imgHandle, true);

	// Effekseerにより再生中のエフェクトを描画する 
	DrawEffekseer3D();

#if false
	// エフェクトの描画開始
	DrawEffekseer3D_Begin();

	for(auto& effect : m_effectDataTable)
	{
		// Zバッファを使うかどうか
		SetUseZBuffer3D(effect.isZBuffer);

		// Effekseerにより再生中のエフェクトを描画
		int result = DrawEffekseer3D_Draw(*effect.playingEffectHandle);
		assert(result != -1);

		// Zバッファ設定を元に戻す
		SetUseZBuffer3D(true);
	}
	for(auto& effect : m_followEffectDataTable)
	{
		// Zバッファを使うかどうか
		SetUseZBuffer3D(effect.isZBuffer);

		// Effekseerにより再生中のエフェクトを描画
		int result = DrawEffekseer3D_Draw(*effect.playingEffectHandle);
		assert(result != -1);

		// Zバッファ設定を元に戻す
		SetUseZBuffer3D(true);
	}
	// エフェクトの描画終了
	DrawEffekseer3D_End();
#endif
}

// 終了処理
void Effekseer3DEffectManager::End()
{
	// 画像の削除
	DeleteGraph(m_imgHandle);

	// エフェクトリソースを削除する(Effekseer終了時に破棄されるので削除しなくてもいいが一応)
	for (auto& effectResource : m_effectResourceTable)
	{
		DeleteEffekseerEffect(effectResource.second.effectHandle);
	}

	// 配列の削除
	m_effectResourceTable.clear();
	m_effectDataTable.clear();
	m_followEffectDataTable.clear();

	// Effekseerを終了する
	Effkseer_End();
}

// エフェクトのロード
void Effekseer3DEffectManager::LoadCsvEffectFile(const std::string& filePath)
{
	// ファイルの読み込み
	auto data = FileUtil::LoadCsvFile(filePath);

	for (auto& list : data)
	{
		// 同じファイルパスが登録されている場合はそのハンドルを使う
		for (auto& effect : m_effectResourceTable)
		{
			if (effect.second.effectFilePath == list[static_cast<int>(EffectFileType::FILE_PATH)])
			{
				m_effectResourceTable[list[static_cast<int>(EffectFileType::ID)]] = effect.second;
				continue;
			}
		}

		// エフェクトのファイルパスを保存
		m_effectResourceTable[list[static_cast<int>(EffectFileType::ID)]].effectFilePath = 
			list[static_cast<int>(EffectFileType::FILE_PATH)];

		// エフェクトのロード(失敗したら止める)
		m_effectResourceTable[list[static_cast<int>(EffectFileType::ID)]].effectHandle = 
			LoadEffekseerEffect(list[static_cast<int>(EffectFileType::FILE_PATH)].c_str());
		assert(m_effectResourceTable[list[static_cast<int>(EffectFileType::ID)]].effectHandle != -1 && "エフェクトのロードに失敗したよ");
	}
}

// ループエフェクトの再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, const std::string& id)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

	// エフェクトの再再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);
}

// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(int& playingEffectHandle, const std::string& id, const Vector3& pos, const Vector3& scale, const float speed, const Vector3& rot, const bool isZbuffer)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.type = PlayType::NORMAL;
	effect.isZBuffer = isZbuffer;
	m_effectDataTable.push_back(effect);
}

// 指定のエフェクトのループ再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, const std::string& id, const Vector3& pos, const Vector3& scale, const float speed, const Vector3& rot, const bool isZbuffer)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectId = id;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP;
	effect.isZBuffer = isZbuffer;
	m_effectDataTable.push_back(effect);
}

// 指定のエフェクトの追従再生
void Effekseer3DEffectManager::PlayEffectFollow(int& playingEffectHandle, const std::string& id, Vector3* pos, const Vector3& scale, const float speed, const Vector3& rot, const bool isZbuffer)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectId = id;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::FOLLOW;
	effect.isZBuffer = isZbuffer;
	m_followEffectDataTable.push_back(effect);
}

// 指定のエフェクトの追従とループ再生
void Effekseer3DEffectManager::PlayEffectLoopAndFollow(int& playingEffectHandle, const std::string& id, Vector3* pos, const Vector3& scale, const float speed, const Vector3& rot, const bool isZbuffer)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceTable.find(id) != m_effectResourceTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceTable[id].effectHandle);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectId = id;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP_AND_FOLLOW;
	effect.isZBuffer = isZbuffer;
	m_followEffectDataTable.push_back(effect);
}

// エフェクトの全ての値の変更
void Effekseer3DEffectManager::SetEffectAllParam(const int playingEffectHandle, const Vector3& pos, const Vector3& scale, const float speed, const Vector3& rot)
{
	// 再生速度
	SetEffectSpeed(playingEffectHandle, speed);

	// 拡大率
	SetEffectScale(playingEffectHandle, scale);

	// 回転率
	SetEffectRot(playingEffectHandle, rot);

	// 位置
	SetEffectPos(playingEffectHandle, pos);
}

// エフェクトの拡大率の変更
void Effekseer3DEffectManager::SetEffectScale(const int playingEffectHandle, const Vector3& scale)
{
	// エフェクトの拡大率の設定
	int result = SetScalePlayingEffekseer3DEffect(playingEffectHandle, scale.x, scale.y, scale.z);

	// 失敗したら止める
	assert(result != -1);
	
	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : m_effectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.scale = scale;
		}
	}
	for (auto& effect : m_followEffectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.scale = scale;
		}
	}
}

// エフェクトの再生速度の変更
void Effekseer3DEffectManager::SetEffectSpeed(const int playingEffectHandle, const float speed)
{
	// エフェクトの再生速度の設定
	int result = SetSpeedPlayingEffekseer3DEffect(playingEffectHandle, speed);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : m_effectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.speed = speed;
		}
	}
	for (auto& effect : m_followEffectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.speed = speed;
		}
	}
}

// エフェクトの回転率の変更
void Effekseer3DEffectManager::SetEffectRot(const int playingEffectHandle, const Vector3& rot)
{
	// エフェクトの回転率の設定
	int result = SetRotationPlayingEffekseer3DEffect(playingEffectHandle, rot.x, rot.y, rot.z);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認する
	for (auto& effect : m_effectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.rot = rot;
		}
	}
	for (auto& effect : m_followEffectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.rot = rot;
		}
	}
}

// エフェクトの位置の変更
void Effekseer3DEffectManager::SetEffectPos(const int playingEffectHandle, const Vector3& pos)
{
	// エフェクトの位置の設定
	int result = SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);

	// 失敗したら止める
	assert(result != -1);

	// エフェクトのデータを保存しているテーブルの値を変更する
	// 追従エフェクトは必要ないので変更しない
	for (auto& effect : m_effectDataTable)
	{
		if (*effect.playingEffectHandle == playingEffectHandle)
		{
			effect.pos = pos;
		}
	}
}

// 特定のエフェクトが再生中か
bool Effekseer3DEffectManager::IsPlayingEffect(const int playingEffectHandle)
{
	if (IsEffekseer3DEffectPlaying(playingEffectHandle) == -1)
	{
		return false;
	}
	return true;
}

// 特定のエフェクトの再生をストップ
void Effekseer3DEffectManager::StopEffect(const int playingEffectHandle)
{
	int result = StopEffekseer3DEffect(playingEffectHandle);
	assert(result != -1);
}

// エフェクト全ての再生をストップ
void Effekseer3DEffectManager::StopAllEffect()
{
	for (auto& effect : m_effectDataTable)
	{
		StopEffect(*effect.playingEffectHandle);
	}
	for (auto& effect : m_followEffectDataTable)
	{
		StopEffect(*effect.playingEffectHandle);
	}
}

// 特定のエフェクトの削除
void Effekseer3DEffectManager::DeleteEffect(const int playingEffectHandle)
{
	// エフェクトの再生を止める
	StopEffect(playingEffectHandle);

	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認しハンドルが一致したデータを削除
	m_effectDataTable.remove_if(
		[playingEffectHandle](EffectData data) 
		{ return *data.playingEffectHandle == playingEffectHandle; });
	m_followEffectDataTable.remove_if(
		[playingEffectHandle](FollowEffectData data) 
		{ return *data.playingEffectHandle == playingEffectHandle; });
}

// 全てのエフェクトの削除
void Effekseer3DEffectManager::DeleteAllEffect()
{
	// 全てのエフェクトの再生を止める
	StopAllEffect();

	// 全てのデータの削除
	m_effectDataTable.clear();
	m_followEffectDataTable.clear();
}