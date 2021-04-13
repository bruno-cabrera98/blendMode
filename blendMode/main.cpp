#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <vector>

#include <iostream>

class vec3 {
public:
	enum class vecType {
		V_COLOR,
		V_POS,
		V_NORMAL
	};

	vec3(float x, float y, float z, vecType type)
		: x(x), y(y), z(z), type(type) {}

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	vecType getType() { return type; }

	void setX(float x) { x = x; }
	void setY(float y) { y = y; }
	void setZ(float z) { z = z; }
	void setType(vecType type) { type = type; }

private:
	float x, y, z;
	vecType type;
};

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "No se pudo ejecutar SDL\n";
		exit(1);
	}

	SDL_Window* win = SDL_CreateWindow("blendMode",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext contex = SDL_GL_CreateContext(win);

	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, 640 / 480.f, 0.1, 100);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<vec3> colores = {
		vec3(0,0,0,vec3::vecType::V_COLOR),
		vec3(1,0,0,vec3::vecType::V_COLOR),
		vec3(0,1,0,vec3::vecType::V_COLOR),
		vec3(1,1,0,vec3::vecType::V_COLOR),
		vec3(0,0,1,vec3::vecType::V_COLOR),
		vec3(1,0,1,vec3::vecType::V_COLOR),
		vec3(0,1,1,vec3::vecType::V_COLOR),
		vec3(1,1,1,vec3::vecType::V_COLOR),
	};

	const float width = 2/8.f;

	float alpha = 0;
	double rate = 1.0 / 2500; // Alpha por milisegundo 
	double delta = 0;
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last;

	do {
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

		last = now;
		now = SDL_GetPerformanceCounter();

		delta = ((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());
		alpha = (alpha + rate * delta);
		if (alpha > 1 || alpha < 0) {
			rate *= -1.0;
		}

		float startPos = -1;

		for (vec3 col : colores) {
			// Horizontal
			glBegin(GL_QUADS);
			glColor4f(col.getX(), col.getY(), col.getZ(), 1);
			glVertex3f(-1, startPos * -1, -.01);
			glVertex3f(-1, (startPos * -1) - 2 / 8.f, -.01);
			glVertex3f(1, (startPos * -1) - 2 / 8.f, -.01);
			glVertex3f(1, startPos * -1, -.01);
			glEnd();
			startPos += 2 / 8.f;
		}
		startPos = -1;
		for (vec3 col : colores) {
			// Vertical
			glBegin(GL_QUADS);
			glColor4f(col.getX(), col.getY(), col.getZ(), alpha);
			glVertex3f(startPos, 1, 0);
			glVertex3f(startPos, -1, 0);
			glVertex3f(startPos + 2 / 8.f, -1, 0);
			glVertex3f(startPos + 2 / 8.f, 1, 0);
			glEnd();
			startPos += 2 / 8.f;
		}
		SDL_GL_SwapWindow(win);
	} while (true);


	return 0;
}