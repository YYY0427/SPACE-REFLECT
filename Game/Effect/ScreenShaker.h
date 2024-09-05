#pragma once
#include "../Util/Timer.h"
#include "../Math/Vector2.h"
#include <memory>

// �v���g�^�C�v�錾
class Camera;

namespace Effect
{
	/// <summary>
	/// ��ʂ�h�炷�N���X
	/// </summary>
	class ScreenShaker
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="pCamera">�J�����̃|�C���^</param>
		ScreenShaker(const std::shared_ptr<Camera>& pCamera);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~ScreenShaker();

		/// <summary>
		/// �X�V
		/// </summary>
		void Update();

		/// <summary>
		/// �`�揀��
		/// �`��O�ɕK���Ă� 
		/// </summary>
		void PreDraw();

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// ��ʗh��̊J�n
		/// </summary>
		/// <param name="quakeSize">��ʗh��̑傫��</param>
		/// <param name="frame">��ʗh��̃t���[��</param>
		void StartShake(const Math::Vector2& quakeSize, const int frame);

	private:
		// �J�����|�C���^
		std::shared_ptr<Camera> m_pCamera;

		// �h��鎞��
		Timer<int> m_quakeTimer;

		// �h��鋭��
		Math::Vector2 m_quakeSize;

		// PreDraw��������
		bool m_isPreDraw;

		// ��ʃn���h��
		int m_screenHandle;
	};
}