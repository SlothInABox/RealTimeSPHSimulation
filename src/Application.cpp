#include <GLFW/glfw3.h>
#include <iostream>
#include "Particle.h"
#include "Vector2D.h"
#include "Simulation.h"
#include "LibConstants.h"

GLFWwindow* InitWindow()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return nullptr;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WindowParameters::WINDOW_WIDTH, WindowParameters::WINDOW_HEIGHT, "SPH Simulation", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make windows context current */
	glfwMakeContextCurrent(window);

	glClearColor(0.9f, 0.9f, 0.9f, 1);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(SolverParameters::KERNEL_RADIUS / 2.0f);
	glMatrixMode(GL_PROJECTION);

	return window;
}

int main(int argc, char** argv)
{
	GLFWwindow* window = InitWindow();

	Simulation simulation;

	/* Loop until user closes window */
	while (!glfwWindowShouldClose(window))
	{
		/* Update simulation */
		simulation.Update();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(0, WindowParameters::WINDOW_WIDTH, 0, WindowParameters::WINDOW_HEIGHT, 0, 1);

		glColor4f(0.2f, 0.6f, 1.f, 1);
		glBegin(GL_POINTS);
		/* Particle positions */
		for (const Particle& particle : simulation.particles)
		{
			glVertex2f(particle.position.x, particle.position.y);
		}
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}