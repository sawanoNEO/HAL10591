#include "AiSceneSmartPtr.h"
#include "renderer.h"

#include <fstream>
#include <assert.h>
#include "utility.h"
#include "main.h"

using namespace DirectX::SimpleMath;

std::unordered_map<std::string, const aiScene*>AiSceneSmartPtr::m_LoadedScene;
std::unordered_map<std::string, int>AiSceneSmartPtr::m_UseCount;

const aiScene* AiSceneSmartPtr::AddScene(const char* FileName, SceneType _type)
{
	const std::string modelPath(FileName);

	std::ofstream outputFile("asset\\editer\\ModelData.csv");

	//ロード済でなければロードし、ロード済であればそのaiSceneを返す
	if (m_LoadedScene.find(FileName) == m_LoadedScene.end())
	{
		switch (_type)
		{
		case TypeModel:
			m_LoadedScene[FileName] = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
			break;
		case TypeAnimation:
			m_LoadedScene[FileName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
			break;
		default:
			break;
		}
		assert(m_LoadedScene[FileName]);
	
	}
	m_UseCount[FileName]++;
	m_UseSceneName.push_back(FileName);
	return m_LoadedScene[FileName];
}

void AiSceneSmartPtr::UnInit()
{
	for (auto itr : m_UseSceneName)
	{
		if (m_UseCount[itr] == 1)
		{
			aiReleaseImport(m_LoadedScene[itr]);
		}
		m_UseCount[itr]--;
	}
}
