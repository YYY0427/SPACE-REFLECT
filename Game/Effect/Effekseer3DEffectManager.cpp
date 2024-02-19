#include "Effekseer3DEffectManager.h"
#include "EffekseerForDXLib.h"
#include "../Util/DrawFunctions.h"
#include <cassert>



namespace
{
	// エフェクトのファイルパス
	const std::string sound_resourse_file_path = "Data/Effect/";

	// エフェクトの拡張子
	const std::string data_extension = ".efk";
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
	// 唯一の実態
	static Effekseer3DEffectManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// Effekseerの初期化とエフェクトのロード
void Effekseer3DEffectManager::Init()
{
	// Effekseerを初期化する
	// 引数には画面に表示する最大パーティクル数を設定する
	if (Effkseer_Init(8000) == -1)
	{
		// 初期化の失敗
		assert(0);
	}

	// 画像のロード
	m_imgHandle = my::MyLoadGraph("Data/Effect/background.png");

	// Effectのロード
	for (auto& filePath : EffectID::effect_file_path)
	{
		LoadEffectFile(filePath.c_str());
	}
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
		PlayEffectLoop(*effect.playingEffectHandle, effect.effectFileName);

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
			PlayEffectLoop(*effect.playingEffectHandle, effect.effectFileName);
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
}

// 終了処理
void Effekseer3DEffectManager::End()
{
	// 画像の削除
	DeleteGraph(m_imgHandle);

	// エフェクトリソースを削除する(Effekseer終了時に破棄されるので削除しなくてもいいが一応)
	for (auto& effectResource : m_effectResourceHandleTable)
	{
		DeleteEffekseerEffect(effectResource.second);
	}

	// 配列の削除
	m_effectResourceHandleTable.clear();
	m_effectDataTable.clear();
	m_followEffectDataTable.clear();

	// Effekseerを終了する
	Effkseer_End();
}

// エフェクトのロード
void Effekseer3DEffectManager::LoadEffectFile(std::string fileName)
{
	// エフェクトのファイルパスの生成
	std::string path = sound_resourse_file_path;
	path += fileName;
	path += data_extension;

	// エフェクトのロード(失敗したら止める)
	int handle = LoadEffekseerEffect(path.c_str());
	assert(handle != -1);	// -1以外じゃなかったら止める

	// ロードしたエフェクトのハンドルの保存
	m_effectResourceHandleTable[fileName] = handle;
}

// ループエフェクトの再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, std::string fileName)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceHandleTable.find(fileName) != m_effectResourceHandleTable.end());

	// エフェクトの再再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandleTable[fileName]);
}

// 指定のエフェクトの再生
void Effekseer3DEffectManager::PlayEffect(int& playingEffectHandle, std::string fileName, Vector3 pos, Vector3 scale, float speed, Vector3 rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceHandleTable.find(fileName) != m_effectResourceHandleTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandleTable[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.type = PlayType::NORMAL;
	m_effectDataTable.push_back(effect);
}

// 指定のエフェクトのループ再生
void Effekseer3DEffectManager::PlayEffectLoop(int& playingEffectHandle, std::string fileName, Vector3 pos, Vector3 scale, float speed, Vector3 rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceHandleTable.find(fileName) != m_effectResourceHandleTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandleTable[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, pos, scale, speed, rot);

	// データを保存
	EffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP;
	m_effectDataTable.push_back(effect);
}

// 指定のエフェクトの追従再生
void Effekseer3DEffectManager::PlayEffectFollow(int& playingEffectHandle, std::string fileName, Vector3* pos, Vector3 scale, float speed, Vector3 rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceHandleTable.find(fileName) != m_effectResourceHandleTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandleTable[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::FOLLOW;
	m_followEffectDataTable.push_back(effect);
}

// 指定のエフェクトの追従とループ再生
void Effekseer3DEffectManager::PlayEffectLoopAndFollow(int& playingEffectHandle, std::string fileName, Vector3* pos, Vector3 scale, float speed, Vector3 rot)
{
	// エフェクトリソースに指定したエフェクトがロードされていない場合止める
	assert(m_effectResourceHandleTable.find(fileName) != m_effectResourceHandleTable.end());

	// エフェクトの再生
	playingEffectHandle = PlayEffekseer3DEffect(m_effectResourceHandleTable[fileName]);

	// エフェクトのパラメーターの設定
	SetEffectAllParam(playingEffectHandle, *pos, scale, speed, rot);

	// データを保存
	FollowEffectData effect{};
	effect.playingEffectHandle = &playingEffectHandle;
	effect.effectFileName = fileName;
	effect.pos = pos;
	effect.scale = scale;
	effect.speed = speed;
	effect.rot = rot;
	effect.type = PlayType::LOOP_AND_FOLLOW;
	m_followEffectDataTable.push_back(effect);
}

// エフェクトの全ての値の変更
void Effekseer3DEffectManager::SetEffectAllParam(int playingEffectHandle, Vector3 pos, Vector3 scale, float speed, Vector3 rot)
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
void Effekseer3DEffectManager::SetEffectScale(int playingEffectHandle, Vector3 scale)
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
void Effekseer3DEffectManager::SetEffectSpeed(int playingEffectHandle, float speed)
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
void Effekseer3DEffectManager::SetEffectRot(int playingEffectHandle, Vector3 rot)
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
void Effekseer3DEffectManager::SetEffectPos(int playingEffectHandle, Vector3 pos)
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
bool Effekseer3DEffectManager::IsPlayingEffect(int effectPlayingHandle)
{
	if (IsEffekseer3DEffectPlaying(effectPlayingHandle) == 0)
	{
		return true;
	}
	return false;
}

// 特定のエフェクトの再生をストップ
void Effekseer3DEffectManager::StopEffect(int effectPlayingHandle)
{
	int result = StopEffekseer3DEffect(effectPlayingHandle);
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
void Effekseer3DEffectManager::DeleteEffect(int effectPlayingHandle)
{
	// エフェクトの再生を止める
	StopEffect(effectPlayingHandle);

	// どっちのテーブルに格納されているか分からないため両方のテーブルを確認しハンドルが一致したデータを削除
	m_effectDataTable.remove_if(
		[effectPlayingHandle](EffectData data) 
		{ return *data.playingEffectHandle == effectPlayingHandle; });
	m_followEffectDataTable.remove_if(
		[effectPlayingHandle](FollowEffectData data) 
		{ return *data.playingEffectHandle == effectPlayingHandle; });
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