#pragma once

#include <list>
#include <map>
#include <string>

class TextureMgr
{
public:

	TextureMgr(void);
	~TextureMgr(void);

	int Init(void);


	int LoadTexture(IDirect3DDevice9* pDevice, const LPCSTR cpFilePath, int& nReference);
	int UnLoadTexture(const LPCSTR cpFilePath);

	int GetTexture(const LPCSTR cpFilePath, IDirect3DTexture9** ppOutput);

private:

	struct TextureRef
	{
		IDirect3DTexture9* pTexture;
		int nCount;
	};

	typedef std::map<std::string, TextureRef>	TextureRefMap;

	TextureRefMap	m_FilePathMap;
};
