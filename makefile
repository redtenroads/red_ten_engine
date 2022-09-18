CC = clang++
LD = clang++
CFLAGS = -I../JoltPhysics -I../openal/include \
	     -I../SDL2-2.0.14/include -I../SDL2_ttf-2.0.15/include \
		 -Wall -c  -std=c++17 -mfpmath=sse -mavx -o2

LFLAGS = -shared -Wall -g 
LIBRARIES = -L../SDL2-2.0.14/lib/x64/ -L../openal/libs/Win64 -L../JoltPhysics/Build/VS2019_CL/Release \
			-L../SDL2_ttf-2.0.15/lib/x64 \
			-lSDL2 -lSDL2main -lkernel32 -luser32 -lgdi32 -lwinspool -lSDL2_ttf.lib \
			-lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 -lopengl32 -lJolt.lib -lOpenAL32.lib

# The build target 
TARGET = rtengine
SRCDIR = src
EXMDIR = examples
OBJDIR = objects
BINDIR = bin
 
OBJ_FILES = ${OBJDIR}/rtengine.o ${OBJDIR}/view.o ${OBJDIR}/viewManager.o ${OBJDIR}/layer.o ${OBJDIR}/glew.o \
			${OBJDIR}/layerActors.o ${OBJDIR}/layerEffects.o \
			${OBJDIR}/stage.o ${OBJDIR}/stageManager.o ${OBJDIR}/stb_image.o ${OBJDIR}/effect.o  \
			${OBJDIR}/resourceManager.o ${OBJDIR}/camera.o ${OBJDIR}/cameraOrto.o \
			${OBJDIR}/shadersManager.o  ${OBJDIR}/physicsManager.o ${OBJDIR}/soundManager.o \
			${OBJDIR}/transformation.o ${OBJDIR}/inputManager.o \
			${OBJDIR}/physicsEntity.o ${OBJDIR}/physicsEntityBox.o ${OBJDIR}/physicsEntitySphere.o \
			${OBJDIR}/actor.o  ${OBJDIR}/actorPawn.o ${OBJDIR}/actorGUIElement.o \
			${OBJDIR}/vector2.o ${OBJDIR}/vector3.o ${OBJDIR}/vector4.o ${OBJDIR}/matrix4.o \
			${OBJDIR}/sound.o ${OBJDIR}/texture.o ${OBJDIR}/font.o \
			${OBJDIR}/component.o ${OBJDIR}/componentSprite.o ${OBJDIR}/componentSoundPlayer.o ${OBJDIR}/componentText.o \
			${OBJDIR}/stb_vorbis.o

EXAMPLES = 	${BINDIR}/1-helloWorld.exe ${BINDIR}/2-helloActors.exe ${BINDIR}/3-helloPhysics.exe ${BINDIR}/4-helloSorting.exe \
			${BINDIR}/5-helloInput.exe ${BINDIR}/6-helloBytemap.exe ${BINDIR}/7-helloSound.exe ${BINDIR}/8-helloGUI.exe \
			${BINDIR}/9-helloEffects.exe ${BINDIR}/10-helloAnimation.exe ${BINDIR}/11-helloMusic.exe

all: engine examples

engine: $(BINDIR)/$(TARGET).dll
${OBJDIR}/rtengine.o: ${SRCDIR}/rtengine.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/rtengine.o ${SRCDIR}/rtengine.cpp

${OBJDIR}/view.o: ${SRCDIR}/view.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/view.o ${SRCDIR}/view.cpp

${OBJDIR}/viewManager.o: ${SRCDIR}/viewManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/viewManager.o ${SRCDIR}/viewManager.cpp

${OBJDIR}/layerActors.o: ${SRCDIR}/layerActors.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layerActors.o ${SRCDIR}/layerActors.cpp

${OBJDIR}/layerEffects.o: ${SRCDIR}/layerEffects.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layerEffects.o ${SRCDIR}/layerEffects.cpp
	
${OBJDIR}/layer.o: ${SRCDIR}/layer.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layer.o ${SRCDIR}/layer.cpp

${OBJDIR}/stage.o: ${SRCDIR}/stage.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stage.o ${SRCDIR}/stage.cpp

${OBJDIR}/stageManager.o: ${SRCDIR}/stageManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stageManager.o ${SRCDIR}/stageManager.cpp

${OBJDIR}/texture.o: ${SRCDIR}/texture.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/texture.o ${SRCDIR}/texture.cpp

