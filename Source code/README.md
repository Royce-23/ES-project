# ES Project – ATmega128

## 1. Giới thiệu

Project gồm các bài thực hành P1.2 đến P1.8 trên vi điều khiển **ATmega128**, biên dịch bằng **Microchip XC8 AVR** và mô phỏng bằng **Proteus**.

Trong đó, P1.8 main.c là file build hoàn thiện sau cùng, tích hợp toàn bộ các khối chức năng (LCD, LED 7 đoạn, Nút nhấn, Relay, Motor DC và cảm biến DS18B20) để điều khiển và mô phỏng toàn bộ sơ đồ mạch Proteus. Các bài từ P1.2 đến P1.7 đóng vai trò là các bài thực hành và kiểm tra từng phần cứng riêng lẻ.

Mỗi lần chỉ biên dịch **một file `.c`**. Các file dùng chung như `userdef.h` không được biên dịch riêng.

## 2. Cấu trúc file

| File | Chức năng chính |
|---|---|
| `P1. 2 main.c` | Điều khiển 32 LED qua bộ nhớ ngoài; có ví dụ xuất trực tiếp và xuất bằng hàm. |
| `P1.3 main.c` | Hiển thị số trên LED 7 đoạn: xuất trực tiếp và quét nhiều LED. |
| `P1.4 main.c` | Kiểm tra điều khiển relay bằng union/bit-field. |
| `P1.5 main code simulate button and motor.c` | Đọc nút nhấn và điều khiển motor DC 1/2. |
| `P1.5 main code simulate lcd lm044l.c` | Khởi tạo LCD LM044L và hiển thị menu cơ bản. |
| `P1.6main.c` | Sử dụng danh sách liên kết tạo menu tương tác trên LCD. |
| `P1.7main.c` | Sử dụng con trỏ hàm (function pointer) tạo menu tương tác. |
| `P1.8 main.c` | File build hoàn thiện sau cùng: Tích hợp đầy đủ Menu LCD, LED 7 đoạn, nút nhấn, relay, motor và đọc nhiệt độ DS18B20 để chạy toàn bộ hệ thống mô phỏng. |
| `userdef.h` | Định nghĩa địa chỉ thiết bị, nút nhấn, relay, motor, LCD, LED 7 đoạn và giao thức DS18B20. |
| `build.ps1` | Script biên dịch `.c` thành `.hex`. |
| `.vscode/tasks.json` | Danh sách file để chọn khi nhấn `Ctrl + Shift + B`. |
| `build/` | Chứa file `.elf`, `.hex` và `.map` sau khi build. |

## 3. Phần mềm cần cài

### 3.1. Phần mềm bắt buộc để biên dịch

Trên máy Windows mới, cần cài đủ **cả hai** thành phần sau:

1. **Microchip MPLAB XC8 AVR Compiler**: trình biên dịch mã C cho ATmega128.
2. **Microchip MPLAB X IDE và ATmega Device Family Pack**:
   - Mở `Tools ? Packs ? Pack Manager` trong MPLAB X IDE.
   - Tìm `ATmega_DFP` và cài phiên bản mới nhất.
   - Gói này cung cấp thông tin thanh ghi, thư viện và linker script cho ATmega128.

Đường dẫn cần có dạng:

```text
C:\\Program Files\\Microchip\\xc8\\<version>
C:\\Program Files\\Microchip\\MPLABX\\<version>\\packs\\Microchip\\ATmega_DFP\\<version>
```

### 3.2. Phần mềm tùy chọn

- **Visual Studio Code** và extension **C/C++** của Microsoft: dùng để mở, sửa và build.
- **Proteus**: chỉ cần khi muốn mô phỏng mạch; không cần để biên dịch.

Không cần cài Python, CMake, Make hoặc npm.

### 3.3. Kiểm tra và chạy build

Mở PowerShell tại thư mục `Source code` (thư mục chứa `build.ps1`) và chạy:

```powershell
.\setup.ps1
```

Nếu hiện `Moi truong da san sang`, build mặc định P1.8:

```powershell
.\build.ps1
```

Build bài khác bằng tên file C:

```powershell
.\build.ps1 -Source 'P1.3 main.c'
```

File HEX sau khi build nằm tại `Source code\build\ES-project.hex`. File HEX mẫu nằm tại `release\ES-project-P1.8.hex`.

Nếu gặp lỗi `Khong tim thay thu muc ATmega_DFP trong XC8`, hãy cài ATmega Device Family Pack rồi chạy lại hai lệnh trên.

## 4. Mở project

Mở đúng thư mục chứa `build.ps1` bằng VS Code:

```text
Source code 
```

Không nên mở riêng từng file `.c`, vì VS Code có thể không nhận được task build.

## 5. Build bằng VS Code

1. Nhấn `Ctrl + Shift + B`.
2. Chọn task `Build HEX for Proteus`.
3. Chọn file bài cần chạy.
4. Chờ thông báo `BUILD THANH CONG`.

Kết quả được tạo tại:

```text
build/ES-project.hex
build/ES-project.elf
build/ES-project.map
```

File cần nạp vào Proteus là `build/ES-project.hex`.

## 6. Build bằng PowerShell

Mở PowerShell tại thư mục project rồi chạy lệnh tương ứng:

