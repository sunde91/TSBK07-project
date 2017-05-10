// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"
#include "camera.h"
#include "init_object.h"

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex, TextureData *scaling, TextureData *waterhole)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
			// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = (tex->imageData[(x + z * tex->width) * (tex->bpp/8)] - waterhole->imageData[(x + z * tex->width) * (tex->bpp/8)]) / (GLfloat)(scaling->imageData[(x + z * tex->width)]/25.0);
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
			// Normal vectors. You need to calculate these.

			if( x > 1 && z > 1 )
			{
				float vx1 = vertexArray[((x) + z * tex->width)*3 + 0];
				float vx2 = vertexArray[((x-1) + z * tex->width)*3 + 0];
				float vx3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 0];
				float vy1 = vertexArray[((x) + z * tex->width)*3 + 1];
				float vy2 = vertexArray[((x-1) + z * tex->width)*3 + 1];
				float vy3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 1];
				float vz1 = vertexArray[((x) + z * tex->width)*3 + 2];
				float vz2 = vertexArray[((x-1) + z * tex->width)*3 + 2];
				float vz3 = vertexArray[((x-1) + (z-1) * tex->width)*3 + 2];

				vec3 v1 = SetVector(vx1 - vx2, vy1 - vy2, vz1 - vz2);
				vec3 v2 = SetVector(vx3 - vx2, vy3 - vy2, vz3 - vz2);
				//vec3 v3 = SetVector(vx2 - vx3, vy2 - vy3, vz2 - vz3);
				vec3 normal = CrossProduct(v1,v2);
				normalArray[(x + z * tex->width)*3 + 0] = normal.x;
				normalArray[(x + z * tex->width)*3 + 1] = normal.y;
				normalArray[(x + z * tex->width)*3 + 2] = normal.z;
			}
			else
			{
				normalArray[(x + z * tex->width)*3 + 0] = 0.0;
				normalArray[(x + z * tex->width)*3 + 1] = 1.0;
				normalArray[(x + z * tex->width)*3 + 2] = 0.0;
			}
// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width * 200.0; // x
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height *  200.0; // y
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	// Normal vectors

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

Model * GenerateWater(TextureData *tex, float waterLevel)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);


	//printf("texture width = (%d,%f), height =(%d,%f)\n",tex->width,tex->texWidth,tex->height,tex->texHeight);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = waterLevel + (((double) rand() / (double) RAND_MAX)-0.5)/2.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
// Normal vectors. You need to calculate these.
			normalArray[(x + z * tex->width)*3 + 0] = 0.0;
			normalArray[(x + z * tex->width)*3 + 1] = 1.0;
			normalArray[(x + z * tex->width)*3 + 2] = 0.0;

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = (float)x / tex->width * 100.0; // x
			texCoordArray[(x + z * tex->width)*2 + 1] = (float)z / tex->height *  100.0; // y
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// Triangle 1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// Triangle 2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}
	// Normal vectors

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

unsigned int nr_of_trees = 1000;
GLfloat *treePos;
int *tree_tex;

unsigned int nr_of_rocks = 15;
GLfloat *rockPos;
int *rockScale;

// vertex array object
Model *m, *m2, *tm, *water, *rock;
Model * skybox;
// Reference to shader program
GLuint program, waterProgram, treeProgram, rockShader;
GLuint skyboxShader;
GLuint grassTex, sandTex, forestTex, mountainTex,snowTex,savannhTex;
GLuint treeTex, treeTex2, treeTex3, treeTex4;
GLuint rockTex;
GLuint skyboxTexObjID;
vec4 skyboxOffset;
mat4 skyboxCameraMatrix;
unsigned int vertexArrayObjID;
TextureData ttex; // terrain
TextureData scale;
TextureData waterhole;
float mouseX, mouseY;
float moveZ, moveX;
Camera camera;

GLfloat treePol[] ={
	-10,0,0,
	-10,30,0,
	10,30,0,
	10,0,0
};

GLuint indexArr[] = {
	0,1,2,
	0,2,3
};

GLuint normalArr[] = {
	0,0,1,
	0,0,1,
	0,0,1,
	0,0,1
};

GLfloat texCoordArr[] = {
	0,1,
	0,0,
	1,0,
	1,1
};

Model *billBoardModel;

void updateKey(unsigned char event, int up)
{
	switch(event)
	{
		case 'w': moveZ = up ? 0 : -1; break;
		case 'a': moveX = up ? 0 : -1; break;
		case 's': moveZ = up ? 0 : 1; break;
		case 'd': moveX = up ? 0 : 1; break;
	}
}
void keyboardCallback(unsigned char event, int x, int y)
{
	updateKey(event, 0);
}
void keyboardCallbackRelease(unsigned char event, int x, int y)
{
	updateKey(event, 1);
}

