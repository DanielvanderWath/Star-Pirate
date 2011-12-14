SRC := main.cpp \
	planet.cpp \
	system.cpp \
	data.cpp \
	errorcheck.cpp \
	graphics.cpp \

FLAGS := -DGL_GLEXT_PROTOTYPES \

LIBS := -lGL \
	-lSDL \

starTrader: $(SRC)
	g++ -o starPirate -g  $(FLAGS) $(SRC) $(LIBS)

