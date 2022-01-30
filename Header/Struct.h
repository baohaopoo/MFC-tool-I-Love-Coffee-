#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;

	D3DXMATRIX		matWorld;

}INFO;

typedef	struct tagTexture
{
	// �ؽ�ó �İ�ü
	LPDIRECT3DTEXTURE9		pTexture;

	// �̹��� ������ �����ϱ� ���� ����ü
	D3DXIMAGE_INFO			tImgInfo;

}TEXINFO;


typedef struct	tagFrame 
{
	int			iFrameStart;
	int			iFrameEnd;
	int			iFrameScene;

}FRAME;