#pragma once
#include <string>
#include <DxLib.h>
#include <list>

namespace
{
	// �`�悷��X���W
	constexpr int draw_width = 0;

	// �`�悷��X�^�[�gY���W
	constexpr int draw_start_height = 150;

	// �`�悷��Y���W�̊Ԋu
	constexpr int draw_height_interval = 20;

	// �`�悷�镶���̐F
	const unsigned int draw_font_color = 0xffffff;

	// �t�H���g
	const std::string font = "�l�r �S�V�b�N";

	// �����T�C�Y
	constexpr int font_size = 20;

	// �����̑���
	constexpr int font_thickness = 9;

	// �����̔w�i�̐F
	const unsigned int draw_back_color = 0xffffff;

	// �����̔w�i�̓����x
	constexpr int draw_back_alpha = 127;
}

namespace Debug
{
	class Text
	{
	/// <summary>
	/// �f�o�b�O���ɕ\������e�L�X�g
	/// printf�̂悤�Ȃ���
	/// </summary>
	public:
		/// <summary>
		/// ������
		/// </summary>
		static void Init();

		/// <summary>
		/// �I������
		/// </summary>
		static void End();

		/// <summary>
		/// ���O�̃N���A
		/// </summary>
		static void ClearLog();

		/// <summary>
		/// ���O�̕`��
		/// </summary>
		static void DrawLog();

		/// <summary>
		/// ���O�̒ǉ�(�����̂�)
		/// </summary>
		/// <param name="string">�o�͂��镶����</param>
		/// <param name="color">�����̐F</param>
		static void AddLog(const std::string& string, const unsigned int color = 0xffffff);

		/// <summary>
		/// ���O�̒ǉ�(�����Ɛ���)
		/// </summary>
		/// <param name="string">������</param>
		/// <typeparam name="T">�ϐ��̌^</typeparam>
		/// <param name="array">�ϐ�</param>
		/// <typeparam name="N">�ϐ��̐�</typeparam>
		/// <param name="color">�����̐F</param>
		template <typename T, size_t N>
		static void AddLog(const std::string& string, const T(&array)[N], const unsigned int color = 0xffffff)
		{
#ifdef _DEBUG
			// ���O�f�[�^�̍쐬
			LogData logData;
			logData.str = string;
			logData.color = color;

			// �z��̒��g�𕶎���ɕϊ�
			for (int i = 0; i < N; i++)
			{
				// 1�ڂ̗v�f�� : �ŋ�؂�
				if (i == 0)
				{
					logData.str += " : " + std::to_string(array[i]);
				}
				// 2�ڈȍ~�̗v�f�� , �ŋ�؂�
				else
				{
					logData.str += ", " + std::to_string(array[i]);
				}
			}
			// ���O���i�[
			m_logList.push_back(logData);
#endif
		}

	private:
		// ���O�f�[�^
		struct LogData
		{
			// ������
			std::string str;

			// �����̐F
			unsigned int color = 0xffffff;
		};

	private:
		// ���O���X�g
		static std::list<LogData> m_logList;

		// ���O�̏o�͉�
		static int m_logCount;

		// �t�H���g�n���h��
		static int m_fontHandle;
	};
}