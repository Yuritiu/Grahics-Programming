#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
}

MainGame::~MainGame()
{

}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay();

	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\ball.obj"); //ball.obj for the sphere
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\Skull.obj"); 
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	outlineShader.init("..\\res\\outline.vert", "..\\res\\outline.frag");
	eMapping.init("..\\res\\envmap.vert", "..\\res\\envmap.frag");
	noiseShader.init("..\\res\\noise.vert", "..\\res\\noise.frag"); //new shader
	pass.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	texture.init("..\\res\\noise.png"); //load texture
	texture1.init("..\\res\\lava3.jpg"); //load texture
	texture2.init("..\\res\\Water.jpg"); //load texture
	earthTex.init("..\\res\\earth.jpg");
	glow.init("..\\res\\glow.vert", "..\\res\\glow.frag");
	geoShader.initGeo("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.geom", "..\\res\\shaderGeoText.frag");

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.png",
		"..\\res\\skybox\\left.png",
		"..\\res\\skybox\\top.png",
		"..\\res\\skybox\\bot.png",
		"..\\res\\skybox\\front.png",
		"..\\res\\skybox\\back.png"
	};

	skybox.init(faces);
	reflectiveCubeShader.init("..\\res\\reflectCube.vert", "..\\res\\reflectCube.frag");
	initCubeMesh();          // next step
	initDynamicCubemap();
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				if (evnt.key.keysym.sym == SDLK_ESCAPE)
				{
					_gameState = GameState::EXIT;
				}
				break;


			case SDL_MOUSEMOTION:
			{
				float xoffset = evnt.motion.xrel * mouseSensitivity;
				float yoffset = -evnt.motion.yrel * mouseSensitivity; // invert Y

				yaw += xoffset;
				pitch += yoffset;

				// clamp pitch to avoid flipping
				if (pitch > 89.0f)  pitch = 89.0f;
				if (pitch < -89.0f) pitch = -89.0f;

				glm::vec3 front;
				front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				front.y = sin(glm::radians(pitch));
				front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				front = glm::normalize(front);

				myCamera.setForward(front);  // or setLookDirection/front, adapt to your API
				break;
			}
		}
	}
	const Uint8* state = SDL_GetKeyboardState(nullptr);

	if (state[SDL_SCANCODE_W])
		myCamera.MoveForward(camSpeed);
	if (state[SDL_SCANCODE_S])
		myCamera.MoveForward(-camSpeed);
	if (state[SDL_SCANCODE_A])
		myCamera.MoveRight(camSpeed);
	if (state[SDL_SCANCODE_D])
		myCamera.MoveRight(-camSpeed);
	if (state[SDL_SCANCODE_Q])
		myCamera.MoveUp(-camSpeed);
	if (state[SDL_SCANCODE_E])
		myCamera.MoveUp(camSpeed);
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon(const Camera& cam)
{
	glm::vec3 pos = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	// ===== Pass 1: normal toon =====
	toonShader.Bind();
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));

	transform.SetPos(pos);
	transform.SetRot(rot);
	transform.SetScale(scale);

	toonShader.Update(transform, cam);
	mesh2.draw();

	// ===== Pass 2: outline =====
	// Draw the slightly scaled-up backfaces as a silhouette.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);              // render back faces only (outline trick)

	outlineShader.Bind();
	outlineShader.setVec3("outlineColor", glm::vec3(0.0f, 0.0f, 0.0f)); // black outline

	transform.SetPos(pos);
	transform.SetRot(rot);
	transform.SetScale(scale * 1.05f);  // slightly bigger than pass 1

	outlineShader.Update(transform, cam);
	mesh2.draw();

	// Restore default culling
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}


