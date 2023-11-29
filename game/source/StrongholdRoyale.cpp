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
#include "Clock.h"
#include "Cube.h"
#include "Image.h"
#include "Initer.h"
#include "InputAction.h"
#include "Lightning.h"
#include "ShaderPack.h"
#include "Texture.h"
#include "UpdateableCollector.h"
#include "Widget.h"
#include "WorldVariables.h"
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

	float cameraSpeed = 5.0f;

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);

	Cube cube;
	cube.setTexture(textureBox);
	cube.setOrigin({-50.f, -50.f, -50.f});

	Cube sun;
	sun.setTexture(textureLoading);
	sun.setPosition({1000.f, 0, 0});
	sun.setOrigin({-50.f, -50.f, -50.f});

	Lightning lightning;
	lightning.ambient.lightColor = toGlColor3({255, 255, 255});
	lightning.specular.position = sun.getPosition();

	glm::vec2 textureSize = textureBox.getImage()->getSize();
	float size = 50.f;
	Triangle triangle;
	triangle.setVertices({TriangleVbo::Unit{{0.f, 0.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}},
		TriangleVbo::Unit{{size, 0.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}},
		TriangleVbo::Unit{{size, size, 0.f}, {1.f, 1.f}, {0.f, 0.f, 1.f}, {textureSize.x, textureSize.y}}});
	triangle.setPosition({500.f, 0.f, 0.f});
	triangle.setTexture(textureBox);

	Clock clock;
	while (!GetWindow().shouldClose())
	{
		clock.start();
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GetWorldVariables()["tick"] = clock.getGap();

		cube.draw(shaderPack, lightning, camera);
		sun.draw(shaderPack, lightning, camera);
		triangle.draw(shaderPack, lightning, camera);
		triangle.rotateY(-0.005f);

		widget.draw(shaderPack);
		widget.rotate(-0.05f);

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
		GetUpdateableCollector().updateAll();
		clock.stop();
	}
}
