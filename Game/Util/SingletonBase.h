#pragma once
#include <memory>

/// <summary>
/// �V���O���g���N���X�̊��N���X
/// </summary>
template<class T>
class SingletonBase
{
public:
	/// <summary>
	/// �C���X�^���X���擾����
	/// ���߂Ď擾����ꍇ�̓C���X�^���X���쐬����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static T* GetInstance()
	{
		// �C���X�^���X���Ȃ��ꍇ�͍쐬����
		if (!m_pInstance)
		{
			// �C���X�^���X���쐬
			m_pInstance = new T();
		}
		// �C���X�^���X��Ԃ�
		return m_pInstance;
	}

	/// <summary>
	/// �C���X�^���X��j������
	/// </summary>
	static void DeleteInstance()
	{
		// �C���X�^���X������ꍇ�͔j������
		if (m_pInstance)
		{
			// �C���X�^���X��j��
			delete m_pInstance;

			// �C���X�^���X��nullptr�ɂ���
			m_pInstance = nullptr;
		}
	}

protected:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SingletonBase() {}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SingletonBase() {}

private:
	// �R�s�[�֎~
	SingletonBase(const SingletonBase&) = delete;

	// ����֎~
	SingletonBase& operator=(const SingletonBase&) = delete;

	// �C���X�^���X
	static T* m_pInstance;
};

// �C���X�^���X�̎��̉�
template <typename T> T* SingletonBase<T>::m_pInstance;