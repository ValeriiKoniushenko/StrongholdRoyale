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

#include "SRGameState.h"

#include "Raycast.h"

void SRGameState::onCreate()
{
	// ========= CAMERA ===========
	camera.setSensitive({3.f, 3.f});
	camera.setFov(120.f);
	camera.setPosition({100.f, 100.f, 100.f});

	// ========= SHADERS ===========
	shaderPack.loadShaders("widget", "assets/shaders/widget.vert", "assets/shaders/widget.frag");
	shaderPack.loadShaders("triangle", "assets/shaders/triangle.vert", "assets/shaders/triangle.frag");
	shaderPack.loadShaders("outline", "assets/shaders/outline.vert", "assets/shaders/outline.frag");
	shaderPack.loadShaders("line", "assets/shaders/line.vert", "assets/shaders/line.frag");
	shaderPack.loadShaders("skybox", "assets/shaders/skybox.vert", "assets/shaders/skybox.frag");

	// ========== INPUT ACTIONS ============
	iaCameraRight.setName("Move camera to right");
	iaCameraRight.setKey(Keyboard::Key::D);
	iaCameraRight.onPress.subscribe([&]() { camera.addImpulseRight(cameraImpulse); });

	iaCameraLeft.setName("Move camera to left");
	iaCameraLeft.setKey(Keyboard::Key::A);
	iaCameraLeft.onPress.subscribe([&]() { camera.addImpulseRight(-cameraImpulse); });

	iaCameraForward.setName("Move camera forward");
	iaCameraForward.setKey(Keyboard::Key::W);
	iaCameraForward.onPress.subscribe([&]() { camera.addImpulseForward(cameraImpulse); });

	iaCameraBackward.setName("Move camera backward");
	iaCameraBackward.setKey(Keyboard::Key::S);
	iaCameraBackward.onPress.subscribe([&]() { camera.addImpulseForward(-cameraImpulse); });

	iaCameraUp.setName("Move camera to up");
	iaCameraUp.setKey(Keyboard::Key::Space);
	iaCameraUp.onPress.subscribe([&]() { camera.addImpulseUp(cameraImpulse); });

	iaCameraDown.setName("Move camera to down");
	iaCameraDown.setKey(Keyboard::Key::C);
	iaCameraDown.onPress.subscribe([&]() { camera.addImpulseUp(-cameraImpulse); });

	iaExit.setName("Exit");
	iaExit.setIsRepeatable(false);
	iaExit.setKey(Keyboard::Key::Esc);
	iaExit.onPress.subscribe([&]() { std::exit(0); });

	iaCameraRotate.setName("Camera rotate");
	iaCameraRotate.onMove.subscribe(
		[&](glm::ivec2 direction)
		{
			camera.rotate(static_cast<glm::vec3>(glm::ivec3(direction, 0.f)));
			GetWindow().setCursorPosition(GetWindow().getSize().width / 2.0, GetWindow().getSize().height / 2.0);
		});

	iaCameraRay.setName("Camera Ray");
	iaCameraRay.setKey(Mouse::Key::Left);
	iaCameraRay.setIsRepeatable(false);
	iaCameraRay.onMouseClick.subscribe(
		[&](glm::ivec2 mousePosition)
		{
			RayCast rayCast;

			auto ray = camera.getPosition() - camera.getForwardVector() * 10000.f;
			rayCast.setStartAndEndPoint(camera.getPosition(), ray);
			if (auto* obj = rayCast.findIntersects(); obj)
			{
				obj->toggleOutline();
				obj->toggleDrawCoordinateSystem();
			}
		});

	// ========= TEXTURES & IMAGES ===========
	textureRock.loadImage("assets/textures/overlay.png");
	textureRock.setInternalChannel(Gl::Texture::Channel::SRGBA);

	textureRockSpecular.loadImage("assets/textures/overlaySpecular.png");
	textureRockSpecular.setInternalChannel(Gl::Texture::Channel::SRGBA);

	// ========= MODELS ===========
	// modelPack.loadFromFile("assets/models/stone_small/stone_small_a.obj");
	modelPack.loadFromFile("assets/models/landscape2.obj");
	for (auto& model : modelPack)
	{
		model.second.setScale({100.f, 100.f, 100.f});
		model.second.setPosition({0.f, 0.f, 0.f});
		model.second.setTexture(textureRock);
		model.second.setSpecularTexture(textureRockSpecular);
		model.second.setTextureRect({3072.f, 2560.f});
	}

	skybox.loadFromFiles({"assets/textures/cubemap/px.png", "assets/textures/cubemap/nx.png", "assets/textures/cubemap/py.png",
		"assets/textures/cubemap/ny.png", "assets/textures/cubemap/pz.png", "assets/textures/cubemap/nz.png"});
}

void SRGameState::onTick()
{
	skybox.draw(shaderPack, camera);

	grid.draw(shaderPack, lightning, camera);

	csv.draw(shaderPack, lightning, camera);

	for (auto& model : modelPack)
	{
		model.second.draw(shaderPack, lightning, camera);
	}
}
