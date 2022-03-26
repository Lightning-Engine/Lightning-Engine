#include "li/window.hh"
#include "li/application.hh"
#include "li/math/vector.hh"
#include "li/math/matrix.hh"
#include "li/util/logger.hh"
#include "li/gl/shader.hh"
#include "li/gl/buffer.hh"
#include <iostream>
#include <chrono>

extern "C" {
	#include "li/gl.h"
	#include "li/util/log.h"
	#include "li/memory.h"
}

class sandbox : public li::windowed_application {
public:
	li::opengl::shader<li::opengl::shader_type::vertex_shader> vshader;
	li::opengl::shader<li::opengl::shader_type::fragment_shader> fshader;
	li::opengl::program prog;

	li::opengl::buffer<li::vec3f> vertex_buffer;
	li::opengl::buffer<unsigned int> index_buffer;
	li::opengl::vertex_array vertex_array;

	li::opengl::buffer_binding<li::vec3f, li::opengl::array_buffer> vertex_buffer_binding;
	li::opengl::buffer_binding<unsigned int, li::opengl::element_array_buffer> index_buffer_binding;
	li::opengl::vertex_array_binding vertex_array_binding;

	li::vec3f vertices[12] = {
		{ -0.5f, -0.5f, 0.0f },
		{ -0.5f, +0.5f, 0.0f },
		{ +0.5f, +0.5f, 0.0f },
		{ +0.5f, -0.5f, 0.0f },
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0,
	};

	const char *vertex_shader = "\
#version 330 core\n\
in vec3 aPos;\n\
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

		vshader.create();
		fshader.create();
		prog.create();
		vshader.source(vertex_shader);
		fshader.source(fragment_shader);
		vshader.compile();
		fshader.compile();
		prog.attach(vshader);
		prog.attach(fshader);
		prog.link();
		prog.use();

		vertex_buffer.gen();
		index_buffer.gen();
		vertex_array.gen();
		vertex_array_binding.bind(vertex_array);
		vertex_buffer_binding.bind(vertex_buffer);
		index_buffer_binding.bind(index_buffer);
		vertex_buffer_binding.data(sizeof(vertices) / sizeof(*vertices), vertices);
		index_buffer_binding.data(sizeof(indices) / sizeof(*indices), indices);
		vertex_array_binding.attrib_pointer<0, li::vec3f, li::vec3f>(vertex_buffer_binding);
		vertex_array_binding.enable_attrib<0>();

		return true;
	}

	bool deinit() override {
		return windowed_application::deinit();
	}

	void update() override {
		li::window &window = get_window();
		li::gl->Viewport(0, 0, window.get_size().WIDTH, window.get_size().HEIGHT);
		li::gl->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		li::gl->Clear(GL_COLOR_BUFFER_BIT);
		// gl.DrawArrays(GL_TRIANGLES, 0, 3);
		// LI_DEBUG("{}", (void*) li::gl->GetError());
		vertex_array_binding.draw_elements<li::opengl::triangles>(index_buffer_binding, 6);
		li::gl->Flush();
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

	LI_FATAL("vector: {}", lhs * li::vec4d { 1, 2, 3, 4 });
	sandbox sandbox;
	sandbox.start();
	return 0;
}