void MainGame::linkGeo(const Camera& cam)
{
	geoShader.Bind();

	float randX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float randZ = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

	geoShader.setFloat("randColourX", randX);
	geoShader.setFloat("randColourY", randY);
	geoShader.setFloat("randColourZ", randZ);
	geoShader.setFloat("time", counter);

	transform.SetPos(glm::vec3(-4.0f, 0.0f, -4.0f));
	transform.SetRot(glm::vec3(0.0f, counter * 0.5f, 0.0f));
	transform.SetScale(glm::vec3(0.01f));

	geoShader.Update(transform, cam);

	// bind texture for skull
	GLint loc = glGetUniformLocation(geoShader.getID(), "diffuse");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1.getID()); 
	glUniform1i(loc, 0);

	mesh3.draw();
}

void MainGame::linkEmapping(const Camera& cam)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	eMapping.Bind();

	// ---- Orbit parameters (RESTORE THIS) ----
	float radius = 3.0f;
	float orbitSpeed = 1.0f;
	float orbitAngle = counter * orbitSpeed;

	float x = radius * cos(orbitAngle);
	float z = radius * sin(orbitAngle);

	float spinSpeed = 3.0f;
	float spinAngle = counter * spinSpeed;

	transform.SetPos(glm::vec3(x, 0.0f, z));
	transform.SetRot(glm::vec3(0.0f, spinAngle, 180.0f));
	transform.SetScale(glm::vec3(0.6f));   // KEEP your old ball size

	// ---- Envmap uniforms ----
	eMapping.setVec3("cameraPos", cam.getPos());
	eMapping.setMat4("model", transform.GetModel());
	eMapping.setMat4("view", cam.getView());
	eMapping.setMat4("projection", cam.getProjection());
	eMapping.setFloat("reflectStrength", 0.6f); // 1.0 is very shiny

	// Earth texture (diffuse) -> unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex.getID());
	glUniform1i(glGetUniformLocation(eMapping.getID(), "diffuse"), 0);

	// Cubemap -> unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	glUniform1i(glGetUniformLocation(eMapping.getID(), "skybox"), 2);

	mesh1.draw();
}

void MainGame::linkEarthPlain(const Camera& cam)
{
	pass.Bind();

	// same orbit transform as linkEmapping
	float radius = 3.0f;
	float orbitAngle = counter * 1.0f;
	float x = radius * cos(orbitAngle);
	float z = radius * sin(orbitAngle);
	float spinAngle = counter * 3.0f;

	transform.SetPos(glm::vec3(x, 0.0f, z));
	transform.SetRot(glm::vec3(0.0f, spinAngle, 180.0f));
	transform.SetScale(glm::vec3(0.6f));

	pass.Update(transform, cam);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex.getID());
	glUniform1i(glGetUniformLocation(pass.getID(), "diffuse"), 0);

	mesh1.draw();
}

void MainGame::linkNoise(const Camera& cam)
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	noiseShader.Bind();

	noiseShader.setFloat("fogDensity", 0.08f);
	noiseShader.setVec3("fogColor", 0.10f, 0.05f, 0.18f);
	noiseShader.setFloat("minDist", 0.6f);
	noiseShader.setFloat("maxDist", 1.0f);

	// Bind lava texture as texture2
	GLuint t2L = glGetUniformLocation(noiseShader.getID(), "texture2");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1.getID());   // lava3.jpg
	glUniform1i(t2L, 0);

	transform.SetPos(glm::vec3(0.0f));
	transform.SetRot(glm::vec3(0.0f, 10.0f, 0.0f));
	transform.SetScale(glm::vec3(1.2f));

	noiseShader.Update(transform, cam);
	mesh1.draw();
}

