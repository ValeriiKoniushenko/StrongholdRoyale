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
#include "InputAction.h"
#include "ShaderPack.h"
#include "Texture.h"
#include "Triangle.h"
#include "UpdateableCollector.h"
#include "Widget.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include <iostream>

void StrongholdRoyale::start()
{
	Initer::init({.glfwVersion = glm::ivec2(3, 3), .windowSize = {2000, 1000}, .title = "Stronghold Royale"});

	GetWindow().viewport(0, 0, GetWindow().getSize().width, GetWindow().getSize().height);

	ShaderPack shaderPack;
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack.loadShaders("triangle", "assets/shaders/triangle.vert", "assets/shaders/triangle.frag");

	Texture textureLoading(Gl::Texture::Target::Texture2D, true, true);
	Image imageLoading("assets/textures/loading.png", Gl::Texture::Channel::SRGBA);
	textureLoading.setImage(imageLoading);

	Widget widget(textureLoading);
	widget.move({100.f, 100.f});
	widget.setOrigin({-50.f, -50.f});

	Triangle triangle(textureLoading);
	triangle.move({0.f, 0.f, -100.f});

	glEnable(GL_DEPTH_TEST);

	float speed = 10.f;
	KeyboardInputAction moveRight("moveRight", Keyboard::Key::D);
	moveRight.setIsRepeatable(true);
	moveRight.onAction.subscribe([&]() { triangle.move(glm::vec3(speed, 0, 0)); });

	KeyboardInputAction moveLeft("moveLeft", Keyboard::Key::A);
	moveLeft.setIsRepeatable(true);
	moveLeft.onAction.subscribe([&]() { triangle.move(glm::vec3(-speed, 0, 0)); });

	KeyboardInputAction moveTop("moveTop", Keyboard::Key::W);
	moveTop.setIsRepeatable(true);
	moveTop.onAction.subscribe([&]() { triangle.move(glm::vec3(0, speed, 0)); });

	KeyboardInputAction moveBottom("moveBottom", Keyboard::Key::S);
	moveBottom.setIsRepeatable(true);
	moveBottom.onAction.subscribe([&]() { triangle.move(glm::vec3(0, -speed, 0)); });

	KeyboardInputAction moveFar("moveFar", Keyboard::Key::Q);
	moveFar.setIsRepeatable(true);
	moveFar.onAction.subscribe([&]() { triangle.move(glm::vec3(0, 0, speed)); });

	KeyboardInputAction moveNear("moveNear", Keyboard::Key::E);
	moveNear.setIsRepeatable(true);
	moveNear.onAction.subscribe([&]() { triangle.move(glm::vec3(0, 0, -speed)); });

	triangle.setVertices({
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 0.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
	});

	const glm::mat4 proj = glm::perspective(glm::radians(190.0f),
		static_cast<float>(GetWindow().getSize().width) / static_cast<float>(GetWindow().getSize().height), 0.1f, 10000.0f);

	glm::mat4 model = glm::mat4(1.f);
	glm::mat4 view = glm::mat4(1.f);
	view = glm::translate(view, glm::vec3(0, 0, -100.f));

	while (!GetWindow().shouldClose())
	{
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		widget.draw(shaderPack);
		widget.rotate(-0.05f);

		triangle.draw(shaderPack, proj, view, model);

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
		GetUpdateableCollector().updateAll();
	}
}
