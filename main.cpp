
//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"

#include "keyboard.h"		//�L�[�{�[�h�̏���

#include "FPS.h"			//FPS�̏���

//�\���̂̒�`

//�}�N����`
#define TAMA_DIV_MAX	4	//�e�̉摜�̍ő吔

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	BOOL IsDraw = FALSE;//�摜���`��ł���H
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			//�摜�\����

	int speed = 1;		//�ړ����x

	RECT coll;			//�����蔻��̗̈�(�l�p)

	
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	int Volume = 255;	//�{�����[��(�ŏ�)0�`255(�ő�)
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//�{�����[���iMIN�@�O�`�Q�T�T�@MAX�j
	int playType = -1;	//BGM or SE
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;							//	���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;						//	�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;						//	���̃Q�[���̃V�[��

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;							//	�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;							//	�t�F�[�h�C��

int fadeTimeMill = 2000;						//	�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;		//	�~���b���t���[���b�ɕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;							//�����l
int fadeOutCnt = fadeOutCntInit;				//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;				//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int fadeInCntInit = fadeTimeMax;				//�����l
int fadeInCnt = fadeInCntInit;					//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;					//�t�F�[�h�A�E�g�̃J�E���^MAX

//�e�̃n���h��
int Tama[TAMA_DIV_MAX];
int TamaIndex = 0;								//�摜�̓Y��
int TanaChangeCnt = 0;							//�摜��ς���^�C�~���O
int TanaChangeCntMax = 30;						//�摜��ς���^�C�~���OMAX

//�v���g�^�C�v�錾
VOID Title(VOID);								//	�^�C�g�����
VOID TitleProc(VOID);							//	�^�C�g����ʁi�����j
VOID TitleDraw(VOID);							//	�^�C�g����ʁi�`��j

VOID Play(VOID);								//	�v���C���
VOID PlayProc(VOID);							//	�v���C��ʁi�����j
VOID PlayDraw(VOID);							//	�v���C��ʁi�`��j

VOID End(VOID);									//	�G���h���
VOID EndProc(VOID);								//	�G���h��ʁi�����j
VOID EndDraw(VOID);								//	�G���h��ʁi�`��j

VOID EndOver(VOID);								//�Q�[���I�[�o�[���
VOID EndOverProc(VOID);							//�Q�[���I�[�o�[��ʁi�����j
VOID EndOverDraw(VOID);							//�Q�[���I�[�o�[��ʁi�`��j

VOID Change(VOID);								//	�؂�ւ����
VOID ChangeProc(VOID);							//	�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);							//	�؂�ւ���ʁi�`��j

VOID ChangeScene(GAME_SCENE scene);				//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);		//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);				//�����蔻��

BOOL OnCollRect(RECT A, RECT B);				//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);							//�Q�[���̃f�[�^�̓ǂݍ���

BOOL LoadImageMem(IMAGE* image, const char* path);								//�摜�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//�Q�[���̉��y��ǂݍ���
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate);	//�Q�[���摜�̕����ǂݍ���

VOID GameInit(VOID);							//�Q�[���f�[�^�̏�����



// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���I
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O�����񂽂�Ɏg����d�g��

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{

	SetOutApplicationLogValidFlag(FALSE);				//	tag.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);								//	�E�B���h�E���[�h�ɐݒ�

	SetMainWindowText(GAME_TITLE);						//	�E�B���h�E�̃^�C�g������

	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//	�E�B���h�E�̉𑜓x��ݒ�

	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//	�E�B���h�E�̑傫����ݒ�

	SetBackgroundColor(255, 255, 255);					//	�f�t�H���g�̔w�i�̐F

	SetWindowIconID(GAME_ICON_ID);						//	�A�C�R���t�@�C����ǂݍ���

	SetWindowStyleMode(GAME_WINDOW_BAR);				//	�E�B���h�E�o�[�̏��

	SetWaitVSyncFlag(TRUE);								//	�f�B�X�v���C�̐���������L���ɂ��� ���d�v

	SetAlwaysRunFlag(TRUE);								//	�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

	//WaitKey();				// �L�[���͑҂�

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{

		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�

		DxLib_End();	//DxLib�I��
		return -1;		//�ُ�I��

	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v
	while (1)
	{

		if (ProcessMessage() != 0)	{ break; }	//���b�Z�[�W���󂯎�葱����

		if (ClearDrawScreen() != 0) { break; }  //��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_ENDOVER:
			EndOver();			//�Q�[���I�[�o�[���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();
		
		ScreenFlip();			//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�ǂݍ��񂾉摜�����
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(Tama[i]); }
	

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					//	�\�t�g�̏I�� 
}

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂��� TRUE / �ǂݍ��߂��� FALSE</returns>
BOOL GameLoad(VOID)
{
	if (LoadImageDivMem(&Tama[0], ".\\image\\tama.png", 4, 1) == FALSE) { return FALSE; }
	
	return TRUE;	//���ׂēǂݍ��߂�
	
}

/// <summary>
/// �摜�𕪊����ă������ɓǂݍ���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate)
{

	

	//�e�̓ǂݍ���
	int IsTamaLoad = -1;			//�摜���ǂݍ��߂����H

	//�ꎞ�I�Ɏg���n���h����p�ӂ���
	int TamaHandle = LoadGraph(path);

	//�ǂݍ��݃G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;				//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	int TamaWidth = -1;				//��
	int TamaHeight = -1;			//����
	GetGraphSize(TamaHandle, &TamaWidth, &TamaHeight);

	//�������ēǂݍ���
	IsTamaLoad = LoadDivGraph(
		".\\image\\tama.png",								//�摜�̃p�X
		TAMA_DIV_MAX,										//��������
		bunkatuYoko, bunkatuTate,							//���̕����A�c�̕���
		TamaWidth / bunkatuYoko, TamaHeight / bunkatuTate,	//�摜1���̕��A����
		handle												//�A���ŊǗ�����z��̐擪�A�h���X
	);

	//�����G���[
	if (TamaHandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�B���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;				//�ǂݍ��ݎ��s
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�������
	DeleteGraph(TamaHandle);

	return TRUE;
}

/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	

}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scerne">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;		//�V�[����؂�ւ�
	IsFadeIn = FALSE;		//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;		//�t�F�[�h�A�E�g���Ȃ�

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();		//����
	TitleDraw();		//�`��
	
	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{
	//�v���C�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{
	//�e�̕`��
	DrawGraph(0, 0, Tama[TamaIndex], TRUE);

	if (TanaChangeCnt < TanaChangeCntMax)
	{
		TanaChangeCnt++;
	}
	else
	{
		//�e�̓Y�����e�̕������̍ő�������̂Ƃ�
		if (TamaIndex < TAMA_DIV_MAX - 1)
		{
			TamaIndex++;	//���̉摜��
		}
		else
		{
			TamaIndex = 0;	//�ŏ��ɖ߂�
		}
		TanaChangeCnt = 0;
	}

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();		//����
	PlayDraw();		//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	//�G���h�V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}


	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();		//����
	EndDraw();		//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{

	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// GameOve���
/// </summary>
VOID EndOver(VOID)
{
	EndOverProc();		//����
	EndOverDraw();		//�`��

	return;
}

/// <summary>
/// GameOve��ʂ̏���
/// </summary>
VOID EndOverProc(VOID)
{
	

	//�^�C�g���V�[���֐؂�ւ���
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

/// <summary>
/// GameOver��ʂ̕`��
/// </summary>
VOID EndOverDraw(VOID)
{
	DrawString(0, 0, "�Q�[���I�[�o�[���", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();		//����
	ChangeDraw();		//�`��

	return;
}

/// <summary>
/// �؂�ւ���ʂ̏���
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�𑝂₷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ������I��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃Q�[���V�[���X�V
	}

	return;
}

/// <summary>
/// �؂�ւ���ʂ̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();			//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();				//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();				//�G���h��ʂ̕`��
		break;
	case GAME_SCENE_ENDOVER:	//�Q�[���I�[�o�[��ʂ̕`��
		EndOverDraw();
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	
	return;
}

/// <summary>
/// �v���C���[�����蔻��̗̈�X�V
/// </summary>
/// <param name="Coll"></param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 25;						//�����蔻��������
	chara->coll.top = chara->img.y + 15;						//�����蔻��������
	chara->coll.right = chara->img.x + chara->img.width - 15;		//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 5;		//�����蔻��������

	return;
}

/// <summary>
/// �S�[�������蔻��̍X�V
/// </summary>
/// <param name="chara"></param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;


	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// �����蔻��̏���
/// </summary>
/// <param name="A">��`(������)A</param>
/// <param name="B">��`(������)B</param>
/// <returns>�����������������ĂȂ���</returns>
BOOL OnCollRect(RECT A, RECT B)
{
	if ( 
		 A.left < B.right &&	//���ӂ�X���W���E�ӂ�X���W   ����
		 A.right > B.left &&	//�E�ӂ�X���W�����ӂ�X���W   ����
		 A.top < B.bottom &&	//��ӂ̂x���W�����ӂ̂x���W ����
		 A.bottom > B.top		//���ӂ̂x���W����ӂ̂x���W
		)
	{

		return TRUE;
	
	}
	else
	{
	
		return FALSE;
	
	}
}

BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	//���y�̓ǂݍ���
	strcpyDx(audio->path, path);					//�p�X�̃R�s�[
	audio->handle = LoadSoundMem(audio->path);		//���y�̓ǂݍ���

	//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[�i�[1�j������
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,			//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;		//�ǂݍ��ݎ��s
	}

	//���̑��̐ݒ�
	audio->Volume = volume;
	audio->playType = playType;

	return TRUE;
}

BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�S�[���̉摜��ǂݍ���
	strcpyDx(image->path, path);	//�p�X�̃R�s�[
	image->handle = LoadGraph(image->path);	//�摜�̓ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,			//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);

		return FALSE;	//�ǂݍ��ݎ��s
	}

	//�摜�̕��ƍ������擾
	GetGraphSize(image->handle, &image->width, &image->height);

	//�ǂݍ��߂�
	return TRUE;
}