${OBJDIR}/sound.o: ${SRCDIR}/sound.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/sound.o ${SRCDIR}/sound.cpp

${OBJDIR}/font.o: ${SRCDIR}/font.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/font.o ${SRCDIR}/font.cpp

${OBJDIR}/vector3.o: ${SRCDIR}/vector3.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector3.o ${SRCDIR}/vector3.cpp

${OBJDIR}/stb_image.o: ${SRCDIR}/stb_image.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stb_image.o ${SRCDIR}/stb_image.cpp

${OBJDIR}/effect.o: ${SRCDIR}/effect.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/effect.o ${SRCDIR}/effect.cpp

${OBJDIR}/resourceManager.o: ${SRCDIR}/resourceManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/resourceManager.o ${SRCDIR}/resourceManager.cpp

${OBJDIR}/camera.o: ${SRCDIR}/camera.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/camera.o ${SRCDIR}/camera.cpp

${OBJDIR}/cameraOrto.o: ${SRCDIR}/cameraOrto.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/cameraOrto.o ${SRCDIR}/cameraOrto.cpp

${OBJDIR}/actor.o: ${SRCDIR}/actor.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actor.o ${SRCDIR}/actor.cpp

${OBJDIR}/actorPawn.o: ${SRCDIR}/actorPawn.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actorPawn.o ${SRCDIR}/actorPawn.cpp

${OBJDIR}/actorGUIElement.o: ${SRCDIR}/actorGUIElement.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actorGUIElement.o ${SRCDIR}/actorGUIElement.cpp

${OBJDIR}/component.o: ${SRCDIR}/component.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/component.o ${SRCDIR}/component.cpp

${OBJDIR}/matrix4.o: ${SRCDIR}/matrix4.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/matrix4.o ${SRCDIR}/matrix4.cpp

${OBJDIR}/shadersManager.o: ${SRCDIR}/shadersManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/shadersManager.o ${SRCDIR}/shadersManager.cpp

${OBJDIR}/vector2.o: ${SRCDIR}/vector2.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector2.o ${SRCDIR}/vector2.cpp
	
${OBJDIR}/vector4.o: ${SRCDIR}/vector4.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector4.o ${SRCDIR}/vector4.cpp

${OBJDIR}/glew.o: ${SRCDIR}/glew.c
	$(CC) $(CFLAGS) -o ${OBJDIR}/glew.o ${SRCDIR}/glew.c

${OBJDIR}/physicsManager.o: ${SRCDIR}/physicsManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsManager.o ${SRCDIR}/physicsManager.cpp
	
${OBJDIR}/soundManager.o: ${SRCDIR}/soundManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/soundManager.o ${SRCDIR}/soundManager.cpp

${OBJDIR}/transformation.o: ${SRCDIR}/transformation.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/transformation.o ${SRCDIR}/transformation.cpp

${OBJDIR}/componentSprite.o: ${SRCDIR}/componentSprite.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentSprite.o ${SRCDIR}/componentSprite.cpp

${OBJDIR}/componentSoundPlayer.o: ${SRCDIR}/componentSoundPlayer.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentSoundPlayer.o ${SRCDIR}/componentSoundPlayer.cpp

${OBJDIR}/componentText.o: ${SRCDIR}/componentText.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentText.o ${SRCDIR}/componentText.cpp

${OBJDIR}/physicsEntitySphere.o: ${SRCDIR}/physicsEntitySphere.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntitySphere.o ${SRCDIR}/physicsEntitySphere.cpp

${OBJDIR}/physicsEntityBox.o: ${SRCDIR}/physicsEntityBox.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntityBox.o ${SRCDIR}/physicsEntityBox.cpp

${OBJDIR}/physicsEntity.o: ${SRCDIR}/physicsEntity.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntity.o ${SRCDIR}/physicsEntity.cpp

${OBJDIR}/inputManager.o: ${SRCDIR}/inputManager.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/inputManager.o ${SRCDIR}/inputManager.cpp

${OBJDIR}/stb_vorbis.o: ${SRCDIR}/stb_vorbis.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stb_vorbis.o ${SRCDIR}/stb_vorbis.cpp

$(BINDIR)/$(TARGET).dll: ${OBJ_FILES}
	$(LD) ${LFLAGS} ${LIBRARIES} ${OBJ_FILES} -o $(BINDIR)/$(TARGET).dll

