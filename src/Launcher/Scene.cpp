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
	if (objects.size() > 1)							// m�me alespo� 2 objekty?
	{
		float speed = 0.2f;
		float deltaAngle = speed * deltaTime;		// deltaTime = �asov� rozd�l mezi sn�mky
		float orbitSpeed = 0.6f;					// kombinovan� rotace - ob�h�n�
		float selfSpeed = 3.0f;						// kombinovan� rotace - vlastn� rotace
		
		SceneObject& monkey = objects[1];			// ulo��me si odkaz na opici do prom�nn�, v [] index 
		
		//monkey.transform = glm::rotate(				// vytvo� novou rota�n� matici, kter� objekt oto�� o time kolem osy Y
		//	glm::mat4(1.0f),						// vytvo� jednotkovou matici(1.0f) - speci�ln� matice, 
													//		kter� ��k� "ned�lej nic - ��dn� zm�na" = je to startovac� matice
		//	time,									// sekundy od startu -> plynul� animace
		//	glm::vec3(0, 1, 0)
		//) * monkey.transform;						// aplikuj to na opici (vyn�sob� matici p�edchoz� transformac� opice)
		

		float rotationSpeed = 1.5f;					// m��e� libovoln� zm�nit rychlost 

		// ROTACE KOLEM VLASTN� OSY : 
		monkey.transform = glm::rotate(
			monkey.transform,						// rotujeme p�edchoz� stav 
			deltaTime * rotationSpeed,				// deltaTime = rovnom�rn� rychlost 
			glm::vec3(0, 1, 0)
		);

		//ROTACE KOLEM ST�EDU: 
		//monkey.transform = glm::rotate(
		//	glm::mat4(1.0f), 
		//	deltaTime * rotationSpeed, 
		//	glm::vec3(0, 1, 0))
		//	* 
		// monkey.transform;

		// KOMBINOVAN� ROTACE
		// Nejd��v ob�hnut� kolem st�edu
		//monkey.transform =
		//	glm::rotate(glm::mat4(1.0f), deltaTime * orbitSpeed, glm::vec3(0, 1, 0))
		//	* monkey.transform;

		// Pak vlastn� rotace kolem vlastn� osy
		//monkey.transform =
		/*	glm::rotate(monkey.transform, deltaTime * selfSpeed, glm::vec3(0, 1, 0)); */

	}
}