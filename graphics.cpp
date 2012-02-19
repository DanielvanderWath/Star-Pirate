#include "starpirate.h"
#include <iostream>
#include <fstream>

GLuint VAO=0;

using namespace std;

bool checkShaderCompileStatus(GLuint shader)
{//check for a failed compile, and print the log
	int status;
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		GLERR("glGetShaderiv shader compile status");

	if(status==GL_FALSE)
	{
		int loglength;
		char *log=NULL;
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglength);
			GLERR("glGetShaderiv GL_INFO_LOG_LENGTH");
		
		log=(char*)malloc(sizeof(char)*loglength);
		if(log)
		{
			glGetShaderInfoLog(shader, loglength, NULL, log);
				GLERR("glGetShaderInfoLog");
			printf("shader failed to compile: %s", log);
			free(log);
		}
		else
			printf("not enough memory to print info log");
	}	
	return true;
}

int loadPPMIntoTexture(GLuint *texture, const char *filename)
{
	ifstream file;
	unsigned char *texData=NULL, *buffer=NULL;
	unsigned long fileLength=0;
	std::string magicNumber;
	unsigned int width, height, twofivefive;

	if(!filename)
	{
		printf("Cannot open file when filename is NULL.\n");
		return 1;
	}

	file.open(filename, ios::in | ios::binary);
	if(!file.is_open())
	{
		printf("Failed to open file: %s", filename);
		return 1;
	}

	//get length of file
	file.seekg(0, ios::end);
	fileLength=file.tellg();
	file.seekg(0, ios::beg);

	file >> magicNumber >> width >> height >> twofivefive;

	if(magicNumber != "P6")
	{
		printf("Bad magic number. I don't think this is a PPM\n");
		return 1;
	}

	if(twofivefive != 255)
	{
		printf("Something's up, I've misread the file\n");
		return 1;
	}

	//allocate a buffer to read it into
	buffer= new unsigned char[width*height*3];
	texData= new unsigned char[width*height*4];
	if(!buffer || !texData)
	{
		file.close();
		printf("Failed to allocate buffer to read file into. Maybe it's too big\n");
		return 1;
	}

	//read the file into the buffer
	file >> buffer;
	file.close();

	//copy buffer to texData, pure black is the alpha colour
	for(int i=0; i<width*height; i++)
	{
		texData[i*4 + 0] = buffer[i*3 + 0];
		texData[i*4 + 1] = buffer[i*3 + 1];
		texData[i*4 + 2] = buffer[i*3 + 2];

		if((texData[i*4 + 0] == 0) && (texData[i*4 + 1] == 0) &&  (texData[i*4 + 2] == 0))
			texData[i*4 + 3] = 0;
		else
			texData[i*4 + 3] = 255;
	}

	if(!*texture)
	{
		//if the texture handle is NULL, create a texture with some
		//default parameters
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			GLERR("Generating texture");
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
		GLERR("glTexImage2D");

	delete buffer;
	delete texData;
	return 0;
}

int loadShaderFromFile(const char* filename, GLuint *shader, GLuint shaderType)
{//load a shader from filename.
	FILE *file;
	char *source=NULL;
	int length;

	if(filename)
	{
		file=fopen(filename, "r");
		if(file)
		{
			fseek(file, 0, SEEK_END);
			length=ftell(file);
			source=(char*)malloc(sizeof(char)*(length+1));
			if(!source)
			{
				printf("not enough memory to load shader: %s\n", filename);
				fclose(file);
				return 1;
			}
			rewind(file);
			fread(source, 1, length, file);
			fclose(file);
			
			//NULL terminator for the shader source
			source[length]=0;
		}
		else
		{
			printf("couldn't find %s shader file: %s", shaderType==GL_VERTEX_SHADER ? "vertex" : "fragment", filename);
		}
		*shader= glCreateShader(shaderType);
			GLERR("glCreateShader");
		
		glShaderSource(*shader, 1, (const GLchar**)&source, NULL);
			GLERR("glShaderSource");

		glCompileShader(*shader);
			GLERR("glCompilerShader"); 
		checkShaderCompileStatus(*shader);
		free(source);
	}		
	else
		return 1;
	return 0;
}

int createProgramWith2Shaders(GLuint *program, GLuint *vert, GLuint *frag)
{
	//program
	*program=glCreateProgram();
		GLERR("glCreateProgram");

	glAttachShader(*program, *vert);
		GLERR("glAttachShader(vertex)");

	glAttachShader(*program, *frag);
		GLERR("glAttachShader(fragment)");

	//link and check for errors
	glLinkProgram(*program);
		GLERR("glLinkProgram");
	if(checkProgramLinkStatus(*program)) {printf("program failed to link\n"); return 1;}
		
	glUseProgram(*program);
		GLERR("glUseProgram");

	return 0;
}

int checkProgramLinkStatus(GLuint program)
{
	//see if it linked properly
	{
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
			GLERR("glGetProgramiv");

		if(status==GL_FALSE)
		{
			GLint loglength;
			char *log=NULL;	
			printf("Program linking failed\n");
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &loglength);
				GLERR("glGetProgramiv loglength");

			log=(char*)malloc(sizeof(char)*loglength);
			if(log)
			{
				glGetProgramInfoLog(program, loglength, NULL, log);
					GLERR("glGetProgramInfoLog");

				printf("program info log:%s\n", log);

				free(log);
			}
		}
	}
	return 0;
}


int graphicsInit(SDL_Window **window)
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Failed to initialise SDL\n");
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //double buffering on obviously
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	*window=SDL_CreateWindow("Star Pirate", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINWIDTH, WINHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN); 
		if((window)==NULL)
			SDLERR("SDL_CreateWindow");	

	contextGL = SDL_GL_CreateContext(*window);
	SDL_GL_SetSwapInterval(1);

	//VAO
	glGenVertexArrays(1, &VAO);
		GLERR("glGenVertexArrays");
	glBindVertexArray(VAO);
		GLERR("glBindVertexArray");
	//depth
	glEnable(GL_DEPTH_TEST);
		GLERR("glEnable GL_DEPTH_TEST");
	glDepthFunc(GL_LEQUAL);	
		GLERR("glDepthFunc");

	//alpha testing
	glEnable(GL_BLEND);
	//glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	//point size is set in vertex shader
	glEnable(GL_PROGRAM_POINT_SIZE);
		GLERR("enable GL_PROGRAM_POINT_SIZE");
		
	//generate a vbo to use for the planet later
	glGenBuffers(1, &planetVBO);

	//default shaders
	{
		if(loadShaderFromFile("default.vert", &vertShader, GL_VERTEX_SHADER)) return 1;
		if(loadShaderFromFile("texture.frag", &fragShader, GL_FRAGMENT_SHADER)) return 1;
		if(createProgramWith2Shaders(&planetProgram, &vertShader, &fragShader))
		{
			printf("Error building default shaders\n");
			exit(1);
		}
	}

	//shaders for the lines
	{
		if(loadShaderFromFile("nomat.vert", &lineVertShader, GL_VERTEX_SHADER)) return 1;
		if(loadShaderFromFile("colour.frag", &lineFragShader, GL_FRAGMENT_SHADER)) return 1;
		if(createProgramWith2Shaders(&lineProgram, &lineVertShader, &lineFragShader))
		{
			printf("Error building line shaders\n");
			exit(1);
		}
	}

	return 0;
}

void graphicsClean(SDL_Window **window)
{
	glDeleteBuffers(1, &planetVBO);
	glDeleteTextures(1, &planetTexture);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(planetProgram);

	SDL_DestroyWindow(*window);
}