void getTriangle(float posX, float posZ, int gridWidth, int * index0, int * index1, int * index2)
{
	int indX0 = (int)(posX + 0.5f);
	int indZ0 = (int)(posZ + 0.5f);
	int diffX = indX0 < posX ? 1 : -1;
	int diffZ = indZ0 < posZ ? 1 : -1;

	*index0 = indX0 + indZ0 * gridWidth;
	*index1 = (indX0+diffX) + indZ0 * gridWidth;
	*index2 = indX0 + (indZ0+diffZ) * gridWidth;
}

float calcDist(vec3 a, vec3 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;
	return sqrt( x*x + y*y + z*z);
}

float getHeightExact(Model * model, TextureData * heightMap, float posX, float posZ)
{
	int index0, index1, index2;
	getTriangle(posX, posZ, heightMap->width, &index0, &index1, &index2);

	float vx1 = model->vertexArray[index0*3 + 0];
	float vx2 = model->vertexArray[index1*3 + 0];
	float vx3 = model->vertexArray[index2*3 + 0];
	float vy1 = model->vertexArray[index0*3 + 1];
	float vy2 = model->vertexArray[index1*3 + 1];
	float vy3 = model->vertexArray[index2*3 + 1];
	float vz1 = model->vertexArray[index0*3 + 2];
	float vz2 = model->vertexArray[index1*3 + 2];
	float vz3 = model->vertexArray[index2*3 + 2];

	vec3 v0 = SetVector(posX, 0, posZ);
	vec3 v1 = SetVector(vx1,0,vz1);
	vec3 v2 = SetVector(vx2,0,vz2);
	vec3 v3 = SetVector(vx3,0,vz3);

	float d1 = calcDist(v0,v1);
	float d2 = calcDist(v0,v2);
	float d3 = calcDist(v0,v3);
	float sum = d1+d2+d3;
	d1 /= sum;
	d1 = 1.0 - d1;
	d2 /= sum;
	d1 = 1.0 - d1;
	d3 /= sum;
	d1 = 1.0 - d1;
	sum = d1+d2+d3;
	d1 /= sum;
	d2 /= sum;
	d3 /= sum;
	float y = d1 * vy1 + d2 * vy2 + d3 * vy3;
	return y;
}

float getHeight(TextureData * heightMap, float scaleFactor, float posX, float posZ) {
	if( posX < 0 || posZ < 0 )
		return 0;
	else if( posX > heightMap->width || posZ > heightMap->height )
		return 0;

	int indX = (int)posX;
	int indZ = (int)posZ;
	int ind = ((indX + heightMap->width * indZ) * heightMap->bpp) / 8;
	//printf("posX = %f, posZ = %f, indX = %d, indZ = %d, height = %d\n", posX, posZ, indX, indZ,  heightMap->imageData[ind]);
	return scaleFactor * heightMap->imageData[ ind ];
}

Model * createObject(char ** modelPath)
{
	Model * m = LoadModel(modelPath);

	Model* model = LoadDataToModel(
			m->vertexArray,
			m->normalArray,
			m->texCoordArray,
			NULL,
			m->indexArray,
			m->numVertices,
			m->numIndices);

	return model;
}

void mouseCallback(int mx, int my) {
	mouseX = mx;
	mouseY = my;
	int w,h;
	glutGetWindowSize(&w, &h);
	float wf = ((float)w) / 2.0f;
	float hf = ((float)h) / 2.0f;
	mouseX = ((float)mouseX - wf) / wf;
	mouseY = ((float)mouseY - hf) / hf;
}

