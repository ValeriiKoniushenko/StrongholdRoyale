// MIT License
//
// Copyright (c) 2023 Valerii Koniushenko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "StrongholdRoyale.h"

#include "Image.h"
#include "Initer.h"
#include "ShaderPack.h"
#include "Texture.h"
#include "Vao.h"
#include "WidgetVbo.h"
#include "glm/ext/matrix_clip_space.hpp"

void StrongholdRoyale::start()
{
	Initer::init({.glfwVersion = glm::ivec2(3, 3), .windowSize = {2000, 1000}, .title = "Stronghold Royale"});

	GetWindow().viewport(0, 0, 2000, 1000);

	ShaderPack shaderPack;
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");

	shaderPack["widget"].use();

	Vao vao(true, true);
	WidgetVbo vbo(true, true);
	Texture texture(Gl::Texture::Target::Texture2D, true, true);

	Image image;
	image.loadImage("assets/textures/box.jpg");

	texture.setImage(image);
	texture.loadToGpu();

	vbo.data({
		{{0.f, 0.f}, {0.f, 0.f}},
		{{111.f, 0.f}, {1.f, 0.f}},
		{{111.f, 111.f}, {1.f, 1.f}},
	});

	Gl::Vao::vertexAttribPointer(1, 2, Gl::Type::Float, false, 4 * sizeof(float), nullptr);
	Gl::Vao::vertexAttribPointer(2, 2, Gl::Type::Float, false, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));

	glm::mat4 proj = glm::ortho(0.0f, 2000.0f, 0.0f, 1000.0f, 0.1f, 1000.0f);

	while (!GetWindow().shouldClose())
	{
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT);

		auto& shader = shaderPack["widget"];
		shader.use();
		shader.uniform("uProjection", false, proj);

		vao.bind();
		vbo.bind();
		texture.bind();
		Gl::drawArrays(GL_TRIANGLES, 0, 3);

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
	}
}
