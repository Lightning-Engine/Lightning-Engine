#include "li/window.hh"
#include "li/application.hh"
#include "li/math/vector.hh"
#include "li/math/matrix.hh"
#include "li/util/logger.hh"
#include <iostream>
#include <chrono>

extern "C" {
	#include "li/gl.h"
	#include "li/util/log.h"
	#include "li/memory.h"
}

class sandbox : public li::windowed_application {
public:
	GLuint vbo, ebo, vao, vs_id, fs_id, program_id;

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

	sandbox() : li::windowed_application({ 640, 480 }) { }

	bool init() override {
		if (!windowed_application::init())
			return false;
		const li::gl &gl = get_window().get_gl();

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
		gl.BindAttribLocation(program_id, 0, "aPos");
		gl.LinkProgram(program_id);

		gl.GetProgramiv(program_id, GL_LINK_STATUS, &success);
		if (!success) {
			gl.GetProgramInfoLog(program_id, 512, NULL, info_log);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
		}

		gl.UseProgram(program_id);
		gl.GenVertexArrays(1, &vao);
		gl.BindVertexArray(vao);
		gl.GenBuffers(1, &vbo);
		gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
		gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		gl.EnableVertexAttribArray(0);
		gl.BindBuffer(GL_ARRAY_BUFFER, vbo);
		gl.GenBuffers(1, &ebo);
		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		return true;
	}

	bool deinit() override {
		return windowed_application::deinit();
	}

	void update() override {
		li::window &window = get_window();
		const li::gl &gl = get_window().get_gl();
		gl.Viewport(0, 0, window.get_size().WIDTH, window.get_size().HEIGHT);
		gl.ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		// gl.DrawArrays(GL_TRIANGLES, 0, 3);
		gl.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		gl.Flush();
		windowed_application::update();
	}
};

void log_cool_callback(li_sink_t *sink, li_log_str_t *str) {
	printf("%s", str->str);
	(void) sink;
}

void log_cooler_callback(li_sink_t *sink, li_log_str_t *str) {
	printf("%s", str->str);
	(void) sink;
}

int log_cool_fmt(char **out, const char *str) {
	int len;

	auto time = std::chrono::system_clock::now().time_since_epoch().count() / 1000000000;
	len = snprintf(NULL, 0, "[%lu]: %s\n", time, str) + 1;
	*out = (char*) li_safe_malloc(len);
	snprintf(*out, len, "[%lu]: %s\n", time, str);
	return len;
}

int main(void) {
	li::mat4d lhs = li::mat4d::rotate<0, 1>(M_PI / 2);

	li::logger::get_logger("debug").log("\n{}", lhs * li::vec4d { 1, 2, 3, 4 });
	// sandbox sandbox;
	// sandbox.start();
	return 0;
}
