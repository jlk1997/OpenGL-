#include"ffModel.h"

namespace FF
{
	void ffModel::loadModel(std::string _path)
	{
		Assimp::Importer _importer;//建立Assimp类
        //将传入的模型三角形化，并将V坐标进行上下翻转
		const aiScene* _scene = _importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
        //看读入的scene是否为空，Flag是否完整，是否存在根结点
		if (!_scene || _scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !_scene->mRootNode)
		{
			std::cout << "model read fail!" << std::endl;
			return;
		}
        //找到path的最后一个/进行截取，找到路径信息
		m_dir = _path.substr(0, _path.find_last_of('/'));
		processNode(_scene->mRootNode, _scene);
	}

    //processNode是递归调用的，最终会把传入的节点的所有字节点进行遍历并存入vector中
	void ffModel::processNode(aiNode* _node, const aiScene* _scene)
	{
		for (uint i = 0; i < _node->mNumMeshes; i++)
		{
            //aiMesh是Assimp的网格结构，从节点中获取mesh的id再去scene中得到mesh数据
			aiMesh* _mesh = _scene->mMeshes[_node->mMeshes[i]];
            //得到mesh数据后进行处理，processMesh函数返回ffMesh，我们将它存入m_meshVec vector数组中
			m_meshVec.push_back(processMesh(_mesh, _scene ));
		}

		for (uint i = 0; i < _node->mNumChildren; i++)
		{
			processNode(_node->mChildren[i], _scene);//递归子节点
		}
	}
    //解析aiMesh
	ffMesh ffModel::processMesh(aiMesh* _mesh, const aiScene* _scene )
	{
        //构建一个mesh所必须的三个vector
		std::vector<ffVertex>	_vertexVec;
		std::vector<uint>		_indexVec;
		std::vector<ffTexture>	_texVec;

		for (uint i = 0; i < _mesh->mNumVertices; i++)
		{
			ffVertex   _vertex;
			
			//读取顶点的位置信息
			glm::vec3 _pos;
			_pos.x = _mesh->mVertices[i].x;
			_pos.y = _mesh->mVertices[i].y;
			_pos.z = _mesh->mVertices[i].z;
			_vertex.m_pos = _pos;

			//读取法线
			glm::vec3 _normal;
			_normal.x = _mesh->mNormals[i].x;
			_normal.y = _mesh->mNormals[i].y;
			_normal.z = _mesh->mNormals[i].z;
			_vertex.m_normal = _normal;

			//读取纹理信息
            //因为一个mesh可能有多个纹理，这里我们默认取第0个纹理应用
			if (_mesh->mTextureCoords[0])
			{
				glm::vec2 _texCoord;
				_texCoord.x = _mesh->mTextureCoords[0][i].x;//u,第0号的第i个
				_texCoord.y = _mesh->mTextureCoords[0][i].y;//v
				_vertex.m_texCoord = _texCoord;
			}
            //顶点构造完毕后插入vertexVec
			_vertexVec.push_back(_vertex);
		}

		//构造索引，以face为单位进行存取，face就是三角形
		for (uint i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace	_face = _mesh->mFaces[i];

			for (uint j = 0; j < _face.mNumIndices; j++)
			{
				_indexVec.push_back(_face.mIndices[j]);
			}
		}

		//构造纹理材质信息
		if (_mesh->mMaterialIndex >= 0)//判断mesh中是否存在材质
		{
			aiMaterial* _mat = _scene->mMaterials[_mesh->mMaterialIndex];

			std::vector<ffTexture> _diffuseVec = loadMaterialTextures(_mat, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE_STR , _scene);
			_texVec.insert(_texVec.end(), _diffuseVec.begin(), _diffuseVec.end());

			std::vector<ffTexture> _specularVec = loadMaterialTextures(_mat, aiTextureType_SPECULAR, TEXTURE_SPECULAR_STR, _scene);
			_texVec.insert(_texVec.end(), _specularVec.begin(), _specularVec.end());
		}


		return ffMesh(_vertexVec, _indexVec, _texVec);
	}

	std::vector<ffTexture>	ffModel::loadMaterialTextures(aiMaterial* _mat, aiTextureType _type, std::string _typeName, const aiScene* _scene)
	{
		std::vector<ffTexture> _texVec;

		for (uint i = 0; i < _mat->GetTextureCount(_type); i++)
		{
			ffTexture _tex;

			aiString _path;
			_mat->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, i), _path);
			//若材质已经装配在内存中了，就从内存中去读材质
			if (const aiTexture* _aiTexture = _scene->GetEmbeddedTexture(_path.C_Str()))
			{
				_tex.m_id = ffTextureMananger::getInstance()->createTextureFromMemory(_path.C_Str() ,
                                                                                      reinterpret_cast<unsigned char*>(_aiTexture->pcData) ,
                                                                                      _aiTexture->mWidth , _aiTexture->mHeight);
				_tex.m_path = _path.C_Str();
				_tex.m_type = _typeName;
			}
			else
			{
                //未装配的，则创建材质
				_tex.m_id = ffTextureMananger::getInstance()->createTexture(_path.C_Str(), m_dir);
				_tex.m_path = _path.C_Str();
				_tex.m_type = _typeName;
			}
			_texVec.push_back(_tex);
		}
		return _texVec;
	}

	void ffModel::draw(Shader& _shader)
	{
		for (uint i = 0; i < m_meshVec.size(); i++)
		{
			m_meshVec[i].draw(_shader);
		}
	}

	

	SINGLE_INSTANCE_SET(ffTextureMananger)

	uint ffTextureMananger::createTexture(std::string _path)
	{
		std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
		if (_it != m_texMap.end())
		{
			return _it->second;
		}

		ffImage* _image = ffImage::readFromFile(_path.c_str());

		uint _texID = makeGLTexture(_image);

		delete _image;
		m_texMap[_path] = _texID;
		return _texID;
	}

	uint ffTextureMananger::createTextureFromMemory(std::string _path, unsigned char* _data, uint _width, uint _height)
	{
		std::map<std::string, uint>::iterator _it = m_texMap.find(_path);
		if (_it != m_texMap.end())
		{
			return _it->second;
		}
		ffImage* _image = ffImage::readFromMemory(_data, _width, _height);

		uint _texID = makeGLTexture(_image);

		delete _image;
		m_texMap[_path] = _texID;
		return _texID;
	}

	uint ffTextureMananger::makeGLTexture(ffImage* _pImage)//拿到一张图片
	{
		if (!_pImage)
		{
			return 0;
		}
		uint _texID = 0;
		glGenTextures(1, &_texID);
		glBindTexture(GL_TEXTURE_2D, _texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pImage->getWidth(), _pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _pImage->getData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return _texID;
	}

	uint ffTextureMananger::createTexture(std::string _path, std::string _dir)
	{
		return createTexture(_dir + '/' + _path);
	}
}
