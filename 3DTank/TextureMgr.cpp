//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   TextureMgr.cpp
//  Version     :   1.0
//  Creater     :   weibin Huang
//  Date        :   2013-07-04 20:13
//  Comment     :   Container of Texture
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TextureMgr.h"

TextureMgr::TextureMgr(void)
{
	m_FilePathMap.clear();
}

TextureMgr::~TextureMgr(void)
{
	TextureRefMap::iterator itEnd = m_FilePathMap.end();

	for (TextureRefMap::iterator it = m_FilePathMap.begin(); it != itEnd; ++it)
	{
		TextureRef& ref = it->second;
		SafeRelease(ref.pTexture);
	}

	m_FilePathMap.clear();
}

int TextureMgr::LoadTexture( IDirect3DDevice9* pDevice, const LPCSTR cpFilePath, int& nReference )
{
	int 	nResult		= false;
	int 	nRetCode	= false;
	bool	bExist		= false;
	HRESULT hr			= E_FAIL;

	TextureRefMap::iterator itFind;

	LOG_FAILD_JUMP(pDevice);
	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);

	itFind = m_FilePathMap.find(std::string(cpFilePath));

	if (itFind != m_FilePathMap.end())
	{
		TextureRef& texRef = itFind->second;
		++texRef.nCount;
	}
	else
	{
		IDirect3DTexture9* pTexture = NULL;

		hr = D3DXCreateTextureFromFile(pDevice, cpFilePath, &pTexture);
		LOG_FAILD_JUMP(hr == D3D_OK);

		TextureRef ref;
		ref.pTexture = pTexture;
		ref.nCount = 1;

		m_FilePathMap[cpFilePath] = ref;
	}

	nResult = true;
Exit0:
	return nResult;
}

int TextureMgr::GetTexture( const LPCSTR cpFilePath, IDirect3DTexture9** ppOutput )
{
	int nResult  = false;
	int nRetCode = false;

	TextureRefMap::iterator itFind;

	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);
	LOG_FAILD_JUMP(ppOutput);

	itFind = m_FilePathMap.find(cpFilePath);

	if (itFind != m_FilePathMap.end())
	{
		TextureRef& ref = itFind->second;
		*ppOutput = ref.pTexture;
	}	

	nResult = true;
Exit0:
	return nResult;
}

int TextureMgr::UnLoadTexture( const LPCSTR cpFilePath )
{
	int nResult  = false;
	int nRetCode = false;

	TextureRefMap::iterator itFind;

	LOG_FAILD_JUMP(cpFilePath);
	LOG_FAILD_JUMP(cpFilePath[0]);

	itFind = m_FilePathMap.find(cpFilePath);

	if (itFind != m_FilePathMap.end())
	{
		TextureRef& ref = itFind->second;
		--ref.nCount;

		if (ref.nCount <= 0)
		{
			SafeRelease(ref.pTexture);
			m_FilePathMap.erase(itFind);
		}
	}

	nResult = true;
Exit0:
	return nResult;
}
