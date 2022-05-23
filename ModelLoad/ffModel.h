#ifndef MODEL_ffMODEL_H
#define MODEL_ffMODEL_H
#include"Base.h"
#include"Shader.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffImage.h"
#include"ffMesh.h"
#include "ffBone.h"


#define MAX_BONE_WEIGHTS 4
namespace FF
{
	class ffModel
	{
	public:
        ffModel(const char * _path)
		{
			loadModel(_path);
		}
		void draw(Shader& _shader);
	private:
		std::vector<ffMesh>			m_meshVec;
		std::string					m_dir;

        /*
         * ----骨骼动画开始@！！！！
         */
        std::map<std::string,ffBoneInfo> m_boneInfoMap;
        uint m_boneCounter = 0;

        void loadBoneWeightForVertices(std::vector<ffVertex>& _vertexArr,aiMesh* _pMesh,const aiScene* _pScene);
        void setVertexBoneData(ffVertex& _vertex,int _boneID,float weight);

        /*----骨骼动画结束------*/
		void loadModel(std::string _path);
		void processNode(aiNode * _node , const aiScene * _scene);
		ffMesh processMesh(aiMesh* _mesh, const aiScene* _scene);
        //用于加载纹理的函数
		std::vector<ffTexture>	loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene);
		
	};

	class ffTextureMananger//单例类
	{
	public:
		void SINGLE_OVER() {}
		uint createTexture(std::string _path);
		uint createTexture(std::string _path , std::string _dir);
		uint createTextureFromMemory(std::string _path, unsigned char* _data, uint _width, uint _height);
	private:
		SINGLE_INSTANCE(ffTextureMananger)
		ffTextureMananger() {}
		uint makeGLTexture(ffImage* _pImage);

		std::map<std::string, uint>	m_texMap;//string为读取路径，uint为纹理的表示
	};
}

#endif