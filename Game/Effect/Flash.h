#pragma once

namespace Effect
{
	/// <summary>
	/// ��ʂ��t���b�V������N���X
	/// </summary>
	class Flash
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="enableFrame">������܂ł̃t���[����</param>
		Flash(const int enableFrame);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		~Flash();

		/// <summary>
		/// �X�V
		/// </summary>
		/// <param name="color">�t���b�V���̐F</param>
		void Update(const unsigned int color);

		/// <summary>
		/// �`��
		/// </summary>
		void Draw();

		/// <summary>
		/// �I��������
		/// </summary>
		///	<returns>true : �I���Afalse : �r��</returns>
		bool IsEnd() const;

	private:
		// �J���[
		unsigned int m_color;

		// �A���t�@�l
		int m_alpha;

		// ������܂ł̃t���[����
		int m_enableFrame;
	};
}