void MainGame::linkGlowBall()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE); // don't write depth

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // draw backfaces of the shell

	glow.Bind();

	// glow look
	glow.setFloat("rimStart", 0.20f);
	glow.setFloat("rimEnd", 0.95f);
	glow.setFloat("glowPower", 1.0f);
	glow.setFloat("glowStrength", 0.8f);

	glow.setVec3("glowInnerColor", glm::vec3(1.0f, 0.55f, 0.10f));
	glow.setVec3("glowOuterColor", glm::vec3(1.0f, 0.10f, 0.02f));

	float planetScale = 1.2f;
	glm::vec3 ballPos = glm::vec3(0.0f, 0.0f, 0.0f);

	transform.SetPos(ballPos);
	transform.SetRot(glm::vec3(0.0f, 10.0f, 0.0f));
	transform.SetScale(glm::vec3(planetScale * 1.15f));

	glow.setMat4("model", transform.GetModel());

	glow.setVec3("cameraPos", myCamera.getPos());
	glow.setMat4("model", transform.GetModel());
	glow.Update(transform, myCamera);
	mesh1.draw();

	// restore
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void MainGame::linkGlowBallCubemap(const Camera& cam)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	glDisable(GL_CULL_FACE);   // IMPORTANT for cubemap

	glow.Bind();

	glow.setFloat("rimStart", 0.20f);
	glow.setFloat("rimEnd", 0.95f);
	glow.setFloat("glowPower", 1.0f);
	glow.setFloat("glowStrength", 0.8f);

	glow.setVec3("glowInnerColor", glm::vec3(1.0f, 0.55f, 0.10f));
	glow.setVec3("glowOuterColor", glm::vec3(1.0f, 0.10f, 0.02f));

	transform.SetPos(glm::vec3(0.0f));
	transform.SetRot(glm::vec3(0.0f, 10.0f, 0.0f));
	transform.SetScale(glm::vec3(1.2f * 1.15f));

	glow.setVec3("cameraPos", cam.getPos());
	glow.setMat4("model", transform.GetModel());
	glow.Update(transform, cam);
	mesh1.draw();


	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void MainGame::initDynamicCubemap()
{
	// Cubemap texture
	glGenTextures(1, &dynCubeTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, dynCubeTex);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F,
			dynCubeSize, dynCubeSize, 0, GL_RGBA, GL_FLOAT, nullptr);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// FBO + depth
	glGenFramebuffers(1, &dynCubeFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, dynCubeFBO);

	glGenRenderbuffers(1, &dynCubeDepthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, dynCubeDepthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, dynCubeSize, dynCubeSize);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dynCubeDepthRBO);

	// (color attachment set per-face during rendering)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainGame::initCubeMesh()
{
	// position (x,y,z), normal (x,y,z)
	float verts[] = {
		// +Z
		-1,-1, 1,  0,0,1,   1,-1, 1,  0,0,1,   1, 1, 1,  0,0,1,
		-1,-1, 1,  0,0,1,   1, 1, 1,  0,0,1,  -1, 1, 1,  0,0,1,
		// -Z
		-1,-1,-1,  0,0,-1, -1, 1,-1,  0,0,-1,  1, 1,-1,  0,0,-1,
		-1,-1,-1,  0,0,-1,  1, 1,-1,  0,0,-1,  1,-1,-1,  0,0,-1,
		// +X
		 1,-1,-1,  1,0,0,   1, 1,-1,  1,0,0,   1, 1, 1,  1,0,0,
		 1,-1,-1,  1,0,0,   1, 1, 1,  1,0,0,   1,-1, 1,  1,0,0,
		 // -X
		 -1,-1,-1, -1,0,0,  -1,-1, 1, -1,0,0,  -1, 1, 1, -1,0,0,
		 -1,-1,-1, -1,0,0,  -1, 1, 1, -1,0,0,  -1, 1,-1, -1,0,0,
		 // +Y
		 -1, 1,-1,  0,1,0,  -1, 1, 1,  0,1,0,   1, 1, 1,  0,1,0,
		 -1, 1,-1,  0,1,0,   1, 1, 1,  0,1,0,   1, 1,-1,  0,1,0,
		 // -Y
		 -1,-1,-1,  0,-1,0,  1,-1,-1,  0,-1,0,  1,-1, 1,  0,-1,0,
		 -1,-1,-1,  0,-1,0,  1,-1, 1,  0,-1,0, -1,-1, 1,  0,-1,0,
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(2); // normal (use location 2 to match your shaders)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
}

void MainGame::renderDynamicCubemap(const glm::vec3& cubePos)
{
	if (dynCubeFBO == 0 || dynCubeTex == 0)
		return;

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);

	struct Face { glm::vec3 dir; glm::vec3 up; };
	Face faces[6] = {
		{ { 1, 0, 0}, {0,-1, 0} }, // +X
		{ {-1, 0, 0}, {0,-1, 0} }, // -X
		{ { 0, 1, 0}, {0, 0, 1} }, // +Y
		{ { 0,-1, 0}, {0, 0,-1} }, // -Y
		{ { 0, 0, 1}, {0,-1, 0} }, // +Z
		{ { 0, 0,-1}, {0,-1, 0} }, // -Z
	};

	glBindFramebuffer(GL_FRAMEBUFFER, dynCubeFBO);
	glViewport(0, 0, dynCubeSize, dynCubeSize);

	for (int i = 0; i < 6; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dynCubeTex, 0);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Build a temp camera for this face
		Camera cubeCam;
		cubeCam.initCamera(cubePos, 90.0f, 1.0f, 0.1f, 1000.0f);
		cubeCam.setForward(faces[i].dir);
		cubeCam.setUp(faces[i].up);

		// Draw the scene FROM cubeCam, but DO NOT draw the reflective cube itself
		drawSceneForCubemap(cubeCam); // implement next
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, dynCubeTex);
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (int)_gameDisplay.getWidth(), (int)_gameDisplay.getHeight());
}

