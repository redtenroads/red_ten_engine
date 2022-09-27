CC = clang++
LD = clang++
CFLAGS = -I../JoltPhysics -I../openal/include -Isrc \
	     -I../SDL2-2.0.14/include -I../SDL2_ttf-2.0.15/include \
		 -Wall -c  -std=c++17 -mfpmath=sse -mavx -g

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
 
OBJ_FILES = ${OBJDIR}/rtengine.o ${OBJDIR}/view.o ${OBJDIR}/viewController.o ${OBJDIR}/layer.o ${OBJDIR}/glew.o \
			${OBJDIR}/layerActors.o ${OBJDIR}/layerEffects.o  \
			${OBJDIR}/stage.o ${OBJDIR}/stageController.o ${OBJDIR}/effect.o  \
			${OBJDIR}/camera.o ${OBJDIR}/cameraOrto.o ${OBJDIR}/cameraPerspective.o  \
			${OBJDIR}/physicsController.o ${OBJDIR}/soundController.o ${OBJDIR}/resourceController.o \
			${OBJDIR}/transformation.o ${OBJDIR}/inputController.o \
			${OBJDIR}/physicsEntity.o ${OBJDIR}/physicsEntityBox.o ${OBJDIR}/physicsEntitySphere.o \
			${OBJDIR}/actor.o  ${OBJDIR}/actorPawn.o ${OBJDIR}/actorGUIElement.o \
			${OBJDIR}/sound.o ${OBJDIR}/texture.o ${OBJDIR}/font.o ${OBJDIR}/mesh.o \
			${OBJDIR}/component.o ${OBJDIR}/componentSprite.o ${OBJDIR}/componentSoundPlayer.o \
			${OBJDIR}/componentText.o ${OBJDIR}/componentLight.o \
			${OBJDIR}/componentMesh.o ${OBJDIR}/meshDescriptor.o ${OBJDIR}/renderer.o \
			${OBJDIR}/stb_image.o ${OBJDIR}/fbx_loader.o ${OBJDIR}/stb_vorbis.o \
			${OBJDIR}/destroyable.o ${OBJDIR}/commonShaders.o ${OBJDIR}/utils.o \
			${OBJDIR}/phongShader.o ${OBJDIR}/rawShader.o ${OBJDIR}/shader.o ${OBJDIR}/lightningShader.o

EXAMPLES = 	${BINDIR}/1-helloWorld.exe ${BINDIR}/2-helloActors.exe ${BINDIR}/3-helloPhysics.exe ${BINDIR}/4-helloSorting.exe \
			${BINDIR}/5-helloInput.exe ${BINDIR}/6-helloBytemap.exe ${BINDIR}/7-helloSound.exe ${BINDIR}/8-helloGUI.exe \
			${BINDIR}/9-helloEffects.exe ${BINDIR}/10-helloAnimation.exe ${BINDIR}/11-helloMusic.exe ${BINDIR}/12-hello3d.exe

all: engine examples

engine: $(BINDIR)/$(TARGET).dll
${OBJDIR}/rtengine.o: ${SRCDIR}/rtengine.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/rtengine.o ${SRCDIR}/rtengine.cpp

${OBJDIR}/viewController.o: ${SRCDIR}/controller/viewController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/viewController.o ${SRCDIR}/controller/viewController.cpp

${OBJDIR}/stageController.o: ${SRCDIR}/controller/stageController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stageController.o ${SRCDIR}/controller/stageController.cpp

${OBJDIR}/resourceController.o: ${SRCDIR}/controller/resourceController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/resourceController.o ${SRCDIR}/controller/resourceController.cpp

${OBJDIR}/physicsController.o: ${SRCDIR}/controller/physicsController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsController.o ${SRCDIR}/controller/physicsController.cpp
	
${OBJDIR}/soundController.o: ${SRCDIR}/controller/soundController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/soundController.o ${SRCDIR}/controller/soundController.cpp

${OBJDIR}/inputController.o: ${SRCDIR}/controller/inputController.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/inputController.o ${SRCDIR}/controller/inputController.cpp

${OBJDIR}/view.o: ${SRCDIR}/os/view.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/view.o ${SRCDIR}/os/view.cpp

${OBJDIR}/layerActors.o: ${SRCDIR}/stage/layerActors.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layerActors.o ${SRCDIR}/stage/layerActors.cpp

${OBJDIR}/layerEffects.o: ${SRCDIR}/stage/layerEffects.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layerEffects.o ${SRCDIR}/stage/layerEffects.cpp
	
${OBJDIR}/layer.o: ${SRCDIR}/stage/layer.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/layer.o ${SRCDIR}/stage/layer.cpp

${OBJDIR}/stage.o: ${SRCDIR}/stage/stage.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stage.o ${SRCDIR}/stage/stage.cpp

${OBJDIR}/texture.o: ${SRCDIR}/resource/texture.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/texture.o ${SRCDIR}/resource/texture.cpp

${OBJDIR}/sound.o: ${SRCDIR}/resource/sound.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/sound.o ${SRCDIR}/resource/sound.cpp

${OBJDIR}/font.o: ${SRCDIR}/resource/font.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/font.o ${SRCDIR}/resource/font.cpp

${OBJDIR}/mesh.o: ${SRCDIR}/resource/mesh.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/mesh.o ${SRCDIR}/resource/mesh.cpp

${OBJDIR}/vector3.o: ${SRCDIR}/math/vector3.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector3.o ${SRCDIR}/math/vector3.cpp

${OBJDIR}/matrix4.o: ${SRCDIR}/math/matrix4.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/matrix4.o ${SRCDIR}/math/matrix4.cpp

${OBJDIR}/vector2.o: ${SRCDIR}/math/vector2.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector2.o ${SRCDIR}/math/vector2.cpp
	