void init(void)
{
	// Load terrain data

	LoadTGATextureData("textures/t4-1024.tga", &ttex);

	LoadTGATextureData("textures/scale_matrix.tga", &scale);
	LoadTGATextureData("textures/waterhole.tga", &waterhole);

	tm = GenerateTerrain(&ttex, &scale, &waterhole);
	water = GenerateWater(&ttex, 3.0);
	printError("init terrain");

	// Init Camera
	camera = newCamera();
	camera.pos = SetVector(800,5,800);
	camera.yaw = M_PI /2;

	billBoardModel = LoadDataToModel(
			treePol,
			normalArr,
			texCoordArr,
			NULL,
			indexArr,
			4,
			6);
	int i;
	treePos = malloc(sizeof(GLfloat) * 2 * nr_of_trees);
	tree_tex = malloc(sizeof(int) * nr_of_trees);
	for (i = 0; i < nr_of_trees; i++){
		treePos[i*2 + 0] = ((double) rand() / (double) RAND_MAX)*510.0 + 513.0;
		treePos[i*2 + 1] = ((double) rand() / (double) RAND_MAX)*510.0 + 1.0;
		tree_tex[i] = rand() % 4;
	}

	rockPos = malloc(sizeof(GLfloat) * 2 * nr_of_rocks);
	rockScale = malloc(sizeof(GLfloat) * nr_of_rocks);
	for (i = 0; i < nr_of_rocks; i++){
		rockPos[i*2 + 0] = ((double) rand() / (double) RAND_MAX)*510.0 + 513.0;
		rockPos[i*2 + 1] = ((double) rand() / (double) RAND_MAX)*510.0 + 513.0;

		rockScale[i] = ((double) rand() / (double) RAND_MAX)*6.0 + 1;
	}


	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGenVertexArrays(1, &vertexArrayObjID);


	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1500.0);

	// Load and compile shader
	program = loadShaders("shaders/terrain.vert", "shaders/terrain.frag");
	rockShader = loadShaders("shaders/rock.vert", "shaders/rock.frag");
	waterProgram = loadShaders("shaders/water.vert", "shaders/water.frag");
	skyboxShader = loadShaders("shaders/skybox2.vert","shaders/skybox2.frag");
	treeProgram = loadShaders("shaders/tree.vert","shaders/tree.frag");

	glUseProgram(program);
	printError("init shader");

	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniform1i(glGetUniformLocation(program, "grassUnit"), 0); // Texture unit 0
	LoadTGATextureSimple("textures/grass.tga", &grassTex);


	LoadTGATextureSimple("textures/sand1.tga", &sandTex);
	glUniform1i(glGetUniformLocation(program, "sandUnit"), 1); // Texture unit 1
	printError("program shader");

	LoadTGATextureSimple("textures/forestfloor.tga", &forestTex);
	glUniform1i(glGetUniformLocation(program, "forestUnit"), 2); // Texture unit 1


	LoadTGATextureSimple("textures/mountain.tga", &mountainTex);
	glUniform1i(glGetUniformLocation(program, "mountainUnit"), 3); // Texture unit 1


	LoadTGATextureSimple("textures/snow.tga", &snowTex);
	glUniform1i(glGetUniformLocation(program, "snowUnit"), 4); // Texture unit 1


	LoadTGATextureSimple("textures/savannh.tga", &savannhTex);
	glUniform1i(glGetUniformLocation(program, "savannhUnit"), 5); // Texture unit 1




	glUseProgram(skyboxShader);
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	m2 = createObject("models/skybox.obj");
	CenterModel(m2);

	glUseProgram(rockShader);
	glUniformMatrix4fv(glGetUniformLocation(rockShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	rock = createObject("models/rock.obj");
	CenterModel(rock);
	LoadTGATextureSimple("textures/rock.tga", &rockTex);
	glUniform1i(glGetUniformLocation(rockShader, "tex"), 0);



	//glUniform1i(glGetUniformLocation(ball, "tex"), 0); // Texture unit 0
	printError("ball shader");

	glUseProgram(waterProgram);
	glUniformMatrix4fv(glGetUniformLocation(waterProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	printError("water shader");

	/*
	glUseProgram(skyboxShader);
	skybox = createObject("models/skybox.obj");

	CenterModel(skybox);
    //init_object(vertexArrayObjID, skybox, skyboxShader);
    */
    LoadTGATextureSimple("textures/SkyBox512.tga", &skyboxTexObjID);
    skyboxOffset.x = 0;
    skyboxOffset.y = -0.2;
    skyboxOffset.z = 0;
    //glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUseProgram(treeProgram);
	glUniformMatrix4fv(glGetUniformLocation(treeProgram, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);

	LoadTGATextureSimple("textures/tree.tga", &treeTex);
	LoadTGATextureSimple("textures/tree2.tga", &treeTex2);
	LoadTGATextureSimple("textures/tree3.tga", &treeTex3);
	LoadTGATextureSimple("textures/tree4.tga", &treeTex4);
	glUniform1i(glGetUniformLocation(treeProgram, "treeUnit"), 0);


	init_object(vertexArrayObjID, billBoardModel, treeProgram);
	CenterModel(billBoardModel);
}

void updateCameraStuff() {

	cameraSetRotateVel(&camera, mouseY, mouseX);
	cameraSetMoveVel(&camera, moveX, 0, moveZ);
	//cameraSetTargetY(&camera, 5.0 + getHeight(&ttex,1.0 / 5.0, camera.pos.x, camera.pos.z));
	float currentHeight = getHeightExact(tm, &ttex, camera.pos.x, camera.pos.z);
	cameraSetTargetY(&camera, 5.0 + currentHeight);
	//printf("camera pos = %f,%f,%f\n",camera.pos.x,camera.pos.y, camera.pos.z);
	updateCamera(&camera);
}

GLuint get_tree_tex(int i){
	int j = tree_tex[i];
	switch(j){
		case 0: return treeTex;
		case 1: return treeTex2;
		case 2: return treeTex3;
		case 3: return treeTex4;
	}
}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	updateCameraStuff();
	mat4 total, modelView, camMatrix;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, skyboxTexObjID);

	glBindTexture(GL_TEXTURE_2D, skyboxTexObjID);
	glUseProgram(skyboxShader);
	skyboxCameraMatrix = camera.matrix;
  skyboxCameraMatrix.m[3] = skyboxOffset.x;
  skyboxCameraMatrix.m[4 + 3] = skyboxOffset.y;
  skyboxCameraMatrix.m[8 + 3] = skyboxOffset.z;
   // glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "cameraMatrix"), 1, GL_TRUE, skyboxCameraMatrix.m);

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "cameraMatrix"), 1, GL_TRUE, skyboxCameraMatrix.m);
	DrawModel(m2,program,"inPosition", "inNormal", "inTexCoord");
	glClear(GL_DEPTH_BUFFER_BIT);
	printError("pre display");


	// Build matrix
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	camMatrix = camera.matrix;
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);

	glBindTexture(GL_TEXTURE_2D, grassTex);		// Bind Our Texture grassTex
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sandTex);		// Bind Our Texture grassTex
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, forestTex);		// Bind Our Texture grassTex
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mountainTex);		// Bind Our Texture grassTex
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, snowTex);		// Bind Our Texture grassTex
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, savannhTex);		// Bind Our Texture grassTex

	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");
	printError("display 1");

	glUseProgram(rockShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rockTex);

	int i;
	float scale;
	for (i = 0; i < nr_of_rocks; i++){
		GLfloat x = rockPos[i*2 + 0];
		GLfloat z = rockPos[i*2 + 1];
		scale = rockScale[i];
	float y = getHeightExact(tm, &ttex, x,z);
	if(y < 5){
		continue;
	}

	modelView = Mult(T(x,y,z), S(scale,scale,scale));
	total = Mult(camMatrix, modelView); // Mult(camMatrix, modelView);
	//GLfloat gly = y;
	glUniformMatrix4fv(glGetUniformLocation(rockShader, "mdlMatrix"), 1, GL_TRUE, total.m);
	DrawModel(rock,rockShader,"inPosition", "inNormal", "inTexCoord");
	}

	// draw water
	glUseProgram(waterProgram);
	GLuint loc = glGetAttribLocation(waterProgram, "inPosition"); //printf("inPos = %d\n", loc);
	loc = glGetAttribLocation(waterProgram, "inNormal"); //printf("inNormal = %d\n", loc);
	loc = glGetAttribLocation(waterProgram, "inTexCoord"); //printf("inTex = %d\n", loc);
	glEnable(GL_BLEND);
	glUniformMatrix4fv(glGetUniformLocation(waterProgram, "mdlMatrix"), 1, GL_TRUE, camera.matrix.m);
	DrawModel(water, waterProgram, "inPosition", "inNormal", "inTexCoord");
	glDisable(GL_BLEND);

	// Billboard
	glUseProgram(treeProgram);
	glActiveTexture(GL_TEXTURE0);



	for (i = 0; i < nr_of_trees; i++){
		GLfloat x = treePos[i*2 + 0];
		GLfloat z = treePos[i*2 + 1];
		float y = getHeightExact(tm, &ttex, x,z);
		if( y > 60 )
			continue;
		mat4 t = T(x,y,z);
		t = Mult(camera.matrix,t);
		t = T(t.m[3],t.m[7],t.m[11]);

		GLuint TEX = get_tree_tex(i);
		glBindTexture(GL_TEXTURE_2D, TEX);

		glUniformMatrix4fv(glGetUniformLocation(treeProgram, "modelMatrix"), 1, GL_TRUE, t.m);
		//glBindVertexArray(billBoardModel->vertexArray);
		DrawModel(billBoardModel,treeProgram,"in_Position","in_Normal", "in_TexCoord");
	}
	printError("display 2");
	glutSwapBuffers();
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (1600, 900);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardCallbackRelease);
	glutPassiveMotionFunc(mouseCallback);

	glutMainLoop();
	exit(0);
}
