#pragma once
#include "Model.h"
#include "Shader.h"
#include <vector>
class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void Add(Model* model, Shader* shader, const glm::mat4& transform);
	void Draw();
	void Update(float deltaTime);

private:
	struct SceneObject
	{
		Model* model;
		Shader* shader;
		glm::mat4 transform;
	};

	std::vector<SceneObject> objects;

};

