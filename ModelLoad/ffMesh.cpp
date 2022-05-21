#include "ffMesh.h"

namespace FF
{
	ffMesh::ffMesh(std::vector<ffVertex> _vertexVec, std::vector<uint> _indexVec, std::vector<ffTexture> _texVec)
	{
		m_vertexVec = _vertexVec;//????????vector???????
		m_indexVec = _indexVec;
		m_texVec = _texVec;

		setupMesh();
	}
	void ffMesh::draw(Shader& _shader)
	{
        //???
		uint _diffuseN = 1;
		uint _specularN = 1;

		for (uint i = 0; i < m_texVec.size(); i++)
		{
            //????texture??????texture???
			glActiveTexture(GL_TEXTURE0 + i);

			//??shader???
			std::string _typeName = m_texVec[i].m_type;
			std::string _numStr;
			if (_typeName == TEXTURE_DIFFUSE_STR)
			{
				_numStr = std::to_string(_diffuseN++);
			}
			if (_typeName == TEXTURE_SPECULAR_STR)
			{
				_numStr = std::to_string(_specularN++);
			}
			_shader.setInt("myMaterial." + _typeName + _numStr, i);
            //myMaterial.texture_specular1 = i??????????

            //??texture????i?texture?id
			glBindTexture(GL_TEXTURE_2D, m_texVec[i].m_id);
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_indexVec.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	void ffMesh::setupMesh()
	{
		uint _VBO = 0;
		uint _EBO = 0;
    //??VBO?EBO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);
    //?VAO???VBO?EBO
		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    //?????VBO???VBO????
		glBufferData(GL_ARRAY_BUFFER, sizeof(ffVertex) * m_vertexVec.size(), &m_vertexVec[0], GL_STATIC_DRAW);
    //?GL_ARRAY_BUFFER????????????ffVertex??????vertex????*vertex????byte??????????????

		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_indexVec.size(), &m_indexVec[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(SHADER_POSITION_ANCHOR);//??shader??Position
		glVertexAttribPointer(SHADER_POSITION_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)0);
        //?????3?float????????????????????????????????

		glEnableVertexAttribArray(SHADER_NORMAL_ANCHOR);
		glVertexAttribPointer(SHADER_NORMAL_ANCHOR, 3, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)offsetof(ffVertex, m_normal));//??ffVertex???m_normal????

		glEnableVertexAttribArray(SHADER_TEXCOORD_ANCHOR);
		glVertexAttribPointer(SHADER_TEXCOORD_ANCHOR, 2, GL_FLOAT, GL_FALSE, sizeof(ffVertex), (void*)offsetof(ffVertex, m_texCoord));

		glBindVertexArray(0);
	}
}