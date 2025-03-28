OPENGLLIB= -framework OpenGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/opt/homebrew/opt/glew/lib -L/opt/homebrew/opt/glfw/lib
CPPFLAGS=-I/opt/homebrew/opt/glew/include -I/opt/homebrew/opt/glfw/include

BIN1=triandrect
BIN2=rectangle
SRCS1=triandrect.cpp gl_framework.cpp shader_util.cpp
SRCS2=rectangle.cpp gl_framework.cpp shader_util.cpp
INCLUDES=gl_framework.hpp shader_util.hpp

all: $(BIN1) $(BIN2)

$(BIN1): $(SRCS1) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS1) -o $(BIN1) $(LDFLAGS) $(LIBS)


$(BIN2): $(SRCS2) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS2) -o $(BIN2) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN1) $(BIN2)
