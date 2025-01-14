#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H   

#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"

#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <wtypes.h>
#include <ctime>
#include <filesystem>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#define TAU (M_PI * 2.0)

using namespace std;

void main1() {
	float radius;
	int sectors, stacks;

	cin >> radius >> sectors >> stacks;

	float d = 2 * radius / stacks;
	float a = 2 * M_PI / (2 * sectors);

	float topPoint = radius, bottomPoint = -radius;

	const int points = 2 * sectors * stacks + 2;

	float X[1000];
	float Y[1000];
	float Z[1000];

	for (int i = 0, float z = radius - d; z > -radius; z -= d, i++) {
		Z[i] = z;

		for (int j = 1, float a = 2 * M_PI / (2 * sectors); j <= sectors * 2; a *= j, j++) {
			X[i] = radius * cosf(a);
			Y[i] = radius * sinf(a);
		}
	}

	for (int i = 0; i < points; i++) {
		cout << X[i] << '\t' << Y[i] << '\t' << Z[i] << endl;
	}
}