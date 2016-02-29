
CPPCOMPILE="g++ -m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -c"
NASMCOMPILE="nasm -felf32"

LINKORDER="boot.o gdtasm.o gdt.o term.o begin.o"

echo Compiling
echo --Kernel--
$CPPCOMPILE begin.cpp -o ./bin/begin.o

echo --Memory Manager--
$CPPCOMPILE ./memory/gdt.cpp -o ./bin/gdt.o
$NASMCOMPILE ./memory/gdt.asm -o ./bin/gdtasm.o

echo --System Terminal Display--
$CPPCOMPILE term.cpp -o ./bin/term.o

echo --Main Loader--
$NASMCOMPILE boot.asm -o ./bin/boot.o

echo
echo Linking
cd ./bin
gcc -m32 -T ../link.ld -o ../isodir/boot/myos.bin -ffreestanding -O2 -nostdlib $LINKORDER -lgcc
cd ..

echo #Packaging#
grub-mkrescue -o myos.iso isodir
