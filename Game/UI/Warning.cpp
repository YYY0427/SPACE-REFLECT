#include "Warning.h"
#include "../Util/DrawFunctions.h"
#include "../Application.h"
#include "../String/MessageManager.h"
#include "../SoundManager.h"
#include <string>
#include <DxLib.h>

namespace
{
	// 画像ファイルパス
	const std::string center_img_file_path = "Data/Image/Warning.png";
	const std::string up_img_file_path =     "Data/Image/WarningUp.png";
	const std::string down_img_file_path =   "Data/Image/WarningDown.png";

	// 警告文字のフェードイン・アウトの速度
	constexpr float string_fade_speed = 0.07f;

	// 上下の画像のスクロール速度
	constexpr int scroll_speed = 10;

	// 全体のフェードイン・アウトの速度
	constexpr float all_fade_speed = 5.0f;
}

// コンストラクタ
Warning::Warning(const int drawFrame) :
	m_isEnd(false),
	m_scroll(0),
	m_drawFrame(drawFrame),
	m_imgAlphaParam(0),
	m_addImgAlphaParam(all_fade_speed),
	m_stringAlphaParam(0),
	m_ImgAlphaParamRange(0, 255)
{
	// 画像のロード
	m_uiDataTable[static_cast<int>(ID::CENTER)].imgHandle = my::MyLoadGraph(center_img_file_path.c_str());
	m_uiDataTable[static_cast<int>(ID::UP)].imgHandle = my::MyLoadGraph(up_img_file_path.c_str());
	m_uiDataTable[static_cast<int>(ID::DOWN)].imgHandle = my::MyLoadGraph(down_img_file_path.c_str());

	// 画像のサイズを取得
	for (auto& data : m_uiDataTable)
	{
		GetGraphSize(data.imgHandle, &data.imgWidth, &data.imgHeight);
	}

	// 画像の座標を設定
	auto& screenSize = Application::GetInstance().GetWindowSize();
	m_uiDataTable[static_cast<int>(ID::CENTER)].pos.x = screenSize.width / 2.0f;
	m_uiDataTable[static_cast<int>(ID::CENTER)].pos.y = screenSize.height / 2.0f;
	m_uiDataTable[static_cast<int>(ID::UP)].pos.y = (screenSize.height / 2.0f) - 300;
	m_uiDataTable[static_cast<int>(ID::DOWN)].pos.y = (screenSize.height / 2.0f) + 300;

	// 画像のスクロール方向を設定
	m_uiDataTable[static_cast<int>(ID::UP)].scrollDirection = 1;
	m_uiDataTable[static_cast<int>(ID::DOWN)].scrollDirection = -1;
	m_uiDataTable[static_cast<int>(ID::CENTER)].scrollDirection = 0;

	// 通常のBGMのファイル名の取得
	auto& soundManager = SoundManager::GetInstance();
	auto fileName = soundManager.GetPlayBGMFileName();

	// 再生中のBGMがあったら
	if (fileName != "")
	{
		// 通常のBGMのフェードアウトの設定
		soundManager.SetFadeSound(fileName, 120, soundManager.GetSoundVolume(fileName), 0);
	}

	// 警告のSEの再生
	soundManager.PlaySE("Warning");

	// 警告のSEのフェードインの設定
	soundManager.SetFadeSound("Warning", 20, 0, 255);
}

// デストラクタ
Warning::~Warning()
{
	// 画像のアンロード
	for (auto& data : m_uiDataTable)
	{
		DeleteGraph(data.imgHandle);
	}
}

// 更新
void Warning::Update()
{
	// 描画時間が終了したら
	if (m_drawFrame-- <= 0 && !m_isEnd)
	{
		// 終了フラグを立てる
		m_isEnd = true;

		// 警告のSEのフェードアウトの設定
		auto& soundManager = SoundManager::GetInstance();
		soundManager.SetFadeSound("Warning", 30, soundManager.GetSoundVolume("Warning"), 0);

		// 警告画像のアルファ値を減らす
		m_addImgAlphaParam *= -1;
	}

	// スクロール
	m_scroll += scroll_speed;

	// 警告文字のフェード
	if (m_isEnd)
	{
		// フェードアウト
		m_stringAlphaParam -= all_fade_speed;
	}
	else
	{
		// フェードイン・アウトを繰り返す
		m_stringAlphaParam = (0.5f * sinf(m_drawFrame * string_fade_speed) + 0.5f) * 255.0f;
	}

	// 警告画像のアルファ値を更新
	m_imgAlphaParam += m_addImgAlphaParam;
	m_imgAlphaParam = m_ImgAlphaParamRange.Clamp(m_imgAlphaParam);
}

// 描画
void Warning::Draw()
{
	for (auto& data : m_uiDataTable)
	{
		// スクロールの割合を計算
		int scroll = (m_scroll * data.scrollDirection) % data.imgWidth;

		// スクロールあり描画
		if (data.scrollDirection != 0)
		{
			// 画像のアルファ値を設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_imgAlphaParam);

			// スクロールするので2枚描画
			DrawRotaGraph((data.imgWidth * 0.5f) + scroll, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);
			DrawRotaGraph((data.imgWidth * (1 * -data.scrollDirection + 0.5f)) + scroll, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);

			// 元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// スクロールなし描画
		else
		{
			// 画像のアルファ値を設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_imgAlphaParam);

			// 画像の描画
			DrawRotaGraph(data.pos.x, data.pos.y, 1.0f, 0.0f, data.imgHandle, true);

			// 元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	// 警告文字の描画
	auto& screenSize = Application::GetInstance().GetWindowSize();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_stringAlphaParam);
	MessageManager::GetInstance().DrawStringCenter("WarningUI", screenSize.width / 2.0f, screenSize.height / 2.0f, 0xffffff);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 終了しているかどうか
bool Warning::IsEnd() const
{
	return (m_isEnd) && (m_imgAlphaParam <= 0);
}
