#include "Scene.h"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void Scene::Add(Model* model, Shader* shader, const glm::mat4& transform) 
{
	objects.push_back({ model, shader, transform });
}

void Scene::Draw()
{
	float timeValue = glfwGetTime();

	for (auto& obj : objects)
	{
		obj.shader->use();
		obj.shader->SetMatrix4("modelMatrix", obj.transform);
		obj.shader->SetFloat("u_Time", timeValue);
		obj.model->Draw();
	}
}

void Scene::Update(float deltaTime)
{
	if (objects.size() > 1)							// máme alespoò 2 objekty?
	{
		float speed = 0.2f;
		float deltaAngle = speed * deltaTime;		// deltaTime = èasový rozdíl mezi snímky
		float orbitSpeed = 0.6f;					// kombinovaná rotace - obíhání
		float selfSpeed = 3.0f;						// kombinovaný rotace - vlastní rotace
		
		SceneObject& monkey = objects[1];			// uložíme si odkaz na opici do promìnné, v [] index 
		
		//monkey.transform = glm::rotate(				// vytvoø novou rotaèní matici, která objekt otoèí o time kolem osy Y
		//	glm::mat4(1.0f),						// vytvoø jednotkovou matici(1.0f) - speciální matice, 
													//		která øíká "nedìlej nic - žádná zmìna" = je to startovací matice
		//	time,									// sekundy od startu -> plynulá animace
		//	glm::vec3(0, 1, 0)
		//) * monkey.transform;						// aplikuj to na opici (vynásobí matici pøedchozí transformací opice)
		

		float rotationSpeed = 1.5f;					// mùžeš libovolnì zmìnit rychlost 

		// ROTACE KOLEM VLASTNÍ OSY : 
		monkey.transform = glm::rotate(
			monkey.transform,						// rotujeme pøedchozí stav 
			deltaTime * rotationSpeed,				// deltaTime = rovnomìrná rychlost 
			glm::vec3(0, 1, 0)
		);

		//ROTACE KOLEM STØEDU: 
		//monkey.transform = glm::rotate(
		//	glm::mat4(1.0f), 
		//	deltaTime * rotationSpeed, 
		//	glm::vec3(0, 1, 0))
		//	* 
		// monkey.transform;

		// KOMBINOVANÁ ROTACE
		// Nejdøív obìhnutí kolem støedu
		//monkey.transform =
		//	glm::rotate(glm::mat4(1.0f), deltaTime * orbitSpeed, glm::vec3(0, 1, 0))
		//	* monkey.transform;

		// Pak vlastní rotace kolem vlastní osy
		//monkey.transform =
		/*	glm::rotate(monkey.transform, deltaTime * selfSpeed, glm::vec3(0, 1, 0)); */

	}
}