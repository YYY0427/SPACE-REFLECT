#include "MessageManager.h"
#include "StringUtil.h"
#include "../Util/FileUtil.h"

namespace
{
	// ���b�Z�[�W�f�[�^�̃t�@�C���p�X
	const std::string message_file_path = "Data/Csv/Message.csv";
}

namespace String
{
	// �R���X�g���N�^
	MessageManager::MessageManager()
	{
	}

	// �f�X�g���N�^
	MessageManager::~MessageManager()
	{
	}

	// �t�@�C�����當����̃f�[�^��ǂݎ���ăf�[�^�e�[�u���Ɋi�[
	void MessageManager::LoadMessageFileData()
	{
		// �t�@�C�����̓ǂݍ���(�ǂݍ��݂Ɏ��s������~�߂�)
		std::vector<std::vector<std::string>> str = FileUtil::LoadCsvFile(message_file_path);
		for (auto& data : str)
		{
			// csv�f�[�^����擾�����f�[�^����t�H���g�n���h���̍쐬���Ċi�[
			MessageData messageData;
			std::string font = data[static_cast<int>(MessageDataType::FONT)];
			int size = std::stoi(data[static_cast<int>(MessageDataType::SIZE)]);
			int thick = std::stoi(data[static_cast<int>(MessageDataType::THICK)]);
			messageData.fontHandle = CreateFontToHandle(font.c_str(), size, thick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

			// ������̊i�[
			messageData.string = data[static_cast<int>(MessageDataType::STRING)];

			// �f�[�^�̊i�[
			m_messageDataTable[data[static_cast<int>(MessageDataType::ID)]] = messageData;
		}
	}

	// �I������
	void MessageManager::End()
	{
		// �t�H���g�f�[�^�̍폜
		for (auto& font : m_messageDataTable)
		{
			DeleteFontToHandle(font.second.fontHandle);
		}

		// �C���X�^���X�̍폜
		DeleteInstance();
	}

	// �t�@�C���ɓǂݍ��񂾕�����̕\��
	void MessageManager::DrawString(const std::string& id, const int x, const int y, const unsigned int color)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		DrawStringToHandle(x, y, m_messageDataTable[id].string.c_str(), color, m_messageDataTable[id].fontHandle);
	}

	// �t�@�C���ɓǂݍ��񂾕�������w�肵�����W�𒆐S�Ƃ����ʒu�ɕ\��
	void MessageManager::DrawStringCenter(const std::string& id, const int x, const int y, const unsigned int color, const unsigned int edgeColor)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		// �t�H���g�T�C�Y�A�A������̒������當����̉����̎擾
		std::string string = m_messageDataTable[id].string;
		int fontHandle = m_messageDataTable[id].fontHandle;
		int width = GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
		int height = GetFontSizeToHandle(fontHandle);

		// �擾����������̉������當������������ʒu�ɕ\��
		DrawStringToHandle(x - width / 2, y - height / 2, string.c_str(), color, fontHandle, edgeColor);
	}

	// ���b�Z�[�W�̃f�[�^�̎擾
	const MessageData& MessageManager::GetMessageData(const std::string& id)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());
		return m_messageDataTable[id];
	}

	// ���b�Z�[�W�̕��̎擾
	int MessageManager::GetMessageWidth(const std::string& id)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		// �t�H���g�T�C�Y�A�A������̒������當����̉����̎擾
		std::string string = m_messageDataTable[id].string;
		int fontHandle = m_messageDataTable[id].fontHandle;
		return GetDrawStringWidthToHandle(string.c_str(), static_cast<int>(strlen(string.c_str())), fontHandle);
	}

	// ���b�Z�[�W�̍����̎擾
	int MessageManager::GetMessageHeight(const std::string& id)
	{
		// ���[�h���Ă��Ȃ��ꍇ�͎~�߂�
		assert(m_messageDataTable.find(id) != m_messageDataTable.end());

		// �t�H���g�T�C�Y�A�A������̒������當����̉����̎擾
		int fontHandle = m_messageDataTable[id].fontHandle;
		return GetFontSizeToHandle(fontHandle);
	}
}