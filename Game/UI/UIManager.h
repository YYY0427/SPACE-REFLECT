#pragma once
#include <string>
#include <vector>
#include <memory>
#include "UIBase.h"
#include "../Util/SingletonBase.h"
#include "../Math/Vector2.h"

namespace UI
{
	/// <summary>
	/// UI�̊Ǘ�����V���O���g���N���X
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������
		friend class SingletonBase<Manager>;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Manager(){}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// UI�̒ǉ�
		/// </summary>
		/// <param name="key">�L�[</param>
		/// <param name="ui">UI�̃|�C���^</param>
		/// <param name="drawOrder">�`�揇</param>
		/// <param name="storeVec">UI���i�[����x�N�g��</param>
		void AddUI(const std::string& key, const std::shared_ptr<UIBase>& ui, const int drawOrder, const Math::Vector2& storeVec = {});

		/// <summary>
		/// UI�̍폜
		/// </summary>
		/// <param name="key">�폜������UI�̃L�[</param>
		void DeleteUI(const std::string& key);

		/// <summary>
		/// �S�Ă�UI�̍폜
		/// </summary>
		void DeleteAllUI();

		/// <summary>
		/// UI�̊i�[
		/// </summary>
		void Store();

	private:
		/// <summary>
		/// �R���X�g���N�^ 
		/// �V���O���g���p�^�[���Ȃ̂�private
		/// </summary>
		Manager(){}

	private:
		// UI�̃f�[�^
		struct UIData
		{
			std::string key;			// �L�[
			std::shared_ptr<UIBase> ui; // UI�̃|�C���^
			int drawOrder;				// �`�揇
			Math::Vector2 storeVec;			// UI���i�[����x�N�g��
		};

	private:
		// UI�̃e�[�u��
		std::vector<UIData> m_uiTable;
	};
}