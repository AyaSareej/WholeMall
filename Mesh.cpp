//#include "Mesh.h"
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "Shader.h"
//#include <glm/gtc/type_ptr.hpp>
//
//#include"VAO.h"
//#include"EBO.h"
//
//
//
//Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
//{
//	Mesh::vertices = vertices;
//	Mesh::indices = indices;
//	Mesh::textures = textures;
//
//	
//	setupMesh();
//}
//
//void Mesh::setupMesh() {
//		glGenVertexArrays(1, &VAO);
//		glGenBuffers(1, &VBO);
//		glGenBuffers(1, &EBO);
//
//		glBindVertexArray(VAO);
//		glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
//			&indices[0], GL_STATIC_DRAW);
//
//		// vertex positions
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//		// vertex normals
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
//		// vertex texture coords
//		glEnableVertexAttribArray(2);
//		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
//
//		glBindVertexArray(0);
//}
//
//void Mesh::Draw
//(
//	Shader& shader, 
//	Camera& camera,
//	glm::mat4 matrix,
//	glm::vec3 translation, 
//	glm::quat rotation, 
//	glm::vec3 scale
//)
//{
//	// Bind shader to be able to access uniforms
//	shader.Use();
//
//	// Keep track of how many of each type of textures we have
//	unsigned int numDiffuse = 1;
//	unsigned int numSpecular = 1;
//
//	for (unsigned int i = 0; i < textures.size(); i++)
//	{
//		//std::cout << "Binding texture " << textures[i].path << " to unit " << i << std::endl;
//		glActiveTexture(GL_TEXTURE0 + i);
//		std::string num;
//		std::string type = textures[i].type;
//		if (type == "texture_diffuse")
//		{
//			num = std::to_string(numDiffuse++);
//		}
//		else if (type == "texture_specular")
//		{
//			num = std::to_string(numSpecular++);
//		}
//		shader.setInt(("material." + type + num).c_str(), i);		
//		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
//	}
//	glActiveTexture(GL_TEXTURE0);    // Activate the appropriate texture unit
//	glBindVertexArray(VAO);
//
//	// Take care of the camera Matrix
//	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
//	//camera.Matrix(shader, "camMatrix");
//
//	// Initialize matrices
//	glm::mat4 trans = glm::mat4(1.0f);
//	glm::mat4 rot = glm::mat4(1.0f);
//	glm::mat4 sca = glm::mat4(1.0f);
//
//	// Transform the matrices to their correct form
//	trans = glm::translate(trans, translation);
//	//rot = glm::mat4_cast(rotation);
//	sca = glm::scale(sca, scale);
//
//	 //Push the matrices to the vertex shader
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
//
//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//	// Draw the actual mesh
//	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//}