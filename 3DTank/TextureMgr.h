//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TextureMgr.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-04 20:13
//  Comment     :   Container of Texture
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <vector>
#include <map>
#include <string>

class TextureMgr
{
public:

	static TextureMgr* GetInstance(void);

	int  Init(IDirect3DDevice9* pDevice);
	void UnInit(void);

	int LoadTexture(const LPCSTR cpFilePath, int& nReference);
	int UnLoadTexture(const LPCSTR cpFilePath);
	int UnLoadTexture(int nID);

	int GetTexture(int nID, IDirect3DTexture9** ppOutput);

	/*
	int GetTexture(const LPCSTR cpFilePath, IDirect3DTexture9** ppOutput);
	int GetTexture(const LPCSTR cpFilePath, int& nOutput);
	*/

private:
	TextureMgr(void);
	~TextureMgr(void);

	struct TextureRef
	{
		IDirect3DTexture9* pTexture;
		int nCount;
	};

	typedef std::vector<TextureRef>		TextureRefVector;
	typedef std::map<std::string, int>	TextureIDMap;
	typedef std::list<int>				FreeIDList;

	TextureIDMap		m_FilePathMap;
	TextureRefVector	m_vTextureRef;
	FreeIDList			m_FreeIDList;
	IDirect3DDevice9*	m_pIDirect3DDevice;
};
