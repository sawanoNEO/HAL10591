#pragma once
#include <memory>
#include <unordered_map>
#include <vector>
#include <SimpleMath.h>
#include <list>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#include <d3d11.h>


#pragma comment (lib, "assimp-vc142-mtd.lib")

enum SceneType
{
	TypeModel,TypeAnimation
};

class AiSceneSmartPtr
{
private:
	static std::unordered_map<std::string, const aiScene*> m_LoadedScene;//ロード済のaiSceneとそのモデルの名前(ファイルパス)
	static std::unordered_map<std::string, int> m_UseCount;//同じ名前のモデルが何度ロードされたか
	std::list<std::string> m_UseSceneName;//使用しているaiScene集(FileName)
public:
	const aiScene* AddScene(const char* FileName,SceneType _type);
	void UnInit();
};

