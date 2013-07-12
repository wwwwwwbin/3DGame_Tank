//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TextureMgr.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-04 20:13
//  Comment     :   Container of Texture
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TextureMgr.h"

TextureMgr::TextureMgr(void)
: m_pIDirect3DDevice(NULL)
{
	
}

TextureMgr::~TextureMgr(void)
{
}

int TextureMgr::LoadTexture( const LPCSTR cpFilePath, int& nReference )
{
	int 	nResult		= FALSE;
	int 	nRetCode	= FALSE;
	bool	bExist		= FALSE;
	int		nID			= -1;
	HRESULT hr			= E_FAIL;

	TextureIDMap::iterator itFind;

	LOG_FAILD_JUMP(m_pIDirect3DDevice);
	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);

	itFind = m_FilePathMap.find(std::string(cpFilePath));

	if (itFind != m_FilePathMap.end())
	{
		nID = itFind->second;
		TextureRef& texRef = m_vTextureRef[nID];
		++texRef.nCount;
	}
	else
	{
		IDirect3DTexture9* pTexture = NULL;

		hr = D3DXCreateTextureFromFile(m_pIDirect3DDevice, cpFilePath, &pTexture);
		LOG_FAILD_JUMP(hr == D3D_OK);

		if (m_FreeIDList.empty())
		{
			TextureRef TexRef;
			TexRef.pTexture = pTexture;
			TexRef.nCount = 1;

			nID = (int)m_vTextureRef.size();
			m_vTextureRef.push_back(TexRef);
		}
		else
		{
			nID = m_FreeIDList.front();
			m_FreeIDList.pop_front();

			TextureRef& rTexRef = m_vTextureRef[nID];
			rTexRef.pTexture = pTexture;
			rTexRef.nCount = 1;
		}

		m_FilePathMap[cpFilePath] = nID;
	}

	nResult = TRUE;
	nReference = nID;
	assert(nID != -1);
Exit0:
	return nResult;
}
/*
int TextureMgr::GetTexture( const LPCSTR cpFilePath, IDirect3DTexture9** ppOutput )
{
	int nResult  = FALSE;

	TextureIDMap::iterator itFind;

	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);
	LOG_FAILD_JUMP(ppOutput);

	itFind = m_FilePathMap.find(cpFilePath);

	if (itFind != m_FilePathMap.end())
	{
		int nID = itFind->second;
		TextureRef& texRef = m_vTextureRef[nID];
		*ppOutput = texRef.pTexture;
	}	

	nResult = TRUE;
Exit0:
	return nResult;
}

int TextureMgr::GetTexture( const LPCSTR cpFilePath, int& nOutput )
{
	int nResult  = FALSE;

	TextureIDMap::iterator itFind;

	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);

	itFind = m_FilePathMap.find(cpFilePath);

	if (itFind != m_FilePathMap.end())
	{
		nOutput = itFind->second;
	}	

	nResult = TRUE;
Exit0:
	return nResult;
}
*/
int TextureMgr::GetTexture( int nID, IDirect3DTexture9** ppOutput )
{
	int 				nResult  = FALSE;
	int 				nRetCode = FALSE;
	IDirect3DTexture9*	pTexture = NULL;

	LOG_FAILD_JUMP(nID >= 0);
	LOG_FAILD_JUMP(ppOutput);
	
	if (nID < m_vTextureRef.size())
	{
		TextureRef& texRef = m_vTextureRef[nID];
		if (texRef.nCount > 0 && texRef.pTexture)
		{
			pTexture = texRef.pTexture;
		}
	}


	nResult = TRUE;
	*ppOutput = pTexture;
Exit0:
	return nResult;
}

int TextureMgr::UnLoadTexture( const LPCSTR cpFilePath )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	TextureIDMap::iterator itFind;

	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);

	itFind = m_FilePathMap.find(cpFilePath);

	if (itFind != m_FilePathMap.end())
	{
		int nID = itFind->second;
		TextureRef& texRef = m_vTextureRef[nID];
		
		--texRef.nCount;
		
		if (texRef.nCount <= 0)
		{
			SafeRelease(texRef.pTexture);
			m_FreeIDList.push_back(nID);
		}
	}

	nResult = TRUE;
Exit0:
	return nResult;
}

TextureMgr* TextureMgr::GetInstance( void )
{
	static TextureMgr* pInstance = NULL;

	if (pInstance == NULL)
	{
		pInstance = new TextureMgr();
	}

	return pInstance;
}

int TextureMgr::Init( IDirect3DDevice9* pDevice )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	LOG_FAILD_JUMP(pDevice);

	m_pIDirect3DDevice = pDevice;
		
	m_FilePathMap.clear();
	m_FreeIDList.clear();

	m_vTextureRef.resize(8);
	for (int i = 0; i < 8; ++i)
	{
		m_FreeIDList.push_back(i);
	}	

	nResult = TRUE;
Exit0:
	return nResult;
}

void TextureMgr::UnInit( void )
{
	TextureRefVector::iterator itEnd = m_vTextureRef.end();

	for (TextureRefVector::iterator it = m_vTextureRef.begin(); it != itEnd; ++it)
	{
		TextureRef& ref = *it;
		SafeRelease(ref.pTexture);
	}

	m_vTextureRef.clear();
	m_FilePathMap.clear();
	m_FreeIDList.clear();

	m_pIDirect3DDevice = NULL;
}
