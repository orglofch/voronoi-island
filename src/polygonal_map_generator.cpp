#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <cstdlib>
#include <ctime>

#include "algebra.hpp"
#include "gl_util.hpp"
#include "util.hpp"

#include "tilemap.hpp"

using namespace std;

struct SimulationState
{
	int win_width, win_height;

	TileMap *tile_map;
	Matrix4x4 map_matrix;

	Point3 camera_pos;

	bool mouse_pressed;
	Point2 mouse_pos;
};

Vector3 RotVec(const Vector2 &v_new, const Vector2 &v_old, float fDiameter) {
	Vector3 new_vec, old_vec;
	float length;

	new_vec.x = v_new.x * 2.0 / fDiameter;
	new_vec.y = v_new.y * 2.0 / fDiameter;
	new_vec.z = (1.0 - new_vec.x * new_vec.x - new_vec.y * new_vec.y);

	if (new_vec.z < 0.0) {
		length = sqrt(1.0 - new_vec.z);
		new_vec.z = 0.0;
		new_vec.x /= length;
		new_vec.y /= length;
	} else {
		new_vec.z = sqrt(new_vec.z);
	}

	old_vec.x = v_old.x * 2.0 / fDiameter;
	old_vec.y = v_old.y * 2.0 / fDiameter;
	old_vec.z = (1.0 - old_vec.x * old_vec.x - old_vec.y * old_vec.y);

	if (old_vec.z < 0.0) {
		length = sqrt(1.0 - old_vec.z);
		old_vec.z = 0.0;
		old_vec.x /= length;
		old_vec.y /= length;
	} else {
		old_vec.z = sqrt(old_vec.z);
	}

	return Vector3(old_vec.y * new_vec.z - new_vec.y * old_vec.z,
		old_vec.z * new_vec.x - new_vec.z * old_vec.x,
		old_vec.x * new_vec.y - new_vec.x * old_vec.y);
}

Matrix4x4 AxisRotMatrix(const Vector3 &v) {
	Vector3 v_new;
	float radians, inv_length;

	radians = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

	if (radians > -0.000001 && radians < 0.000001) {
		return Matrix4x4();
	}

	inv_length = 1 / radians;
	v_new = v * inv_length;

	double sin_alpha = sin(radians);
	double cos_alpha = cos(radians);
	double dT = 1 - cos_alpha;

	Matrix4x4 new_mat;

	new_mat[0][0] = cos_alpha + v_new.x * v_new.x * dT;
	new_mat[0][1] = v_new.x * v_new.y * dT + v_new.z * sin_alpha;
	new_mat[0][2] = v_new.x * v_new.z * dT - v_new.y * sin_alpha;
	new_mat[0][3] = 0;

	new_mat[1][0] = v_new.x * v_new.y * dT - sin_alpha * v_new.z;
	new_mat[1][1] = cos_alpha + v_new.y * v_new.y * dT;
	new_mat[1][2] = v_new.y * v_new.z * dT + sin_alpha * v_new.x;
	new_mat[1][3] = 0;

	new_mat[2][0] = v_new.z * v_new.x * dT + sin_alpha * v_new.y;
	new_mat[2][1] = v_new.z * v_new.y * dT - sin_alpha * v_new.x;
	new_mat[2][2] = cos_alpha + v_new.z * v_new.z * dT;
	new_mat[2][3] = 0;

	new_mat[3][0] = 0;
	new_mat[3][1] = 0;
	new_mat[3][2] = 0;
	new_mat[3][3] = 1;

	return new_mat;
}

void RotateCamera(SimulationState &state, Point2 &p1, const Point2 &p2) {
	float diameter = (state.win_width < state.win_height) ? state.win_width * 0.5f : state.win_height * 0.5f;
	Point2 center = Point2(state.win_width / 2, state.win_height / 2);

	Vector3 vec = RotVec(p1 - center, p2 - center, diameter);

	Matrix4x4 rot_mat = AxisRotMatrix(vec);

	state.map_matrix = rot_mat * state.map_matrix;
}

void error_callback(int error, const char *description) {
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
	SimulationState *state = (SimulationState*)glfwGetWindowUserPointer(window);
	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
	}
}

void cursor_position_callback(GLFWwindow *window, double x_pos, double y_pos) {
	SimulationState *state = (SimulationState*)glfwGetWindowUserPointer(window);

	Point2 new_mouse_pos(x_pos, y_pos);

	if (state->mouse_pressed) {
		RotateCamera(*state, state->mouse_pos, new_mouse_pos);
	}
	state->mouse_pos = new_mouse_pos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
	SimulationState *state = (SimulationState*)glfwGetWindowUserPointer(window);

	switch (button) {
		case GLFW_MOUSE_BUTTON_1:
			state->mouse_pressed = action == GLFW_PRESS;
			break;
	}
}

void mouse_scroll_callback(GLFWwindow *window, double x_offset, double y_offset) {
	SimulationState *state = (SimulationState*)glfwGetWindowUserPointer(window);

	state->camera_pos.z += y_offset * 50;
}

void UpdateAndRender(SimulationState &state, double dt) {
	state.tile_map->Step(dt);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(-state.camera_pos.x, -state.camera_pos.y, state.camera_pos.z);

	glTranslatef(state.tile_map->width() / 2, state.tile_map->height() / 2, 0);
	glMultMatrixd(state.map_matrix.transpose().begin());
	glTranslatef(-state.tile_map->width() / 2, -state.tile_map->height() / 2, 0);

	state.tile_map->RenderMap();

	GL_CHECK();
}

int main(int argc, char **argv) {
	srand(time(NULL));

	SimulationState state;
	state.win_height = 1080;
	state.win_width = 1080;
	state.mouse_pressed = false;

	state.tile_map = new VoronoiTileMap(Vector2(1500, 1500), 20000);
	state.tile_map->Generate();

	state.camera_pos.x = state.tile_map->width() / 2;
	state.camera_pos.y = state.tile_map->height() / 2;
	state.camera_pos.z = -1500;

	GLFWwindow *window;
	if (!glfwInit()) {
		LOG("Failed to initialize glfw\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(state.win_width, state.win_height, "FluidSim", NULL, NULL);
	if (!window) {
		LOG("Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(window, &state);

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		LOG("Failed to initialize glew\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, state.win_width, state.win_height);
	glSetPerspectiveProjection(state.win_width, state.win_height, 60, 0.1, 10000);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	double time = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		UpdateAndRender(state, glfwGetTime() - time);

		glfwSwapBuffers(window);

		while (glfwGetTime() - time < 1.0f / 60) {};
		time = glfwGetTime();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}