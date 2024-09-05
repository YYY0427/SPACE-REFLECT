#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <DxLib.h>

/// <summary>
/// �����񃆁[�e�B���e�B
/// </summary>
namespace String
{
	/// <summary>
	/// ������𕪊�����
	/// </summary>
	/// <param name="str">�������镶����</param>
	/// <param name="delim">��؂蕶��</param>
	/// <returns>�������ꂽ������̔z��</returns>
	std::vector<std::string> Split(const std::string& str, const char delimiter);

	/// <summary>
	/// �w�肵�����W�𒆐S�ɕ������`�悷��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="x">X��</param>
	/// <param name="y">Y��</param>
	/// <param name="color">�F</param>
	/// <param name="str">������</param>
	/// <param name="fontHandle">�t�H���g�n���h��</param>
	void DrawStringCenter(const int x, const int y, const unsigned int color, const std::string& str, const int fontHandle, const unsigned int edgeColor = 0x000000);
}