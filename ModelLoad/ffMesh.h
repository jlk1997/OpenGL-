#ifndef MODEL_ffMESH_H
#define MODEL_ffMESH_H
#include"Base.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"Shader.h"

namespace FF
{
	struct ffVertex
	{
		glm::vec3	m_pos;
		glm::vec3	m_normal;
		glm::vec2	m_texCoord;

        float m_boneWeightArr[4] = {0.0f,0.0f,0.0f,0.0f};
        int m_boneIdArr[4] = {-1,-1,-1,-1};

		ffVertex()//初始化Vertex的参数信息
		{
            m_pos = glm::vec3(0.0f);
			m_normal = glm::vec3(0.0f);
			m_texCoord = glm::vec2(0.0f);
		}
		~ffVertex()
		{

		}
	};



#define TEXTURE_DIFFUSE_STR "texture_diffuse"
#define TEXTURE_SPECULAR_STR "texture_specular"
	struct ffTexture
	{
		uint			m_id;
		std::string		m_type;//type对应两个宏
		std::string		m_path;
	};

#define SHADER_POSITION_ANCHOR		0
#define SHADER_NORMAL_ANCHOR		1
#define SHADER_TEXCOORD_ANCHOR		2
#define SHADER_BONE_WEIGHT_ANCHOR   3
#define SHADER_BONE_ID_ANCHOR       4

	class ffMesh
	{
	public:
		ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec, std::vector<ffTexture> _texVec);
		void draw(Shader& _shader);

		std::vector<ffVertex>	m_vertexVec;
		std::vector<uint>		m_indexVec;
		std::vector<ffTexture>	m_texVec;

		GLuint m_VAO;
		void setupMesh();
	};
}

#endif