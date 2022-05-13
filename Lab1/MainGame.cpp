#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;
Transform transform1;
Transform transform3;
Transform transformLight;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	Shader fogShader();
	Shader toonShader();
	Shader rimShader();
	Shader geoShader();
	Shader reflect();
	Shader basicShader();
	Shader phongShader();
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
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	meshOWN.loadModel("..\\res\\Caixote.obj");
	meshBowl.loadModel("..\\res\\sphere2.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\shaderRim.vert", "..\\res\\shaderRim.frag");
	reflect.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");
	basicShader.init("..\\res\\shader.vert", "..\\res\\shader.frag");
	phongShader.init("..\\res\\shaderPhong.vert", "..\\res\\shaderPhong.frag");

	geoShader.initGeo();

	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);
	
	inputCounter = 0;
	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\Yokohama\\right.jpg",
		"..\\res\\Yokohama\\left.jpg",
		"..\\res\\Yokohama\\top.jpg",
		"..\\res\\Yokohama\\bottom.jpg",
		"..\\res\\Yokohama\\front.jpg",
		"..\\res\\Yokohama\\back.jpg"
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
		case SDL_KEYDOWN: //in the case that a key is being held
			switch (evnt.key.keysym.sym)
			{
			case SDLK_LEFT:
				if (inputCounter > 0)
				{
					inputCounter = inputCounter - 1;
				}
				break;
			case SDLK_RIGHT:
				if (inputCounter < 2)
				{
					inputCounter = inputCounter + 1;
				}
				break;
			case SDLK_a:
				if (inputCounter > 0)
				{
					inputCounter = inputCounter - 1;
				}
				break;
			case SDLK_d:
				if (inputCounter < 2)
				{
					inputCounter = inputCounter + 1;
				}
				break;
			case SDLK_ESCAPE:
				_gameState = GameState::EXIT; //quits application
				break;
			default:
				break;
			}
			break;
		

		}
	}

}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{

		return true;
	}
	else
	{
		return false;
	}
}

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
	
	// Geom: uniform float time;
	geoShader.setFloat("time", counter);
}

void MainGame::linkReflect()
{
	reflect.setMat4("model", transform.GetModel());
	reflect.setMat4("view", myCamera.getView());
	reflect.setMat4("projection", myCamera.getProjection());
	reflect.setInt("skybox", 0);
	reflect.setVec3("cameraPos", myCamera.getPos());
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

void MainGame::linkPhong()
{
	
	phongShader.setMat4("projection", myCamera.getProjection());
	phongShader.setMat4("modelView", transform3.GetModel()); //
	phongShader.setMat4("normalMatrix", myCamera.getViewProjection()); 
	
	phongShader.setFloat("ambientAmount", 0.7f);
	phongShader.setFloat("diffuseAmount", 0.8f);
	phongShader.setFloat("specularAmount", 0.97f);
	phongShader.setFloat("amountOfShine", 100.0f);
	phongShader.setVec3("ambientColour", glm::vec3(0.204, 0.098, 0.0));
	phongShader.setVec3("diffuseColour", glm::vec3(0.8, 0.4, 0.0));
	phongShader.setVec3("specularColour", glm::vec3(1.0, 1.0, 1.0));
	phongShader.setVec3("lightPosition", *transformLight.GetPos());
	
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(1.0f, 1.0f, 1.0f, 1.0f); //sets our background colour

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture
	Texture textureBox("..\\res\\box2.jpg");
	Texture textureMoon("..\\res\\MoonTex4.jpg");

	if (inputCounter == 0)
	{

		reflect.Bind();
		linkReflect();

		texture1.Bind(0);
		reflect.Update(transform, myCamera);


		mesh1.draw();
		mesh1.updateSphereData(*transform.GetPos(), 0.62f);

		texture.Bind(0);
		transform.SetPos(glm::vec3(0.0, -sinf(counter), 0.0));
		transform.SetRot(glm::vec3(0.5, counter * 5, 0.2));
		transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

		mesh2.draw();
		mesh2.updateSphereData(*transform.GetPos(), 0.62f);

		

	}

	if (inputCounter == 1)
	{

		transform.SetPos(glm::vec3(0.0, -sinf(counter), 0.0));
		transform.SetRot(glm::vec3(0.5, counter * 5, 0.2));
		transform.SetScale(glm::vec3(0.5, 0.5, 0.5));

		textureMoon.Bind(0);
		geoShader.Bind();

		linkGeo();


		geoShader.Update(transform, myCamera);
		

		meshBowl.draw();
		meshBowl.updateSphereData(*transform.GetPos(), 0.62f);
	}
	
	if (inputCounter == 2)
	{
		//std::cout << -sinf(counter)<< std::endl;

		transform3.SetPos(glm::vec3(-0.3, -0.5, -2.5));
		transform3.SetRot(glm::vec3(0, counter * 2, 0));
		transform3.SetScale(glm::vec3(0.3, 0.3, 0.3));

		transformLight.SetPos(*transform3.GetPos() + glm::vec3(0.1, 0.01, 0.25));
		

		
		
		
		phongShader.Bind();
		
		linkPhong();
		
		phongShader.Update(transform3, myCamera);
		
		

		mesh1.draw();
		mesh1.updateSphereData(*transform3.GetPos(), 0.62f);
	}
	

	

	counter = counter + 0.04f;

	
	skybox.draw(&myCamera);	
	

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();


	_gameDisplay.swapBuffer();	
	

} 