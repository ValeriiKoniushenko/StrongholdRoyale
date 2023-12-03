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
#include "CoordinateSystemVisualizer.h"
#include "Cube.h"
#include "Grid.h"
#include "Image.h"
#include "Initer.h"
#include "InputAction.h"
#include "Lightning.h"
#include "ModelPack.h"
#include "RayCast.h"
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
	camera.setFov(120.f);
	camera.setPosition({1000.f, 0.f, 1000.f});

	ShaderPack shaderPack;
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack.loadShaders("triangle", "assets/shaders/triangle.vert", "assets/shaders/triangle.frag");
	shaderPack.loadShaders("outline", "assets/shaders/outline.vert", "assets/shaders/outline.frag");
	shaderPack.loadShaders("line", "assets/shaders/line.vert", "assets/shaders/line.frag");

	Texture textureLoading(Gl::Texture::Target::Texture2D, true, true);
	Image imageLoading("assets/textures/loading.png", Gl::Texture::Channel::SRGBA);
	textureLoading.setImage(imageLoading);

	Texture textureRock(Gl::Texture::Target::Texture2D, true, true);
	Image imageRock("assets/textures/rock.png", Gl::Texture::Channel::SRGBA);
	textureRock.setImage(imageRock);

	Texture textureRockSpecular(Gl::Texture::Target::Texture2D, true, true);
	Image imageRockSpecular("assets/textures/rockSpecular.png", Gl::Texture::Channel::SRGBA);
	textureRockSpecular.setImage(imageRockSpecular);

	Texture textureBox(Gl::Texture::Target::Texture2D, true, true);
	Image imageBox("assets/textures/box.jpg", Gl::Texture::Channel::SRGB);
	textureBox.setMagAndMinFilter(Gl::Texture::MagFilter::Nearest, Gl::Texture::MinFilter::Nearest);
	textureBox.setImage(imageBox);

	Texture textureBoxSpecular(Gl::Texture::Target::Texture2D, true, true);
	Image imageBoxSpecular("assets/textures/boxSpecular.jpg", Gl::Texture::Channel::SRGB);
	textureBoxSpecular.setImage(imageBoxSpecular);

	Texture textureSun(Gl::Texture::Target::Texture2D, true, true);
	Image imageSun("assets/textures/sun.jpg", Gl::Texture::Channel::SRGB);
	textureSun.setImage(imageSun);

	Texture textureFabric(Gl::Texture::Target::Texture2D, true, true);
	Image imageFabric("assets/textures/Fabric048_4K-JPG_Color.jpg", Gl::Texture::Channel::SRGB);
	textureFabric.setImage(imageFabric);

	Texture textureFabricSpecular(Gl::Texture::Target::Texture2D, true, true);
	Image imageFabricSpecular("assets/textures/Fabric048_4K-JPG_Specular.jpg", Gl::Texture::Channel::SRGB);
	textureFabricSpecular.setImage(imageFabricSpecular);

	Widget widget(textureLoading);
	widget.move({100.f, 100.f});
	widget.setOrigin({-50.f, -50.f});

	float cameraImpulse = 5.0f;

	KeyboardInputAction iaCameraRight("Move camera to right", Keyboard::Key::D);
	iaCameraRight.setIsRepeatable(true);
	iaCameraRight.onAction.subscribe([&]() { camera.addImpulseRight(cameraImpulse); });

	KeyboardInputAction iaCameraLeft("Move camera to left", Keyboard::Key::A);
	iaCameraLeft.setIsRepeatable(true);
	iaCameraLeft.onAction.subscribe([&]() { camera.addImpulseRight(-cameraImpulse); });

	KeyboardInputAction iaCameraForward("Move camera forward", Keyboard::Key::W);
	iaCameraForward.setIsRepeatable(true);
	iaCameraForward.onAction.subscribe([&]() { camera.addImpulseForward(cameraImpulse); });

	KeyboardInputAction iaCameraBackward("Move camera backward", Keyboard::Key::S);
	iaCameraBackward.setIsRepeatable(true);
	iaCameraBackward.onAction.subscribe([&]() { camera.addImpulseForward(-cameraImpulse); });

	KeyboardInputAction iaCameraUp("Move camera to up", Keyboard::Key::Space);
	iaCameraUp.setIsRepeatable(true);
	iaCameraUp.onAction.subscribe([&]() { camera.addImpulseUp(cameraImpulse); });

	KeyboardInputAction iaCameraDown("Move camera to down", Keyboard::Key::C);
	iaCameraDown.setIsRepeatable(true);
	iaCameraDown.onAction.subscribe([&]() { camera.addImpulseUp(-cameraImpulse); });

	KeyboardInputAction iaExit("Exit", Keyboard::Key::Esc);
	iaExit.setIsRepeatable(false);
	iaExit.onAction.subscribe([&]() { std::exit(0); });

	MouseInputAction iaCameraRotate("Camera rotate");
	iaCameraRotate.onMove.subscribe(
		[&](glm::ivec2 direction)
		{
			camera.rotate(static_cast<glm::vec3>(glm::ivec3(direction, 0.f)));
			GetWindow().setCursorPosition(GetWindow().getSize().width / 2.0, GetWindow().getSize().height / 2.0);
		});

	RayCast rayCast;
	rayCast.setWidth(10.f);
	rayCast.setColor({255, 0, 0, 255});

	MouseInputAction iaCameraRay("Camera ray", Mouse::Key::Left);
	iaCameraRay.setIsRepeatable(false);
	iaCameraRay.onMouseClick.subscribe(
		[&](glm::ivec2 mousePosition)
		{
			auto ray = camera.getPosition() - camera.getForwardVector() * 10000.f;
			rayCast.setStartAndEndPoint(camera.getPosition(), ray);
			if (auto* obj = rayCast.findIntersects(); obj)
			{
				obj->toggleOutline();
				obj->toggleDrawCoordinateSystem();
			}
		});

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);

	ModelPack modelPack;
	modelPack.loadFromFile("assets/models/rock.obj");
	for (auto& model : modelPack)
	{
		model.second.setScale({100.f, 100.f, 100.f});
		model.second.setPosition({1000.f, 0.f, 1000.f});
		model.second.setTexture(textureRock);
		model.second.setSpecularTexture(textureRockSpecular);
		model.second.setTextureRect({2048.f, 2048.f});
		model.second.setOutlineSize({0.8f, 0.8f, 0.8f});
	}

	Cube cube;
	cube.setSpecularTexture(textureBoxSpecular);
	cube.setTexture(textureBox);
	cube.setOrigin({50.f, 50.f, -50.f});
	cube.setPosition({350.f, 50.f, 350.f});

	Cube cube3;
	cube3.setSpecularTexture(textureBoxSpecular);
	cube3.setTexture(textureBox);
	cube3.setOrigin({50.f, 50.f, -50.f});
	cube3.setPosition({-350.f, 50.f, 350.f});

	Cube cube2;
	cube2.setTexture(textureFabric);
	cube2.setSpecularTexture(textureFabricSpecular);
	cube2.setPosition({0.f, 500.f, 0});
	cube2.setOrigin({50.f, 50.f, -50.f});

	Cube cube1;
	cube1.setPosition({0.f, 0.f, -1000.f});
	cube1.setTexture(textureBox);
	cube1.setOrigin({50.f, 50.f, -50.f});

	Cube sun;
	sun.setTexture(textureSun);
	sun.setPosition({1000.f, 0, 0});
	sun.setOrigin({50.f, 50.f, -50.f});

	Grid grid;
	grid.setColor({44, 44, 44, 97});
	grid.setWidth(2.f);
	grid.setSize(10'000);

	Lightning lightning;
	lightning.specular.position = sun.getPosition();

	CoordinateSystemVisualizer csv;

	Clock clock;
	while (!GetWindow().shouldClose())
	{
		clock.start();
		GetWindow().clearColor({0.2f, 0.3f, 0.3f});
		GetWindow().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		grid.draw(shaderPack, lightning, camera);

		cube1.draw(shaderPack, lightning, camera);
		cube1.setScale(glm::vec3(sin(::clock() / 300.f) * 3 + 3.2f));

		cube.setScale({1.f, 1.f, 1.f});
		cube.draw(shaderPack, lightning, camera);

		for (auto& model : modelPack)
		{
			model.second.draw(shaderPack, lightning, camera);
		}

		cube2.draw(shaderPack, lightning, camera);

		cube3.rotateX(0.05);
		cube3.rotateY(0.05);
		cube3.rotateZ(0.05);
		cube3.draw(shaderPack, lightning, camera);

		csv.draw(shaderPack, lightning, camera);

		sun.draw(shaderPack, lightning, camera);
		sun.rotateY(-0.005f);

		widget.draw(shaderPack);
		widget.rotate(-0.05f);

		GetWorld().update();
		GetWindow().pollEvent();
		GetWindow().swapBuffers();
		GetUpdateableCollector().updateAll();
		GetWorldVariables()["tick"] = clock.stop();
	}
}