```powershell
.\build.ps1 -Source 'P1. 2 main.c'
.\build.ps1 -Source 'P1.3 main.c'
.\build.ps1 -Source 'P1.4 main.c'
.\build.ps1 -Source 'P1.5 main code simulate button and motor.c'
.\build.ps1 -Source 'P1.5 main code simulate lcd lm044l.c'
.\build.ps1 -Source 'P1.6main.c'
.\build.ps1 -Source 'P1.7main.c'
.\build.ps1 -Source 'P1.8 main.c'
```

Xóa kết quả build cũ:

```powershell
.\build.ps1 -Clean
```

Không dùng `.\build.ps1` không có `-Source` nếu project không có file `main.c` mặc định.

## 7. Chạy file HEX trong Proteus

1. Build đúng file `.c` cần kiểm tra.
2. Mở sơ đồ Proteus.
3. Nhấp đúp vào IC `ATMEGA128`.
4. Trong `Program File`, chọn `build/ES-project.hex`.
5. Đặt `Clock Frequency` là `1 MHz`.
6. Chạy mô phỏng.

Sau khi đổi bài, phải build lại và nạp lại file HEX vào ATmega128.

## 8. Mô tả từng bài

### P1.2 – 32 LED

Ghi dữ liệu 32 bit ra bốn thanh ghi:

```text
LED0, LED1, LED2, LED3 → CS0, CS1, CS2, CS3
```

Bài dùng để kiểm tra ghi dữ liệu qua bộ nhớ ngoài. Có thể thay giá trị LED trong hàm `led_display()` để tạo mẫu sáng khác nhau.

### P1.3 – LED 7 đoạn

Có hai cách hiển thị:

- Xuất trực tiếp từng LED qua `LED7D1` đến `LED7D4`.
- Quét LED bằng `LED7IDDATE` và `LED7IDCTR`.

LED 7 đoạn đang dùng loại **common-anode**, vì vậy mã hiển thị là mã mức thấp. Ví dụ số `3214` được hiển thị bằng:

```c
SevenSegDisplay(3214);
```

### P1.4 – Relay

Dùng `Relays.relayall` hoặc từng bit `RL0` đến `RL7` để bật/tắt relay. Relay được ghi qua `RELAYMAP` tại `CS10`.

### P1.5 – Nút nhấn và motor

`P1.5 main code simulate button and motor.c` đọc các nút `LEFT`, `RIGHT`, `UP`, `DOWN`, `GO` và điều khiển motor qua `MOTORDC` tại `CS13`.

Các trạng thái motor:

```text
STOP = 0
FORWARD = 1
REWARD = 2
```

`P1.5 main code simulate lcd lm044l.c` tập trung kiểm tra LCD LM044L và hiển thị chữ trên bốn dòng.

### P1.6 và P1.7 – Tạo danh sách Menu LCD và sử dụng con trỏ hàm(function pointer) để tạo mennu tương tác

Hai file dùng LCD LM044L và các nút nhấn để:

- `UP/DOWN`: di chuyển lựa chọn.
- `RIGHT`: đi vào menu con.
- `LEFT`: quay lại menu trước.
- `GO`: thực hiện chức năng được chọn.

Menu có các nhóm Sensors, Actuator và Settings. Phần Actuator dùng chung các cấu trúc relay/motor trong `userdef.h`.

### P1.8 – DS18B20 (FILE BUILD HOÀN THIỆN TOÀN BỘ MẠCH)

P1.8 main.c là phiên bản tổng hợp hoàn chỉnh nhất, dùng để biên dịch nạp vào ATmega128 điều khiển toàn bộ mô phỏng Proteus:

`P1.8 main.c` bổ sung:

- Đọc nhiệt độ DS18B20 bằng giao thức 1-Wire.
- Hiển thị nhiệt độ trên LCD.
- Hiển thị phần nguyên trên bốn LED 7 đoạn.

Trình tự sử dụng:

```text
Main Menu → Sensors → Temperature → GO
```

Nhấn `LEFT` để quay lại menu Sensors.

## 9. Địa chỉ bộ nhớ ngoài

Địa chỉ cơ sở được định nghĩa trong `userdef.h` là `0x1100`.

| Thiết bị | Địa chỉ chọn |
|---|---|
| `LED0`–`LED3` | `CS0`–`CS3` |
| `LED7D1`–`LED7D4` | `CS4`–`CS7` |
| Điều khiển LED quét | `CS8`–`CS9` |
| Relay | `CS10` |
| LCD | `CS11` |
| Motor DC | `CS13` |
| Nút nhấn | `CS14` |

ATmega128 phải bật giao tiếp bộ nhớ ngoài trước khi dùng các thiết bị:

```c
MCUCR |= 0x80;
XMCRA = 0;
```

## 10. Lưu ý khi chỉnh sửa

- P1.8 main.c là chương trình nguồn hoàn chỉnh duy nhất dùng để đánh giá toàn bộ mạch mô phỏng Proteus.
- Chỉ sửa file `userdef.h` khi cần thay đổi địa chỉ hoặc chân phần cứng.
- Không đổi tên các file `.c` nếu chưa cập nhật `.vscode/tasks.json`.
- Sau mỗi thay đổi code, phải build lại file HEX trước khi chạy Proteus.
