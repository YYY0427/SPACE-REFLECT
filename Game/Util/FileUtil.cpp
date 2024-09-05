#include "FileUtil.h"
#include "../String/StringUtil.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cassert>

namespace FileUtil
{
	// �t�H���_�z���̃t�@�C�����̈ꊇ�ǂݍ���
	std::vector<std::string> LoadFileNames(const std::string& folderPath, const std::string& extension)
	{
		std::vector<std::string> fileNames;

		// �t�@�C�����̈ꊇ�ǂݍ���
		for (auto& p : std::filesystem::directory_iterator(folderPath))
		{
			// �g���q�̎擾
			std::string ext = p.path().extension().string();

			// �g���q����v���Ă��邩
			if (ext == extension)
			{
				// �t�@�C�����̕ۑ�
				fileNames.push_back(p.path().filename().string());
			}
		}
		return fileNames;
	}

	// CSV�t�@�C���̓ǂݍ���
	// 1�s�ڂ͓ǂݍ��܂Ȃ�
	std::vector<std::vector<std::string>> LoadCsvFile(const std::string& filePath)
	{
		std::vector<std::vector<std::string>> data;

		// �t�@�C�����J��
		std::ifstream inputFile(filePath);

		// �t�@�C�����J������
		bool isFirstLine = true;
		if (inputFile.is_open())
		{
			// 1�s���ǂݍ���
			std::string line;
			while (getline(inputFile, line))
			{
				// 1�s�ڂ͓ǂݍ��܂Ȃ�
				if (isFirstLine)
				{
					isFirstLine = false;
					continue;
				}

				// csv�f�[�^�P�s��','�ŕ����̕�����ɕϊ�
				std::vector<std::string> lineData = String::Split(line, ',');

				// 1�s���̃f�[�^��ۑ�
				data.push_back(lineData);
			}

			// �t�@�C�������
			inputFile.close();
		}
		else
		{
			// �t�@�C�����J���Ȃ�����
			assert(!"�t�@�C�����J���܂���ł���");
		}
		return data;
	}
}