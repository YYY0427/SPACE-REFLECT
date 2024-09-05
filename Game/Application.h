#pragma once
#include "Util/SingletonBase.h"

// �E�B���h�E�̃T�C�Y
struct Size
{
	int width;
	int height;
};

/// <summary>
/// �A�v���P�[�V�����S�̂��Ǘ�����V���O���g���N���X
/// </summary>
class Application : public SingletonBase<Application>
{
public:
	// SingletonBase�N���X�ł̂݃C���X�^���X������������
	friend class SingletonBase<Application>;

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>true : �����Afalse : ���s</returns>
	bool Init();

	/// <summary>
	/// ���s
	/// </summary>
	void Run();
	
	/// <summary>
	/// �E�B���h�E�̃T�C�Y���擾
	/// </summary>
	/// <returns>�E�B���h�E�̃T�C�Y</returns>
	const Size& GetWindowSize() const;

	/// <summary>
	/// FPS���擾
	/// </summary>
	/// <returns>FPS</returns>
	int GetFps() const;

	/// <summary>
	/// �A�v���P�[�V�������I������
	/// </summary>
	void Exit();

private:
	/// <summary>
	/// �R���X�g���N�^
	/// �V���O���g���̂���private
	/// </summary>
	Application();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

private:
	// �E�B���h�E�̃T�C�Y
	Size m_screenSize;

	// �I���t���O
	bool m_isExit;
};