# ES project - ATmega128

Project VS Code nay la ban rieng, duoc tao tu project MPLAB X cu. Folder cu van duoc giu nguyen.

## Tao file HEX

1. Mo folder nay bang VS Code.
2. Bam `Ctrl+Shift+B`.
3. Chon `Build HEX for Proteus` neu VS Code hoi.

Sau khi build thanh cong, file dung de nap vao Proteus nam tai:

`build/ES-project.hex`

## Nap vao Proteus

1. Bam dup vao ATmega128 trong mach Proteus.
2. Tai `Program File`, chon `build/ES-project.hex`.
3. Dat `Clock Frequency` thanh `1 MHz` de khop voi `F_CPU` trong code.

Project dung MPLAB XC8 va Microchip ATmega Device Family Pack da cai tren may.
