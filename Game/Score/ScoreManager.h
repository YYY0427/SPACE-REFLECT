#pragma once
#include <array>
#include "../Util/SingletonBase.h"

namespace Score
{
	// �X�R�A�̎��
	enum class Type
	{
		// ��������
		ENEMY,		// �G��|�����X�R�A
		BOSS,		// �{�X��|�����X�R�A
		ITEM,		// �A�C�e�����擾�����X�R�A

		// ��������
		DAMAGE,		// �_���[�W���X�R�A

		// �X�R�A�̎�ނ̐�
		NUM
	};

	/// <summary>
	/// �X�R�A���Ǘ�����V���O���g���N���X
	/// </summary>
	class Manager : public SingletonBase<Manager>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������ 
		friend class SingletonBase<Manager>;

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Manager();

		/// <summary>
		/// �X�R�A�̒ǉ�
		/// </summary>
		/// <param name="type">�X�R�A�̎��</param>
		void AddScore(const Type type);

		/// <summary>
		/// �X�R�A�̕`��
		/// </summary>
		void DrawScore() const;

		/// <summary>
		/// �X�R�A�̎擾
		/// </summary>
		/// <param name="type">�X�R�A�̎��</param>
		/// <returns>�X�R�A</returns>
		int GetScore(const Type type) const;

		/// <summary>
		/// ���v�X�R�A�̎擾
		/// </summary>
		/// <returns>���v�X�R�A</returns>
		int GetTotalScore() const;

		/// <summary>
		/// �X�R�A�̃��Z�b�g
		/// </summary>
		void Reset();

	private:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Manager();

	private:
		// �X�R�A
		std::array<int, static_cast<int>(Type::NUM)> m_score;
	};
}