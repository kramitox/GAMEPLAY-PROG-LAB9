#include <Game.h>

static bool flip;
const int numOfVertex = 8;
const int numOfTriangles = 36;
Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
} Vertex;

Vertex vertex[numOfVertex];
GLubyte triangles[numOfTriangles];

/* Variable to hold the VBO identifier and shader data */
GLuint	index, //Index to draw
		vsid, //Vertex Shader ID
		fsid, //Fragment Shader ID
		progID, //Program ID
		vao = 0, //Vertex Array ID
		vbo[1], // Vertex Buffer ID
		positionID, //Position ID
		colorID; // Color ID


void Game::initialize()
{
	isRunning = true;
	
	glewInit();

	/* Vertices counter-clockwise winding */
	vertex[0].coordinate[0] = -0.5f;
	vertex[0].coordinate[1] = -0.5f;
	vertex[0].coordinate[2] = 0.0f;

	vertex[1].coordinate[0] = 0.5f;
	vertex[1].coordinate[1] = -0.5f;
	vertex[1].coordinate[2] = 0.0f;

	vertex[2].coordinate[0] = 0.5f;
	vertex[2].coordinate[1] = 0.5f;
	vertex[2].coordinate[2] = 0.0f;

	vertex[3].coordinate[0] = -0.5f;
	vertex[3].coordinate[1] = 0.5f;
	vertex[3].coordinate[2] = 0.0f;

	vertex[4].coordinate[0] = -0.5f;
	vertex[4].coordinate[1] = -0.5f;
	vertex[4].coordinate[2] = 1.0f;

	vertex[5].coordinate[0] = 0.5f;
	vertex[5].coordinate[1] = -0.5f;
	vertex[5].coordinate[2] = 1.0f;

	vertex[6].coordinate[0] = 0.5f;
	vertex[6].coordinate[1] = 0.5f;
	vertex[6].coordinate[2] = 1.0f;

	vertex[7].coordinate[0] = -0.5f;
	vertex[7].coordinate[1] = 0.5f;
	vertex[7].coordinate[2] = 1.0f;
	
	for (size_t i = 0; i < numOfVertex; i++)
	{
		vertex[i].color[0] = 0.0f;
		vertex[i].color[1] = 0.0f;
		vertex[i].color[2] = 0.0f;
		vertex[i].color[3] = 1.0f;
	}
	/*Index of Poly / Triangle to Draw */
	//FRONT FACE
	triangles[0] = 0;   triangles[1] = 1;   triangles[2] = 2;
	triangles[3] = 0;   triangles[4] = 2;	triangles[5] = 3;

	//BACK FACE
	triangles[6] = 4;   triangles[7] = 6;   triangles[8] = 7;
	triangles[9] = 5;   triangles[10]= 6;  triangles[11]= 4;
	//TOP FACE
	triangles[12] = 3;  triangles[13] = 2; triangles[14] = 6;
	triangles[15] = 3;  triangles[16] = 6; triangles[17] = 7;
	//BOTTOM FACE
	triangles[18] = 0;  triangles[19] = 1; triangles[20] = 5;
	triangles[21] = 0;  triangles[22] = 5; triangles[23] = 4;
	//LEFT FACE
	triangles[24] = 4;  triangles[25] = 0; triangles[26] = 7;
	triangles[27] = 0;  triangles[28] = 3; triangles[29] = 7;
	//RIGHT FACE
	triangles[30] = 5;  triangles[31] = 6; triangles[32] = 1;
	triangles[33] = 1;  triangles[34] = 6; triangles[35] = 2;

	GLint isCompiled = 0;
	GLint isLinked = 0;


	// Read shaders
	//std::string vertShaderStr = readFile(vertex_path);

	//const char *vertShaderSrc = vertShaderStr.c_str();
	
	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 7, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * numOfTriangles, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	ifstream vertexFile;
	std::string vString;
	std::stringstream vContent;
	vertexFile.open("vertexShader.txt");
	vContent << vertexFile.rdbuf();
	vertexFile.close();
	vString = vContent.str();
	const char* vs_src = vString.c_str();

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

						   //Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */


	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);

	ifstream fragFile;
	std::string fString;
	std::stringstream fContent;
	fragFile.open("fragmentShader.txt");
	fContent << fragFile.rdbuf();
	fragFile.close();
	fString = fContent.str();
	const char* fs_src = fString.c_str();
	
	glShaderSource(fsid, 1, &fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
}

void Game::update()
{
	
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();

		if (!flip)
		{
			flip = true;
		}
		else
			flip = false;
	}

	if (flip)
	{
		rotationAngle += 0.005f;

		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}

	//Change vertex data
	/*vertex[0].coordinate[0] += -0.0001f;
	vertex[0].coordinate[1] += -0.0001f;
	vertex[0].coordinate[2] += -0.0001f;

	vertex[1].coordinate[0] += -0.0001f;
	vertex[1].coordinate[1] += -0.0001f;
	vertex[1].coordinate[2] += -0.0001f;

	vertex[2].coordinate[0] += -0.0001f;
	vertex[2].coordinate[1] += -0.0001f;
	vertex[2].coordinate[2] += -0.0001f;*/

	Matrix rotationX(cos(0.0005), 0, sin(0.0005), 0, 1, 0, -sin(0.0005), 0, cos(0.0005));
	Matrix rotationY(1, 0, 0, 0, cos(0.0005), sin(0.0005), 0, -sin(0.0005), cos(0.0005));
	for (size_t i = 0; i < numOfVertex; i++)
	{
		if (vertex[i].coordinate[2] == 1.0f)
		{
			customVector::Vector3 tempVect(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2] + 0.5f);
			tempVect.Equals(rotationX* tempVect);
			tempVect.Equals(rotationY * tempVect);
			vertex[i].coordinate[0] = tempVect.x;
			vertex[i].coordinate[1] = tempVect.y;
			vertex[i].coordinate[2] = tempVect.z - 0.5f;
		}
		else
		{
			customVector::Vector3 tempVect(vertex[i].coordinate[0], vertex[i].coordinate[1], vertex[i].coordinate[2] - 0.5f);
			tempVect.Equals(rotationX* tempVect);
			tempVect.Equals(rotationY * tempVect);
			vertex[i].coordinate[0] = tempVect.x;
			vertex[i].coordinate[1] = tempVect.y;
			vertex[i].coordinate[2] = tempVect.z + 0.5f;
		}
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfTriangles, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glDrawElements(GL_TRIANGLES, numOfTriangles, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
}
