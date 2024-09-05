#include <DxLib.h>
#include <crtdbg.h>
#include "Application.h"

// �G���g���[�|�C���g
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef _DEBUG
	// ���������[�N�̊m�F
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���[�N���������Ă��郁�����m�ۉ񐔂������ɂ��邱�ƂŁA
	// ���̌��Ƀf�o�b�O�u���[�N�������Ă����
//	_CrtSetBreakAlloc(136);
#endif

	// �A�v���P�[�V�����̃C���X�^���X���擾
	const auto& app = Application::GetInstance();

	// ������
	if (!app->Init())
	{
		// �������Ɏ��s������I��
		return -1;
	}

	// ���s
	app->Run();

	// �C���X�^���X�̍폜
	app->DeleteInstance();

	// �\�t�g�̏I�� 
	return 0;				
}