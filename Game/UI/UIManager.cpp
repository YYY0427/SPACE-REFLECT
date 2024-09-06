#include "UIManager.h"
#include "UIBase.h"
#include <algorithm>

namespace
{
	// UI�̊i�[���x
	constexpr float store_speed = 10.0f;
}

namespace UI
{
	// �X�V
	void Manager::Update()
	{
		// �X�V
		for (auto& ui : m_uiTable)
		{
			ui.ui->Update();
		}
	}

	// �`��
	void Manager::Draw()
	{
		// �`��
		for (auto& ui : m_uiTable)
		{
			ui.ui->Draw();
		}
	}

	// UI�̒ǉ�
	void Manager::AddUI(const std::string& key, const std::shared_ptr<UIBase>& ui, const int drawOrder, const Math::Vector2& storeVec)
	{
		// ���łɃL�[���o�^����Ă���ꍇ�͒ǉ����Ȃ�
		for (auto& uiT : m_uiTable)
		{
			if (uiT.ui == ui) return;
		}

		// �f�[�^�̍쐬
		UIData data;
		data.drawOrder = drawOrder;	// �`�揇��ݒ�
		data.key = key;				// �L�[��ݒ�
		data.ui = ui;				// UI��ݒ�
		data.storeVec = storeVec;	// UI���i�[����x�N�g��

		// �f�[�^��ǉ�
		m_uiTable.push_back(data);

		// �`�揇�Ƀ\�[�g
		std::sort(m_uiTable.begin(), m_uiTable.end(),
			[](const UIData& a, const UIData& b) { return a.drawOrder < b.drawOrder; });
	}

	// UI�̍폜
	void Manager::DeleteUI(const std::string& key)
	{
		// �L�[���o�^����Ă��邩�ǂ���
		auto itr = std::find_if(m_uiTable.begin(), m_uiTable.end(),
			[&key](const UIData& data) { return data.key == key; });

		// �o�^����Ă��Ȃ��ꍇ�͉������Ȃ�
		if (itr == m_uiTable.end()) return;

		// �폜
		m_uiTable.erase(itr);
	}

	// ���ׂĂ�UI�̍폜
	void Manager::DeleteAllUI()
	{
		// �S�Ă�UI���폜
		m_uiTable.clear();
	}

	// UI�̊i�[
	void Manager::Store()
	{
		for (auto& ui : m_uiTable)
		{
			ui.ui->Store(ui.storeVec, store_speed);
		}
	}
}