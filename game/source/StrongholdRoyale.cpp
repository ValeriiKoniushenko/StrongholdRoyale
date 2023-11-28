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

#include <iostream>

void StrongholdRoyale::start()
{
	Initer::init({.glfwVersion = glm::ivec2(3, 3), .windowSize = {2000, 1000}, .title = "Stronghold Royale"});

	GetWindow().viewport(0, 0, GetWindow().getSize().width, GetWindow().getSize().height);

	Camera camera;
	camera.setSensitive({3.f, 3.f});
	camera.setFov(80.f);

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

	float cameraSpeed = 0.005f;

	KeyboardInputAction iaCameraRight("Move camera to right", Keyboard::Key::D);
	iaCameraRight.setIsRepeatable(true);
	iaCameraRight.onAction.subscribe([&]() { camera.addImpulseRight(cameraSpeed); });

	KeyboardInputAction iaCameraLeft("Move camera to left", Keyboard::Key::A);
	iaCameraLeft.setIsRepeatable(true);
	iaCameraLeft.onAction.subscribe([&]() { camera.addImpulseRight(-cameraSpeed); });

	KeyboardInputAction iaCameraForward("Move camera forward", Keyboard::Key::W);
	iaCameraForward.setIsRepeatable(true);
	iaCameraForward.onAction.subscribe([&]() { camera.addImpulseForward(cameraSpeed); });

	KeyboardInputAction iaCameraBackward("Move camera backward", Keyboard::Key::S);
	iaCameraBackward.setIsRepeatable(true);
	iaCameraBackward.onAction.subscribe([&]() { camera.addImpulseForward(-cameraSpeed); });

	KeyboardInputAction iaCameraUp("Move camera to up", Keyboard::Key::Space);
	iaCameraUp.setIsRepeatable(true);
	iaCameraUp.onAction.subscribe([&]() { camera.addImpulseUp(cameraSpeed); });

	KeyboardInputAction iaCameraDown("Move camera to down", Keyboard::Key::C);
	iaCameraDown.setIsRepeatable(true);
	iaCameraDown.onAction.subscribe([&]() { camera.addImpulseUp(-cameraSpeed); });

	KeyboardInputAction iaExit("Exit", Keyboard::Key::Esc);
	iaExit.setIsRepeatable(false);
	iaExit.onAction.subscribe([&]() { std::exit(0); });

	MouseInputAction iaCameraRotate("Camera rotate");
	iaCameraRotate.onMove.subscribe(
		[&](glm::ivec2 direction)
		{
			camera.rotate(static_cast<glm::vec2>(direction));
			GetWindow().setCursorPosition(GetWindow().getSize().width / 2.0, GetWindow().getSize().height / 2.0);
		});

	// front side
	Triangle triangle1(textureBox);
	triangle1.setVertices({
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 0.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
	});
	Triangle triangle2(textureBox);
	triangle2.setVertices({
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}},
		{{0.f, 100.f, 0.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}},
	});

	// back side
	Triangle triangle3(textureBox);
	triangle3.setVertices({
		{{0.f, 0.f, -100.f}, {0.f, 0.f}, {0.f, 0.f, -1.f}},
		{{100.f, 100.f, -100.f}, {1.f, 1.f}, {0.f, 0.f, -1.f}},
		{{100.f, 0.f, -100.f}, {1.f, 0.f}, {0.f, 0.f, -1.f}},
	});
	Triangle triangle4(textureBox);
	triangle4.setVertices({
		{{0.f, 0.f, -100.f}, {0.f, 0.f}, {0.f, 0.f, -1.f}},
		{{0.f, 100.f, -100.f}, {0.f, 1.f}, {0.f, 0.f, -1.f}},
		{{100.f, 100.f, -100.f}, {1.f, 1.f}, {0.f, 0.f, -1.f}},
	});

	// left side
	Triangle triangle5(textureBox);
	triangle5.setVertices({
		{{0.f, 0.f, 0.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{0.f, 100.f, 0.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{0.f, 0.f, -100.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
	});
	Triangle triangle6(textureBox);
	triangle6.setVertices({
		{{0.f, 0.f, -100.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f}},
		{{0.f, 100.f, 0.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f}},
		{{0.f, 100.f, -100.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f}},
	});

	// right side
	Triangle triangle7(textureBox);
	triangle7.setVertices({
		{{100.f, 0.f, 0.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{100.f, 0.f, -100.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},
	});
	Triangle triangle8(textureBox);
	triangle8.setVertices({
		{{100.f, 0.f, -100.f}, {0.f, 0.f}, {1.f, 0.f, 0.f}},
		{{100.f, 100.f, -100.f}, {1.f, 0.f}, {1.f, 0.f, 0.f}},
		{{100.f, 100.f, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f}},
	});

	// top side
	Triangle triangle9(textureBox);
	triangle9.setVertices({
		{{0.f, 100.f, 0.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
		{{100.f, 100.f, -100.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}},
		{{0.f, 100.f, -100.f}, {0.f, 1.f}, {0.f, 1.f, 0.f}},
	});
	Triangle triangle10(textureBox);
	triangle10.setVertices({
		{{100.f, 100.f, -100.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}},
		{{0.f, 100.f, 0.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
		{{100.f, 100.f, 0.f}, {1.f, 0.f}, {0.f, 1.f, 0.f}},
	});

	// bottom side
	Triangle triangle11(textureBox);
	triangle11.setVertices({
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
		{{0.f, 0.f, -100.f}, {0.f, 1.f}, {0.f, -1.f, 0.f}},
		{{100.f, 0.f, -100.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
	});
	Triangle triangle12(textureBox);
	triangle12.setVertices({
		{{100.f, 0.f, -100.f}, {1.f, 1.f}, {0.f, -1.f, 0.f}},
		{{100.f, 0.f, 0.f}, {1.f, 0.f}, {0.f, -1.f, 0.f}},
		{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, -1.f, 0.f}},
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

		triangle1.draw(shaderPack, camera, glm::mat4(1.f));
		triangle2.draw(shaderPack, camera, glm::mat4(1.f));
		triangle3.draw(shaderPack, camera, glm::mat4(1.f));
		triangle4.draw(shaderPack, camera, glm::mat4(1.f));
		triangle5.draw(shaderPack, camera, glm::mat4(1.f));
		triangle6.draw(shaderPack, camera, glm::mat4(1.f));
		triangle7.draw(shaderPack, camera, glm::mat4(1.f));
		triangle8.draw(shaderPack, camera, glm::mat4(1.f));
		triangle9.draw(shaderPack, camera, glm::mat4(1.f));
		triangle10.draw(shaderPack, camera, glm::mat4(1.f));
		triangle11.draw(shaderPack, camera, glm::mat4(1.f));
		triangle12.draw(shaderPack, camera, glm::mat4(1.f));

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
		GetUpdateableCollector().updateAll();
	}
}
