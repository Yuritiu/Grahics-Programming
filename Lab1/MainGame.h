#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "SkyBox.h"
#include <vector>

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void linkFogShader();
	void linkGlowBall();
	void linkGlowBallCubemap(const Camera& cam);
	void linkRimLighting();
	void linkToon(const Camera& cam);
	void linkGeo(const Camera& cam);
	void linkNoise(const Camera& cam);
	void linkEmapping(const Camera& cam);
	void linkEarthPlain(const Camera& cam);
	void initCubeMesh();
	void initDynamicCubemap();
	void renderDynamicCubemap(const glm::vec3& cubePos);
	void drawSceneForCubemap(const Camera& cam);
	void drawReflectiveCube(const Camera& cam);
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);

	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Camera myCamera;
	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader geoShader;
	Shader shaderSkybox;
	Shader eMapping;
	Shader noiseShader;
	Shader pass;
	Shader glow;
	Shader outlineShader;
	Texture texture;
	Texture texture1;
	Texture texture2;
	Texture earthTex;

	Skybox skybox;

	vector<std::string> faces;
	
	//Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
	
	float camSpeed = 0.1f;
	float mouseSensitivity = 0.1f;

	float yaw = -90.0f;  // looking along -Z initially
	float pitch = 0.0f;

	GLuint dynCubeFBO = 0;
	GLuint dynCubeDepthRBO = 0;
	GLuint dynCubeTex = 0;
	int dynCubeSize = 512;

	GLuint cubeVAO = 0, cubeVBO = 0;
	Shader reflectiveCubeShader;

	glm::vec3 reflectiveCubePos = glm::vec3(0.0f, 0.0f, 7.5f);
	glm::vec3 reflectiveCubeScale = glm::vec3(1.6f);
};

