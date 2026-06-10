PROJECT_NAME = revision-like
ITCHIO_USER = puroprimesouscaprisun

# CXX
CXX = g++
WIN_CXX = x86_64-w64-mingw32-g++

# Targets
TARGET = game
WIN_TARGET = publish/windows/game.exe

WINDOWS_DLLS = /usr/x86_64-w64-mingw32/bin/glfw3.dll \
							 /usr/x86_64-w64-mingw32/bin/libassimp.dll \
							 /usr/x86_64-w64-mingw32/bin/libgcc_s_seh-1.dll \
							 /usr/x86_64-w64-mingw32/bin/libssp-0.dll \
							 /usr/x86_64-w64-mingw32/bin/libstdc++-6.dll \
							 /usr/x86_64-w64-mingw32/bin/zlib1.dll \
							 /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll

# Flags
CXXFLAGS = -std=c++17 -Iinclude
WINDOWS_CXXFLAGS = -std=c++17 -Iinclude
TEST_CXXFLAGS = -Wall -Wextra

# Libraries
LIBS_LINUX = \
-lassimp \
-lglfw \
-lGL \
-ldl \
-lpthread \
-lX11 \
-lXrandr \
-lXi \
-lXxf86vm \
-lXcursor \
-lm

LIBS_WINDOWS = \
-L/usr/x86_64-w64-mingw32/lib \
-lassimp \
-lglfw3 \
-lopengl32 \
-lgdi32 \
-lwinmm

# Sources
COMMON_SRC = \
	src/stbImpl.cpp \
	src/main.cpp \
	src/Window.cpp \
	src/Object.cpp \
	src/FileLoader.cpp \
	src/Sound.cpp \
	src/UIElement.cpp \
	src/Font.cpp \
	src/TextElement.cpp \
	src/Container.cpp \
	src/Button.cpp \
	src/Particle.cpp \
	src/Gameloop.cpp \
	src/FightPhase.cpp \
	src/FrenchRandom.cpp \
	src/Textbox.cpp \
	src/ScrollingElement.cpp \
	src/ShopPhase.cpp \
	src/UIParticle.cpp \
	src/MainMenu.cpp \
	src/professors/Professor.cpp \
	src/professors/Chloe.cpp

SRC = $(COMMON_SRC) src/glad.c

PUBLISH_SRC = textures \
							fonts \
							shaders \
							infos

# Object files
OBJ_DIR = build

OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/src/%.o)
OBJ := $(OBJ:src/glad.c=$(OBJ_DIR)/src/glad.o)

# Build
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(LIBS_LINUX)

$(OBJ_DIR)/src/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/src/glad.o: src/glad.c
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

windows:
	mkdir -p build
	mkdir -p publish/windows

	$(WIN_CXX) \
	$(WINDOWS_CXXFLAGS) \
	$(SRC) \
	-o $(WIN_TARGET) \
	$(LIBS_WINDOWS) \
	-mwindows

test: CXXFLAGS += $(TEST_CXXFLAGS)
test: all
	./$(TARGET)

clean:
	rm -rf build
	rm -rf publish
	rm -f $(TARGET)

publish: clean
	mkdir -p publish/linux
	mkdir -p publish/windows

	# Linux
	cp -r $(PUBLISH_SRC) publish/linux
	$(MAKE)

	cp $(TARGET) publish/linux/$(TARGET)

	# Windows
	cp -r $(PUBLISH_SRC) publish/windows
	cp $(WINDOWS_DLLS) publish/windows

	$(MAKE) windows

	# Zip builds
	cd publish && \
	zip -r -9 linux.zip linux && \
	zip -r -9 windows.zip windows && \
	butler push linux.zip $(ITCHIO_USER)/$(PROJECT_NAME):linux && \
	butler push windows.zip $(ITCHIO_USER)/$(PROJECT_NAME):windows

.PHONY: all clean test windows publish

