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
	static std::unordered_map<std::string, const aiScene*> m_LoadedScene;//���[�h�ς�aiScene�Ƃ��̃��f���̖��O(�t�@�C���p�X)
	static std::unordered_map<std::string, int> m_UseCount;//�������O�̃��f�������x���[�h���ꂽ��
	std::list<std::string> m_UseSceneName;//�g�p���Ă���aiScene�W(FileName)
	static int m_DataNum;//�i�[���ꂽ�f�[�^�̐�
public:
	const aiScene* AddScene(const char* FileName,SceneType _type);
	void UnInit();
	static void UnInitAll();//�V�[���̉�������̍ۂɌĂяo���Bm_LoadedScene���������
};

