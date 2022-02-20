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
	GLuint vbo, ebo, vao, vs_id, fs_id, program_id;
	li::opengl::shader<li::opengl::shader_type::vertex_shader> vshader;
	li::opengl::shader<li::opengl::shader_type::fragment_shader> fshader;
	li::opengl::program prog;

	li::opengl::buffer<float> vertex_buffer;
	li::opengl::buffer<unsigned int> index_buffer;
	li::opengl::vertex_array vertex_array;

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

		// li::opengl::buffer<li::opengl::array_buffer> vbo;

		// li::gl::shader vs = gl.CreateShader(GL_VERTEX_SHADER);

		// li::gl::shader vs(gl, li::gl::vertex_shader);
		// li::gl::shader fs(gl, li::gl::fragment_shader);
		// vs.compile(gl);
		// fs.compile(gl);
		// li::gl::program program(gl);
		// program.attach(gl, vs_compiled);
		// program.attach(gl, fs_compiled);
		// program.link(gl);

		// li::gl::buffer<data> vb(gl, li::gl::array_buffer);
		// li::gl::buffer<data, li::triangles> eb(gl, li::gl::element_array_buffer);
		// vb.data(gl, vertices, sizeof(vertices));
		// li::gl::bind(vb, )
		// models -> meshes -> implementation defined
		// textures
		// materials (shaders)

		// eb.draw(gl, vb, li::gl::triangles, 0, 6);

		vshader.create();
		fshader.create();
		prog.create();
		vshader.set_source(vertex_shader);
		fshader.set_source(fragment_shader);

		vshader.compile();
		fshader.compile();
		prog.attach_shader(vshader);
		prog.attach_shader(fshader);
		prog.link();

		prog.enable();

		vertex_buffer.create();
		index_buffer.create();
		vertex_array.create();

		li::opengl::vertex_array_binding vertex_array_binding(vertex_array);
		li::opengl::buffer_binding<GLfloat, li::opengl::array_buffer> vertex_buffer_binding(vertex_buffer);
		li::opengl::buffer_binding<GLuint, li::opengl::element_array_buffer> index_buffer_binding(index_buffer);

		vertex_buffer_binding.data(sizeof(vertices) / sizeof(*vertices), vertices);
		index_buffer_binding.data(sizeof(indices) / sizeof(*indices), indices);
		vertex_array_binding.attrib<0, float>(3, GL_FLOAT, 0, vertex_buffer_binding);
		vertex_array_binding.enable<0>();

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
		li::opengl::vertex_array_binding vertex_array_binding(vertex_array);
		li::opengl::buffer_binding<unsigned int, li::opengl::element_array_buffer> index_buffer_binding(index_buffer);
		LI_DEBUG("{}", (void*) li::gl->GetError());
		li::gl->DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
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
