//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   EngineDefine.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef ENGINE_DEF
#define ENGINE_DEF

/*
enum TEXTURE_INDEX
{
	TEXTURE_INDEX_INVALD = 0,
	TEXTURE_INDEX_BEGIN,
	//TEXTURE_INDEX_BOX_FONT = TEXTURE_INDEX_BEGIN, 
	//TEXTURE_INDEX_BOX_BACK, 
	//TEXTURE_INDEX_BOX_LEFT, 
	//TEXTURE_INDEX_BOX_RIGHT,
	//TEXTURE_INDEX_BOX_TOP,
	//TEXTURE_INDEX_LAWN,
	//TEXTURE_INDEX_TREE,
	TEXTURE_INDEX_PARTICLE,
	TEXTURE_INDEX_END,
	TEXTURE_INDEX_SIZE = TEXTURE_INDEX_END,
};
*/
enum CameraType
{ 
	CAMERA_TYPE_INVALD,
	CAMERA_TYPE_BEGIN,
	CAMERA_TYPE_LANDOBJECT = CAMERA_TYPE_BEGIN, 
	CAMERA_TYPE_AIRCRAFT,
	CAMERA_TYPE_END,
};

#endif //ENGINE_DEF