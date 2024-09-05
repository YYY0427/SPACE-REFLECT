#pragma once

/// <summary>
/// �Z�[�u�f�[�^�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SaveData
{
private:
	// �Z�[�u�f�[�^
	struct Data
	{
		int version = 0;		// �f�[�^�̃o�[�W����
		int masterVolume = 5;	// �S�̂̉���
		int bgmVolume = 5;		// BGM����	
		int seVolume = 5;		// SE����	
		int padStickSensitivityX = 6;	// �p�b�h�̃X�e�B�b�N�̊��xX
		int padStickSensitivityY = 3;	// �p�b�h�̃X�e�B�b�N�̊��xY
		bool padStickReverseX = false;	// �p�b�h�̃X�e�B�b�N�̉��̃��o�[�X
		bool padStickReverseY = false;	// �p�b�h�̃X�e�B�b�N�̏c�̃��o�[�X
		bool windowMode = false;		// �E�B���h�E���[�h�̐؂�ւ�	
	};
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SaveData();

	/// <summary>
	/// �B��̃C���X�^���X��Ԃ�
	/// </summary>
	/// <returns>�B��̎��Ԃ̎Q��</returns>
	static SaveData& GetInstance();

	/// <summary>
	/// �Z�[�u�f�[�^�̓ǂݍ��� 
	/// </summary>
	void Load();

	/// <summary>
	/// �Z�[�u�f�[�^�̏������� 
	/// �Z�[�u�f�[�^�̃o�[�W��������v���Ȃ��ꍇ�͎~�߂�
	/// </summary>
	void Write();

	/// <summary>
	/// �Z�[�u�f�[�^��V�K�쐬���ď㏑�� 
	/// </summary>
	void CreateNewData();

	/// <summary>
	/// �Z�[�u�f�[�^�̎擾
	/// </summary>
	/// <returns>�Z�[�u�f�[�^</returns>
	const Data& GetSaveData() const;

	/// <summary>
	/// �R���t�B�O�̒l�̐ݒ�
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="configValue">�ݒ肵�����l</param>
	/// <param name="splitNum">������</param>
	template<class T> void SetConfigValue(T& configValue, int splitNum);

	/// <summary>
	/// �R���t�B�O�̃X�C�b�`�̐ݒ�
	/// </summary>
	/// <param name="config">�ݒ肵�����X�C�b�`</param>
	void SetConfigSwitch(bool& config);

	/// <summary>
	/// �S�̉��ʂ̐ݒ�
	/// ���ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	/// </summary>
	/// <param name="configNum">���ʂ̕�����</param>
	void SetMasterVolume(const int configNum);	

	/// <summary>
	/// BGM�̉��ʂ̐ݒ�
	/// ���ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	/// </summary>
	/// <param name="configNum">���ʂ̕�����</param>
	void SetBgmVolume(const int configNum);	

	/// <summary>
	/// SE�̉��ʂ̐ݒ�
	/// ���ʂ̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	/// </summary>
	/// <param name="configNum">���ʂ̕�����</param>
	void SetSeVolume(const int configNum);		

	/// <summary>
	/// �p�b�h�̃X�e�B�b�N��X���x�̐ݒ� 
	/// ���x�̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	/// </summary>
	/// <param name="configNum">������</param>
	void SetPadStickSensitivityX(const int configNum);

	/// <summary>
	/// �p�b�h�̃X�e�B�b�N��Y���x�̐ݒ� 
	/// ���x�̐ݒ�(�ő�l�𒴂����0�ɖ߂�)
	/// </summary>
	/// <param name="configNum">������</param>
	void SetPadStickSensitivityY(const int configNum);

	/// <summary>
	/// �p�b�h�̃X�e�B�b�NX�̃��o�[�X�̐ݒ� 
	/// </summary>
	void SetPadStickReverseX();

	/// <summary>
	/// �p�b�h�̃X�e�B�b�NY�̃��o�[�X�̐ݒ� 
	/// </summary>
	void SetPadStickReverseY();

	/// <summary>
	/// �E�B���h�E���[�h�̐؂�ւ� 
	/// </summary>
	void SetWindowMode();

private:
	/// <summary>
	/// �R���X�g���N�^
	/// �V���O���g���p�^�[���Ȃ̂�private
	/// </summary>
	SaveData();

	// �R�s�[�Ƒ���̋֎~
	SaveData(const SaveData&) = delete;			// �R�s�[�R���X�g���N�^�֎~
	void operator = (const SaveData&) = delete;	// ����֎~

	/// <summary>
	/// �f�[�^�̍폜 
	/// </summary>
	void InitData();

private:
	// �Z�[�u�f�[�^
	Data m_saveData;
};