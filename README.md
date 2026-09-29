# ES Project – ATmega128

Project dùng **ATmega128**, trình biên dịch **Microchip XC8 AVR** và mô phỏng trên **Proteus**.

## 1. Cấu trúc project

Các file bài thực hành hiện có:

- `P1. 2 main.c`
- `P1.3 main.c`
- `P1.4 main.c`
- `P1.5 main code simulate button and motor.c`
- `P1.5 main code simulate lcd lm044l.c`
- `P1.6main.c`
- `P1.7main.c`
- `P1.8 main.c`

Các file dùng chung:

- `userdef.h`: định nghĩa địa chỉ bộ nhớ ngoài, LED, LCD, nút nhấn, relay, motor, LED 7 đoạn và DS18B20.
- `build.ps1`: script biên dịch và tạo file HEX.
- `.vscode/tasks.json`: cấu hình chọn file cần build trong VS Code.
- `build/`: thư mục chứa kết quả build; thư mục này không đưa lên Git.

## 2. Phần mềm cần cài

Cần cài trên Windows:

1. Visual Studio Code.
2. Extension C/C++ của Microsoft.
3. MPLAB XC8 AVR.
4. MPLAB X Device Family Pack cho dòng ATmega.
5. Proteus nếu muốn mô phỏng mạch.

Script tự tìm XC8 và Device Family Pack trong:

```text
C:\Program Files\Microchip\xc8
C:\Program Files\Microchip\MPLABX
```

## 3. Mở project

Mở đúng thư mục chứa file `build.ps1` bằng VS Code:

```text
ES-project-VSCode
```

Không mở riêng một file `.c`, vì VS Code có thể không nhận task build của project.

## 4. Chọn file và build bằng VS Code

1. Mở project trong VS Code.
2. Nhấn `Ctrl + Shift + B`.
3. Chọn task `Build HEX for Proteus` nếu VS Code hỏi.
4. Chọn file `.c` cần biên dịch.
5. Chờ dòng thông báo `BUILD THANH CONG`.

Mỗi lần build, các file sau sẽ bị ghi đè:

```text
build/ES-project.elf
build/ES-project.hex
build/ES-project.map
```

File dùng trong Proteus là `build/ES-project.hex`.

## 5. Build bằng PowerShell

Build file mặc định `main.c`:

```powershell
.\build.ps1
```

Build một file cụ thể:

```powershell
.\build.ps1 -Source 'P1.8 main.c'
.\build.ps1 -Source 'P1.3 main.c'
.\build.ps1 -Source 'P1.4 main.c'
.\build.ps1 -Source 'P1.7main.c'
```

Xóa kết quả build:

```powershell
.\build.ps1 -Clean
```

## 6. Nạp chương trình vào Proteus

1. Build file cần chạy.
2. Mở mạch Proteus.
3. Nhấp đúp vào `ATmega128`.
4. Tại `Program File`, chọn `build/ES-project.hex`.
5. Đặt `Clock Frequency` là `1 MHz`.
6. Chạy mô phỏng.

Mỗi khi đổi bài, cần build lại và nạp lại file HEX vào ATmega128.

## 7. Địa chỉ thiết bị bộ nhớ ngoài

Địa chỉ cơ sở trong `userdef.h` là `0x1100`:

| Thiết bị | Địa chỉ |
|---|---:|
| `LED0` – `LED3` | `CS0` – `CS3` |
| `LED7D1` – `LED7D4` | `CS4` – `CS7` |
| Điều khiển LED quét | `CS8` – `CS9` |
| Relay | `CS10` |
| LCD | `CS11` |
| Motor DC | `CS13` |
| Nút nhấn | `CS14` |

ATmega128 phải bật bộ nhớ ngoài trước khi ghi thiết bị:

```c
MCUCR |= 0x80;
XMCRA = 0;
```

## 8. LED 7 đoạn

Mạch đang dùng LED 7 đoạn **common-anode**, nên bảng mã trong `userdef.h` là mức thấp:

```text
0 -> 0xC0, 1 -> 0xF9, 2 -> 0xA4, ..., 9 -> 0x90
```

Ví dụ:

```c
SegDisplayD(1234);
```

Nếu phần cứng nối đúng CS4–CS7, LED sẽ hiển thị `1234` từ trái sang phải.

Để kiểm tra riêng LED, tạm thay:

```c
Temp = ReadTemp();
SegDisplayD(Temp);
```

bằng:

```c
SegDisplayD(1234);
```

## 9. DS18B20 trong P1.8

DS18B20 được cấu hình ở chân `PB0`:

```c
#define DS18B20 0
```

Kết nối Proteus cần có:

- `DQ` nối vào `PB0`.
- `VCC` nối nguồn.
- `GND` nối mass.
- Điện trở kéo lên khoảng `4.7kΩ` từ `DQ` lên `VCC`.

P1.8 đọc nhiệt độ nguyên và hiển thị lên LED. Ví dụ `42°C` hiển thị thành `0042`.

Nếu hiển thị `0000`, kiểm tra DQ, điện trở kéo lên, VCC/GND và thử chương trình LED bằng `SegDisplayD(1234)` như phần trên.

## 10. Xử lý lỗi thường gặp

### Không tìm thấy XC8

Kiểm tra XC8 đã cài trong:

```text
C:\Program Files\Microchip\xc8
```

### Không thấy file mới trong danh sách build

Thêm tên file vào `options` của input `esSourceFile` trong `.vscode/tasks.json`.

### Proteus không chạy chương trình mới

Build lại, chọn lại `build/ES-project.hex` trong thuộc tính ATmega128 và kiểm tra clock là `1 MHz`.

### Xóa file build cũ

Chạy:

```powershell
.\build.ps1 -Clean
```

Sau đó build lại file cần chạy.
