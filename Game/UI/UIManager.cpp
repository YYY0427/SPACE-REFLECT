#include "UIManager.h"
#include "UIBase.h"
#include <algorithm>

namespace
{
	// UIの格納速度
	constexpr float store_speed = 10.0f;
}

// コンストラクタ
UIManager::UIManager()
{
}

// デストラクタ
UIManager::~UIManager()
{
}

// 更新
void UIManager::Update()
{
	// 更新
	for (auto& ui : m_uiTable)
	{
		ui.ui->Update();
	}
}

// 描画
void UIManager::Draw()
{
	// 描画
	for (auto& ui : m_uiTable)
	{
		ui.ui->Draw();
	}
}

// インスタンスの取得
UIManager& UIManager::GetInstance()
{
	// 唯一の実態
	static UIManager instance;

	// 唯一の実態参照を返す
	return instance;
}

// UIの追加
void UIManager::AddUI(const std::string& key, const std::shared_ptr<UIBase>& ui, const int drawOrder, const Vector2& storeVec)
{
	// すでにキーが登録されている場合は追加しない
	for (auto& uiT : m_uiTable)
	{
		if(uiT.ui == ui) return;
	}

	// データの作成
	UIData data;
	data.drawOrder = drawOrder;	// 描画順を設定
	data.key = key;				// キーを設定
	data.ui = ui;				// UIを設定
	data.storeVec = storeVec;	// UIを格納するベクトル

	// データを追加
	m_uiTable.push_back(data);	

	// 描画順にソート
	std::sort(m_uiTable.begin(), m_uiTable.end(),
		[](const UIData& a, const UIData& b) { return a.drawOrder < b.drawOrder; });
}

// UIの削除
void UIManager::DeleteUI(const std::string& key)
{
	// キーが登録されているかどうか
	auto itr = std::find_if(m_uiTable.begin(), m_uiTable.end(),
				[&key](const UIData& data) { return data.key == key; });

	// 登録されていない場合は何もしない
	if (itr == m_uiTable.end()) return;

	// 削除
	m_uiTable.erase(itr);
}

// すべてのUIの削除
void UIManager::DeleteAllUI()
{
	// 全てのUIを削除
	m_uiTable.clear();
}

// UIの格納
void UIManager::Store()
{
	for (auto& ui : m_uiTable)
	{
		ui.ui->Store(ui.storeVec, store_speed);
	}
}