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

#include "Camera.h"
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

	Camera camera;
	camera.setSensitive({3.f, 3.f});
	camera.setFov(110.f);

	ShaderPack shaderPack;
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack.loadShaders("triangle", "assets/shaders/triangle.vert", "assets/shaders/triangle.frag");

	Texture textureLoading(Gl::Texture::Target::Texture2D, true, true);
	Image imageLoading("assets/textures/loading.png", Gl::Texture::Channel::SRGBA);
	textureLoading.setImage(imageLoading);

	Texture textureBox(Gl::Texture::Target::Texture2D, true, true);
	Image imageBox("assets/textures/box.jpg", Gl::Texture::Channel::SRGB);
	textureBox.setImage(imageBox);

	Widget widget(textureLoading);
	widget.move({100.f, 100.f});
	widget.setOrigin({-50.f, -50.f});

	float cameraSpeed = 1.f;

	KeyboardInputAction iaCameraRight("Move camera to right", Keyboard::Key::D);
	iaCameraRight.setIsRepeatable(true);
	iaCameraRight.onAction.subscribe([&]() { camera.moveRight(cameraSpeed); });

	KeyboardInputAction iaCameraLeft("Move camera to left", Keyboard::Key::A);
	iaCameraLeft.setIsRepeatable(true);
	iaCameraLeft.onAction.subscribe([&]() { camera.moveRight(-cameraSpeed); });

	KeyboardInputAction iaCameraForward("Move camera forward", Keyboard::Key::W);
	iaCameraForward.setIsRepeatable(true);
	iaCameraForward.onAction.subscribe([&]() { camera.moveForward(cameraSpeed); });

	KeyboardInputAction iaCameraBackward("Move camera to backward", Keyboard::Key::S);
	iaCameraBackward.setIsRepeatable(true);
	iaCameraBackward.onAction.subscribe([&]() { camera.moveForward(-cameraSpeed); });

	MouseInputAction iaCameraRotate("Camera rotate");
	iaCameraRotate.onMove.subscribe([&](glm::ivec2 direction) { camera.rotate(static_cast<glm::vec2>(direction)); });

	Triangle triangle(textureBox);
	triangle.setVertices({
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 0.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
	});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	while (!GetWindow().shouldClose())
	{
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		widget.draw(shaderPack);
		widget.rotate(-0.05f);

		std::cout << camera.getRotation().x << " " << camera.getRotation().y << std::endl;
		triangle.draw(shaderPack, camera, glm::mat4(1.f));

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
		GetUpdateableCollector().updateAll();
	}
}