void MainGame::drawSceneForCubemap(const Camera& cam)
{
	// Draw skybox into the cubemap so reflections have something to show
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	skybox.draw((Camera*)&cam);   // your skybox.draw takes Camera*, so cast is OK here

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Now draw scene objects (but NOT the reflective cube and NOT env-mapped sphere)
	linkEarthPlain(cam);
	linkToon(cam);
	linkGeo(cam);
	linkNoise(cam);

	linkGlowBallCubemap(cam);
}

void MainGame::drawReflectiveCube(const Camera& cam)
{
	reflectiveCubeShader.Bind();

	Transform t;
	t.SetPos(reflectiveCubePos);
	t.SetRot(glm::vec3(0));
	t.SetScale(reflectiveCubeScale);

	reflectiveCubeShader.setMat4("model", t.GetModel());
	reflectiveCubeShader.setMat4("view", cam.getView());
	reflectiveCubeShader.setMat4("projection", cam.getProjection());
	reflectiveCubeShader.setVec3("cameraPos", cam.getPos());
	reflectiveCubeShader.setFloat("reflectStrength", 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, dynCubeTex);
	glUniform1i(glGetUniformLocation(reflectiveCubeShader.getID(), "dynamicCube"), 0);

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void MainGame::drawGame()
{
	// ---- left & right motion for reflective cube ----
	float minX = -5.0f;
	float maxX = 5.0f;
	float moveSpeed = 0.6f;

	// sin(counter * speed) gives -1..1
	float t = sin(counter * moveSpeed);

	// remap -1..1  minX..maxX
	float x = (maxX - minX) * (t * 0.5f + 0.5f) + minX;

	// keep Y and Z fixed
	reflectiveCubePos = glm::vec3(x, 0.0f, 7.5f);

	renderDynamicCubemap(reflectiveCubePos);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	skybox.draw(&myCamera);

	linkEmapping(myCamera);
	linkToon(myCamera);
	linkGeo(myCamera);
	linkNoise(myCamera);
	drawReflectiveCube(myCamera);
	linkGlowBall();


	counter += 0.01f;

	_gameDisplay.swapBuffer();
}