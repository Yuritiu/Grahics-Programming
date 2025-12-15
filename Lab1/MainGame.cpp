#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Texture texture(); //load texture
	Texture texture1(); //load texture
	//Audio* audioDevice();
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
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\ball.obj"); //ball.obj for the sphere
	mesh2.loadModel("..\\res\\monkey3.obj");
	mesh3.loadModel("..\\res\\Skull.obj"); 
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	outlineShader.init("..\\res\\outline.vert", "..\\res\\outline.frag");
	eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	noiseShader.init("..\\res\\noise.vert", "..\\res\\noise.frag"); //new shader
	pass.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	texture.init("..\\res\\noise.png"); //load texture
	texture1.init("..\\res\\lava3.jpg"); //load texture
	texture2.init("..\\res\\Water.jpg"); //load texture
	earthTex.init("..\\res\\earth.jpg");
	glow.init("..\\res\\glow.vert", "..\\res\\glow.frag");
	geoShader.initGeo();

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

void MainGame::linkToon()
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

	toonShader.Update(transform, myCamera);
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

	outlineShader.Update(transform, myCamera);
	mesh2.draw();

	// Restore default culling
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}


void MainGame::linkGeo()
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

	geoShader.Update(transform, myCamera);

	// bind texture for skull
	GLint loc = glGetUniformLocation(geoShader.getID(), "diffuse");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1.getID()); 
	glUniform1i(loc, 0);

	mesh3.draw();
}



void MainGame::linkRimLighting()
{
	glm::vec3 camDir;
	camDir = mesh2.getSpherePos() - myCamera.getPos();
	camDir = glm::normalize(camDir);
	rimShader.setMat4("u_pm", myCamera.getProjection());
	rimShader.setMat4("u_vm", myCamera.getView());
	rimShader.setMat4("model", transform.GetModel());
	rimShader.setMat4("view", myCamera.getView());
	rimShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkEmapping()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	eMapping.Bind();

	// Orbit parameters
	float radius = 3.0f;
	float orbitSpeed = 1.0f;
	float orbitAngle = counter * orbitSpeed;

	float x = radius * cos(orbitAngle);
	float z = radius * sin(orbitAngle);

	// rotation of the ball itself
	float spinSpeed = 3.0f;
	float spinAngle = counter * spinSpeed;

	transform.SetPos(glm::vec3(x, 0.0f, z));
	transform.SetRot(glm::vec3(0.0f, spinAngle, 180.0f));   // spin around Y
	transform.SetScale(glm::vec3(0.6f));

	eMapping.setVec3("cameraPos", myCamera.getPos());

	// Bind Earth texture
	GLint t1L = glGetUniformLocation(eMapping.getID(), "diffuse");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTex.getID());
	glUniform1i(t1L, 0);

	// Upload MVP etc.
	eMapping.Update(transform, myCamera);

	mesh1.draw();
}

void MainGame::linkNoise()
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

	noiseShader.Update(transform, myCamera);
	mesh1.draw();
}

void MainGame::linkGlowBall()
{
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);


	glow.Bind();

	// Use the SAME transform as the lava sphere, but slightly larger
	transform.SetPos(glm::vec3(0.0f));
	transform.SetRot(glm::vec3(0.0f, 10.0f, 0.0f));
	transform.SetScale(glm::vec3(1.2f * 1.03f));

	glow.setVec3("glowColor", glm::vec3(1.0f, 0.35f, 0.15f)); // orange-red
	glow.setFloat("glowPower", 3.0f);                         // higher = thinner rim
	glow.setFloat("glowStrength", 0.8f);                      // alpha strength

	glow.Update(transform, myCamera);
	mesh1.draw();

	// Restore defaults
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	skybox.draw(&myCamera);
	linkEmapping();
	//linkFogShader();
	linkToon();
	linkGeo(); // exploding skull (mesh3)
	//linkRimLighting();
	linkNoise();
	linkGlowBall();   

	//myCamera.MoveRight(0.0001);
	//myCamera.setLook(mesh1.getSpherePos());

	//GLuint t1L = glGetUniformLocation(pass.getID(), "diffuse"); //texture 1 location
	//GLuint t2L = glGetUniformLocation(pass.getID(), "diffuse1");

	////set textures
	//glActiveTexture(GL_TEXTURE0); //set acitve texture unit
	//glBindTexture(GL_TEXTURE_2D, texture.getID());
	//glUniform1i(t1L, 0);

	//glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	//glBindTexture(GL_TEXTURE_2D, texture1.getID());
	//glUniform1i(t2L, 1);

	//mesh1.updateSphereData(*transform.GetPos(), 0.62f);
		
	counter = counter + 0.01f;
	//cout << sinf(counter) << '\n';

	

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();


	_gameDisplay.swapBuffer();	
	

} 