${OBJDIR}/vector4.o: ${SRCDIR}/math/vector4.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/vector4.o ${SRCDIR}/math/vector4.cpp
	
${OBJDIR}/transformation.o: ${SRCDIR}/math/transformation.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/transformation.o ${SRCDIR}/math/transformation.cpp

${OBJDIR}/stb_image.o: ${SRCDIR}/loaders/stb_image.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stb_image.o ${SRCDIR}/loaders/stb_image.cpp

${OBJDIR}/stb_vorbis.o: ${SRCDIR}/loaders/stb_vorbis.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/stb_vorbis.o ${SRCDIR}/loaders/stb_vorbis.cpp
	
${OBJDIR}/fbx_loader.o: ${SRCDIR}/loaders/fbx_loader.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/fbx_loader.o ${SRCDIR}/loaders/fbx_loader.cpp

${OBJDIR}/camera.o: ${SRCDIR}/camera/camera.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/camera.o ${SRCDIR}/camera/camera.cpp

${OBJDIR}/cameraOrto.o: ${SRCDIR}/camera/cameraOrto.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/cameraOrto.o ${SRCDIR}/camera/cameraOrto.cpp

${OBJDIR}/cameraPerspective.o: ${SRCDIR}/camera/cameraPerspective.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/cameraPerspective.o ${SRCDIR}/camera/cameraPerspective.cpp

${OBJDIR}/actor.o: ${SRCDIR}/actor/actor.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actor.o ${SRCDIR}/actor/actor.cpp

${OBJDIR}/actorPawn.o: ${SRCDIR}/actor/actorPawn.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actorPawn.o ${SRCDIR}/actor/actorPawn.cpp

${OBJDIR}/actorGUIElement.o: ${SRCDIR}/actor/actorGUIElement.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/actorGUIElement.o ${SRCDIR}/actor/actorGUIElement.cpp

${OBJDIR}/component.o: ${SRCDIR}/component/component.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/component.o ${SRCDIR}/component/component.cpp

${OBJDIR}/componentSprite.o: ${SRCDIR}/component/componentSprite.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentSprite.o ${SRCDIR}/component/componentSprite.cpp

${OBJDIR}/componentSoundPlayer.o: ${SRCDIR}/component/componentSoundPlayer.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentSoundPlayer.o ${SRCDIR}/component/componentSoundPlayer.cpp

${OBJDIR}/componentText.o: ${SRCDIR}/component/componentText.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentText.o ${SRCDIR}/component/componentText.cpp

${OBJDIR}/componentMesh.o: ${SRCDIR}/component/componentMesh.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentMesh.o ${SRCDIR}/component/componentMesh.cpp

${OBJDIR}/componentLight.o: ${SRCDIR}/component/componentLight.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/componentLight.o ${SRCDIR}/component/componentLight.cpp

${OBJDIR}/physicsEntitySphere.o: ${SRCDIR}/physics/physicsEntitySphere.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntitySphere.o ${SRCDIR}/physics/physicsEntitySphere.cpp

${OBJDIR}/physicsEntityBox.o: ${SRCDIR}/physics/physicsEntityBox.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntityBox.o ${SRCDIR}/physics/physicsEntityBox.cpp

${OBJDIR}/physicsEntity.o: ${SRCDIR}/physics/physicsEntity.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/physicsEntity.o ${SRCDIR}/physics/physicsEntity.cpp

${OBJDIR}/utils.o: ${SRCDIR}/common/utils.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/utils.o ${SRCDIR}/common/utils.cpp

${OBJDIR}/destroyable.o: ${SRCDIR}/common/destroyable.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/destroyable.o ${SRCDIR}/common/destroyable.cpp

${OBJDIR}/commonShaders.o: ${SRCDIR}/common/commonShaders.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/commonShaders.o ${SRCDIR}/common/commonShaders.cpp

${OBJDIR}/meshDescriptor.o: ${SRCDIR}/common/meshDescriptor.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/meshDescriptor.o ${SRCDIR}/common/meshDescriptor.cpp
	
${OBJDIR}/renderer.o: ${SRCDIR}/common/renderer.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/renderer.o ${SRCDIR}/common/renderer.cpp

${OBJDIR}/effect.o: ${SRCDIR}/shaders/effect.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/effect.o ${SRCDIR}/shaders/effect.cpp

${OBJDIR}/phongShader.o: ${SRCDIR}/shaders/phongShader.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/phongShader.o ${SRCDIR}/shaders/phongShader.cpp

${OBJDIR}/rawShader.o: ${SRCDIR}/shaders/rawShader.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/rawShader.o ${SRCDIR}/shaders/rawShader.cpp

${OBJDIR}/lightningShader.o: ${SRCDIR}/shaders/lightningShader.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/lightningShader.o ${SRCDIR}/shaders/lightningShader.cpp

${OBJDIR}/shader.o: ${SRCDIR}/shaders/shader.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/shader.o ${SRCDIR}/shaders/shader.cpp

${OBJDIR}/glew.o: ${SRCDIR}/opengl/glew.c
	$(CC) $(CFLAGS) -o ${OBJDIR}/glew.o ${SRCDIR}/opengl/glew.c

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

${OBJDIR}/12-hello3d.o: ${EXMDIR}/12-hello3d.cpp
	$(CC) $(CFLAGS) -o ${OBJDIR}/12-hello3d.o ${EXMDIR}/12-hello3d.cpp

${BINDIR}/12-hello3d.exe: ${OBJDIR}/12-hello3d.o
	$(LD) -Wl ${OBJDIR}/12-hello3d.o -o ${BINDIR}/12-hello3d.exe


# llvm-objcopy
clean:
	$(RM) $(TARGET)