examples: ${EXAMPLES} engine
${OBJDIR}/1-helloWorld.o: ${EXMDIR}/1-helloWorld.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/1-helloWorld.o ${EXMDIR}/1-helloWorld.cpp

${BINDIR}/1-helloWorld.exe: ${OBJDIR}/1-helloWorld.o
	$(LD) -Wl ${OBJDIR}/1-helloWorld.o -o ${BINDIR}/1-helloWorld.exe

${OBJDIR}/2-helloActors.o: ${EXMDIR}/2-helloActors.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/2-helloActors.o ${EXMDIR}/2-helloActors.cpp

${BINDIR}/2-helloActors.exe: ${OBJDIR}/2-helloActors.o
	$(LD) -Wl ${OBJDIR}/2-helloActors.o -o ${BINDIR}/2-helloActors.exe
	
${OBJDIR}/3-helloPhysics.o: ${EXMDIR}/3-helloPhysics.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/3-helloPhysics.o ${EXMDIR}/3-helloPhysics.cpp

${BINDIR}/3-helloPhysics.exe: ${OBJDIR}/3-helloPhysics.o
	$(LD) -Wl ${OBJDIR}/3-helloPhysics.o -o ${BINDIR}/3-helloPhysics.exe

${OBJDIR}/4-helloSorting.o: ${EXMDIR}/4-helloSorting.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/4-helloSorting.o ${EXMDIR}/4-helloSorting.cpp

${BINDIR}/4-helloSorting.exe: ${OBJDIR}/4-helloSorting.o
	$(LD) -Wl ${OBJDIR}/4-helloSorting.o -o ${BINDIR}/4-helloSorting.exe

${OBJDIR}/5-helloInput.o: ${EXMDIR}/5-helloInput.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/5-helloInput.o ${EXMDIR}/5-helloInput.cpp

${BINDIR}/5-helloInput.exe: ${OBJDIR}/5-helloInput.o
	$(LD) -Wl ${OBJDIR}/5-helloInput.o -o ${BINDIR}/5-helloInput.exe

${OBJDIR}/6-helloBytemap.o: ${EXMDIR}/6-helloBytemap.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/6-helloBytemap.o ${EXMDIR}/6-helloBytemap.cpp

${BINDIR}/6-helloBytemap.exe: ${OBJDIR}/6-helloBytemap.o
	$(LD) -Wl ${OBJDIR}/6-helloBytemap.o -o ${BINDIR}/6-helloBytemap.exe

${OBJDIR}/7-helloSound.o: ${EXMDIR}/7-helloSound.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/7-helloSound.o ${EXMDIR}/7-helloSound.cpp

${BINDIR}/7-helloSound.exe: ${OBJDIR}/7-helloSound.o
	$(LD) -Wl ${OBJDIR}/7-helloSound.o -o ${BINDIR}/7-helloSound.exe

${OBJDIR}/8-helloGUI.o: ${EXMDIR}/8-helloGUI.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/8-helloGUI.o ${EXMDIR}/8-helloGUI.cpp

${BINDIR}/8-helloGUI.exe: ${OBJDIR}/8-helloGUI.o
	$(LD) -Wl ${OBJDIR}/8-helloGUI.o -o ${BINDIR}/8-helloGUI.exe

${OBJDIR}/9-helloEffects.o: ${EXMDIR}/9-helloEffects.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/9-helloEffects.o ${EXMDIR}/9-helloEffects.cpp

${BINDIR}/9-helloEffects.exe: ${OBJDIR}/9-helloEffects.o
	$(LD) -Wl ${OBJDIR}/9-helloEffects.o -o ${BINDIR}/9-helloEffects.exe

${OBJDIR}/10-helloAnimation.o: ${EXMDIR}/10-helloAnimation.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/10-helloAnimation.o ${EXMDIR}/10-helloAnimation.cpp

${BINDIR}/10-helloAnimation.exe: ${OBJDIR}/10-helloAnimation.o
	$(LD) -Wl ${OBJDIR}/10-helloAnimation.o -o ${BINDIR}/10-helloAnimation.exe

${OBJDIR}/11-helloMusic.o: ${EXMDIR}/11-helloMusic.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/11-helloMusic.o ${EXMDIR}/11-helloMusic.cpp

${BINDIR}/11-helloMusic.exe: ${OBJDIR}/11-helloMusic.o
	$(LD) -Wl ${OBJDIR}/11-helloMusic.o -o ${BINDIR}/11-helloMusic.exe



# llvm-objcopy
clean:
	$(RM) $(TARGET)