#include "starpirate.h"

GLuint VAO=0;

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

	//point size is set in vertex shader
	glEnable(GL_PROGRAM_POINT_SIZE);

	//create a VBO with a triangle in it
	{
		GLfloat verts[]={	-0.05f, -0.05f, 0.0f,
					 0.0f,  0.05f, 0.0f,
					 0.05f, -0.05f, 0.0f,
				};
		unsigned int numVerts=9;

		glGenBuffers(1, &planetVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVerts, verts, GL_STATIC_DRAW);
	}

	//VBO for links
	{
		//glGenBuffers(1, &linkVBO);
		//glBindBuffer(GL_ARRAY_BUFFER, linkVBO);
		//not calling BufferData here because we're going to change it every frame
	}

	//default shaders
	{
		if(loadShaderFromFile("default.vert", &vertShader, GL_VERTEX_SHADER)) return 1;
		if(loadShaderFromFile("colour.frag", &fragShader, GL_FRAGMENT_SHADER)) return 1;
		if(createProgramWith2Shaders(&colourProgram, &vertShader, &fragShader))
		{
			printf("Error building default shaders\n");
			exit(1);
		}
	}

	//shaders for the lines
	{
		if(loadShaderFromFile("nomat.vert", &lineVertShader, GL_VERTEX_SHADER)) return 1;
		if(createProgramWith2Shaders(&lineProgram, &lineVertShader, &fragShader))
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

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteProgram(colourProgram);

	SDL_DestroyWindow(*window);
}

