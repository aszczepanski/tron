#ifndef TEXTURED_MODEL_3D_H
#define TEXTURED_MODEL_3D_H

#include <string>
#include <map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <GL/gl.h>

class TexturedModel3D
{
public:
	TexturedModel3D(const std::string& basePath, const std::string& fileName);
	int loadGLTextures();
	
	void draw();
private:
	bool import3DFromFile(const std::string& pFile);
	void applyMaterial(const aiMaterial* mtl);
	void recursiveRender(const aiScene* sc, const aiNode* md);

	const std::string basePath, fileName;

	const aiScene* scene;
	std::map<std::string, GLuint*> textureIdMap;
	GLuint* textureIds;

	Assimp::Importer importer;
};

#endif // TEXTURED_MODEL_3D_H
