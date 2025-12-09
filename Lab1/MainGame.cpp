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
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	//eMapping.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	noiseShader.init("..\\res\\noise.vert", "..\\res\\noise.frag"); //new shader
	pass.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	texture.init("..\\res\\noise.png"); //load texture
	texture1.init("..\\res\\lava3.jpg"); //load texture
	//texture2.init("..\\res\\Water.jpg"); //load texture
	glow.init("..\\res\\glow.vert", "..\\res\\glow.frag");
	//geoShader.initGeo();

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

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
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
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkGeo()
{
	float randX = ((float)rand() / (RAND_MAX));
	float randY = ((float)rand() / (RAND_MAX));
	float randZ = ((float)rand() / (RAND_MAX));
	// Frag: uniform float randColourX; uniform float randColourY; uniform float randColourZ;
	geoShader.setFloat("randColourX", randX);
	geoShader.setFloat("randColourY", randY);
	geoShader.setFloat("randColourZ", randZ);
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
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
	//eMapping.setMat4("projection", myCamera.getProjection());
	//eMapping.setMat4("view", myCamera.getView());
	eMapping.setMat4("model", transform.GetModel());
	eMapping.setVec3("cameraPos", myCamera.getPos());

	GLuint t1L = glGetUniformLocation(eMapping.getID(), "diffuse"); //texture 1 location

	//set textures
	glActiveTexture(GL_TEXTURE0); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, texture2.getID());
	glUniform1i(t1L, 0);

	transform.SetPos(glm::vec3(-sinf(counter / 20), 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter / 10, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	eMapping.Update(transform, myCamera);
	mesh1.draw();
}

void MainGame::linkNoise()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	noiseShader.Bind();
	noiseShader.setFloat("time", counter / 20);
	noiseShader.setFloat("fogDensity", 0.5);
	noiseShader.setVec3("fogColor", 0.0f, 0.0f, 0.5f);
	noiseShader.setFloat("maxDist", 10.0f);
	noiseShader.setFloat("minDist", 0.0f);
	GLuint t1L = glGetUniformLocation(noiseShader.getID(), "texture1"); //texture 1 location
	GLuint t2L = glGetUniformLocation(noiseShader.getID(), "texture2");

	//set textures
	glActiveTexture(GL_TEXTURE0); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, texture.getID());
	glUniform1i(t1L, 0);

	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, texture1.getID());
	glUniform1i(t2L, 1);

	//type of and texture to bind to unit


	transform.SetPos(glm::vec3(0.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, 10, 0.0));
	transform.SetScale(glm::vec3(1.2, 1.2, 1.2));

	myCamera.MoveRight(0.0001);

	noiseShader.Update(transform, myCamera);



	mesh1.draw();
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);
	skybox.draw(&myCamera);
	//linkEmapping();
	//linkFogShader();
	//linkToon();
	//linkRimLighting();
	linkNoise();

	myCamera.MoveRight(0.0001);
	myCamera.setLook(mesh1.getSpherePos());

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