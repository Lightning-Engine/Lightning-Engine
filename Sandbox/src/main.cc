#include "li/window.hh"
#include "li/application.hh"
#include "li/gl.h"
#include <iostream>

class sandbox : public li::windowed_application {
public:
	int width;
	int height;
	float xp = 0;
	float yp = 0;
	GLuint vbo, ebo, vs_id, fs_id, program_id;

	GLfloat vertices[12] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, +0.5f, 0.0f,
		+0.5f, +0.5f, 0.0f,
		+0.5f, -0.5f, 0.0f,
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	const char *vertex_shader = "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
void main() {\n\
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
}\n";

	const char *fragment_shader = "#version 330 core\n\
out vec4 FragColor;\n\
void main() {\n\
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
}\n";

	sandbox() : li::windowed_application(640, 480), width(640), height(480) { }

	bool init() override {
		if (!windowed_application::init())
			return false;
		const li::gl &gl = get_window().get_gl();
		li::window &window = get_window();
		window.dispatcher.add_listener<li::window_close_event>(*this);
		window.dispatcher.add_listener<li::window_resize_event>(*this);
		window.dispatcher.add_listener<li::motion_notify_event>(*this);
		
		vs_id = gl.CreateShader(GL_VERTEX_SHADER);
		fs_id = gl.CreateShader(GL_FRAGMENT_SHADER);
		gl.ShaderSource(vs_id, 1, &vertex_shader, NULL);
		gl.ShaderSource(fs_id, 1, &fragment_shader, NULL);
		gl.CompileShader(vs_id);
		gl.CompileShader(fs_id);

		int success;
		char info_log[512];

		gl.GetShaderiv(vs_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			gl.GetShaderInfoLog(vs_id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		gl.GetShaderiv(fs_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			gl.GetShaderInfoLog(fs_id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
		}

		program_id = gl.CreateProgram();
		gl.AttachShader(program_id, vs_id);
		gl.AttachShader(program_id, fs_id);
		gl.LinkProgram(program_id);

		gl.GetProgramiv(program_id, GL_LINK_STATUS, &success);
		if (!success) {
			gl.GetProgramInfoLog(program_id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
		}

		gl.UseProgram(program_id);

		gl.GenBuffers(1, &vbo);
		gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
		gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		gl.EnableVertexAttribArray(0);
		gl.GenBuffers(1, &ebo);
		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		return true;
	}

	bool deinit() override {
		return windowed_application::deinit();
	}

	void update() override {
		const li::gl &gl = get_window().get_gl();
		gl.Viewport(0, 0, width, height);
		gl.ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		gl.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		gl.Flush();
		windowed_application::update();
	}

	void on(li::window_close_event &event) {
		(void) event;
		stop();
	}

	void on(li::window_resize_event &event) {
		width = event.width;
		height = event.height;
	}

	void on(li::motion_notify_event &event) {
		xp = (float) event.x / width * 2 - 1;
		yp = (float) event.y / height * 2 - 1;
	}
};

int main(void) {
	sandbox sandbox;
	sandbox.start();
	return 0;
}
