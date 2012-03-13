SRC := main.cpp \
	maths.cpp \
	planet.cpp \
	system.cpp \
	link.cpp \
	data.cpp \
	errorcheck.cpp \
	graphics.cpp \

FLAGS := -DGL_GLEXT_PROTOTYPES \

LIBS := -lGL \
	-lSDL \

starTrader: $(SRC)
	g++ -o starPirate -g  $(FLAGS) $(SRC) $(LIBS)

