#include "UIManager.h"
#include "UIBase.h"
#include <algorithm>

namespace
{
	// UI‚ÌŠi”[‘¬“x
	constexpr float store_speed = 10.0f;
}

namespace UI
{
	// XV
	void Manager::Update()
	{
		// XV
		for (auto& ui : m_uiTable)
		{
			ui.ui->Update();
		}
	}

	// •`‰æ
	void Manager::Draw()
	{
		// •`‰æ
		for (auto& ui : m_uiTable)
		{
			ui.ui->Draw();
		}
	}

	// UI‚Ì’Ç‰Á
	void Manager::AddUI(const std::string& key, const std::shared_ptr<UIBase>& ui, const int drawOrder, const Math::Vector2& storeVec)
	{
		// ‚·‚Å‚ÉƒL[‚ª“o˜^‚³‚ê‚Ä‚¢‚éê‡‚Í’Ç‰Á‚µ‚È‚¢
		for (auto& uiT : m_uiTable)
		{
			if (uiT.ui == ui) return;
		}

		// ƒf[ƒ^‚Ìì¬
		UIData data;
		data.drawOrder = drawOrder;	// •`‰æ‡‚ğİ’è
		data.key = key;				// ƒL[‚ğİ’è
		data.ui = ui;				// UI‚ğİ’è
		data.storeVec = storeVec;	// UI‚ğŠi”[‚·‚éƒxƒNƒgƒ‹

		// ƒf[ƒ^‚ğ’Ç‰Á
		m_uiTable.push_back(data);

		// •`‰æ‡‚Éƒ\[ƒg
		std::sort(m_uiTable.begin(), m_uiTable.end(),
			[](const UIData& a, const UIData& b) { return a.drawOrder < b.drawOrder; });
	}

	// UI‚Ìíœ
	void Manager::DeleteUI(const std::string& key)
	{
		// ƒL[‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©
		auto itr = std::find_if(m_uiTable.begin(), m_uiTable.end(),
			[&key](const UIData& data) { return data.key == key; });

		// “o˜^‚³‚ê‚Ä‚¢‚È‚¢ê‡‚Í‰½‚à‚µ‚È‚¢
		if (itr == m_uiTable.end()) return;

		// íœ
		m_uiTable.erase(itr);
	}

	// ‚·‚×‚Ä‚ÌUI‚Ìíœ
	void Manager::DeleteAllUI()
	{
		// ‘S‚Ä‚ÌUI‚ğíœ
		m_uiTable.clear();
	}

	// UI‚ÌŠi”[
	void Manager::Store()
	{
		for (auto& ui : m_uiTable)
		{
			ui.ui->Store(ui.storeVec, store_speed);
		}
	}
}