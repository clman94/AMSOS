CPP_FILES      := $(wildcard src/*.cpp)
ASM_FILES      := $(wildcard asm/*.asm)
ASM_BOOT_FILES := $(wildcard boot/*.asm)
OBJ_FILES      := $(addprefix obj/,$(notdir $(ASM_BOOT_FILES:.asm=.o))) $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o))) $(addprefix obj/,$(notdir $(ASM_FILES:.asm=.o)))

CPP            := g++
CPP_INCLUDES   := -I./include
CPP_FLAGS      := -MMD -std=c++11 -m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

ASM            := nasm
ASM_FLAGS      := -felf32

OBJ_DIR        := obj

EXECUTABLE     := isodir/boot/myos.bin
ISO            := myos.iso
ISO_DIR        := isodir

all: $(ISO)

$(ISO): $(EXECUTABLE)
	grub-mkrescue -o $@ $(ISO_DIR)

$(EXECUTABLE): $(OBJ_FILES)
	gcc -m32 -T link.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc -Wl,--build-id=none

obj/%.o: src/%.cpp $(OBJ_DIR)
	$(CPP) $(CPP_FLAGS) $(CPP_INCLUDES) -c $< -o $@

obj/%.o: asm/%.asm $(OBJ_DIR)
	$(ASM) $(ASM_FLAGS) $< -o $@ -MD $(@:.o=.d)

obj/%.o: boot/%.asm $(OBJ_DIR)
	$(ASM) $(ASM_FLAGS) $< -o $@ -MD $(@:.o=.d)
	
$(OBJ_DIR):
	mkdir $(OBJ_DIR)
	
-include $(OBJ_FILES:.o=.d) obj/boot.d