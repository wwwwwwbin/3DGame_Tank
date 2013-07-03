#include "StdAfx.h"
#include "TextureMgr.h"

TextureMgr::TextureMgr(void)
{
}

TextureMgr::~TextureMgr(void)
{
}

int TextureMgr::Init( void )
{
	int nResult  = false;
	int nRetCode = false;


	nResult = true;
Exit0:
	return nResult;
}

int TextureMgr::LoadTexture( IDirect3DDevice9* pDevice, const LPCSTR cpFilePath, int& nReference )
{
	int 	nResult		= false;
	int 	nRetCode	= false;
	bool	bExist		= false;
	HRESULT hr			= E_FAIL;

	TextureRefMap::iterator itFind;

	CHECK_FAILD_JUMP(pDevice);
	CHECK_FAILD_JUMP(cpFilePath);
	CHECK_FAILD_JUMP(cpFilePath[0]);

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
		CHECK_FAILD_JUMP(hr == D3D_OK);

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

	CHECK_FAILD_JUMP(cpFilePath);
	CHECK_FAILD_JUMP(cpFilePath[0]);
	CHECK_FAILD_JUMP(ppOutput);

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

	CHECK_FAILD_JUMP(cpFilePath);
	CHECK_FAILD_JUMP(cpFilePath[0]);

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
