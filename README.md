# ES LCD Menu — Arduino Uno R3

Chương trình menu điều khiển LCD 16x2 trên Arduino Uno R3. Dự án được xây dựng bằng PlatformIO và có thể mô phỏng trên Proteus.

## 1. Chức năng chính

- Hiển thị menu trên LCD 16x2.
- Điều hướng bằng bốn nút:
  - UP: tăng hoặc di chuyển lên.
  - DOWN: giảm hoặc di chuyển xuống.
  - OK: chọn hoặc xác nhận.
  - BACK: quay lại.
- Hiển thị nhiệt độ từ cảm biến LM35.
- Hiển thị độ ẩm từ biến trở mô phỏng cảm biến độ ẩm.
- Hiển thị thời gian chạy của Arduino.
- Cài đặt giới hạn nhiệt độ, độ ẩm và thời gian.
- Điều khiển bốn relay.
- Điều khiển hai motor.
- Bật hoặc tắt LED trên chân D13.
- Tắt toàn bộ relay, motor và LED bằng chức năng `All outputs OFF`.

## 2. Cấu trúc menu

```text
Main
├── Sensors
│   ├── Temperature
│   │   ├── Display
│   │   └── Setup
│   ├── Humidity
│   │   ├── Display
│   │   └── Setup
│   └── Time
│       ├── Display
│       └── Setup
├── Actuators
│   ├── Relays
│   │   ├── Relay 1
│   │   │   ├── ON
│   │   │   └── OFF
│   │   ├── Relay 2
│   │   ├── Relay 3
│   │   └── Relay 4
│   ├── Motors
│   │   ├── Motor 1
│   │   └── Motor 2
│   └── LEDs
│       └── LED
│           ├── ON
│           └── OFF
└── Setup
    ├── All outputs OFF
    └── About
```

## 3. Kết nối phần cứng

| Thiết bị | Chân Arduino |
|---|---:|
| LCD RS | D6 |
| LCD E | D7 |
| LCD DB4 | D8 |
| LCD DB5 | D9 |
| LCD DB6 | D10 |
| LCD DB7 | D11 |
| Nút DOWN | D2 |
| Nút UP | D3 |
| Nút OK | D5 |
| Nút BACK | D4 |
| LED tích hợp | D13 |
| LM35 | A0 |
| Biến trở độ ẩm | A1 |

Các nút nhấn được nối giữa chân tín hiệu và GND. Chương trình sử dụng chế độ `INPUT_PULLUP`, vì vậy nút được xem là nhấn khi chân đọc mức `LOW`.

## 4. Cách điều khiển

### Trong menu

- Nhấn UP: di chuyển lên.
- Nhấn DOWN: di chuyển xuống.
- Nhấn OK:
  - Mở menu con.
  - Thực hiện chức năng của mục hiện tại.
- Nhấn BACK: quay về menu cha.

### Trong màn hình cài đặt

- UP: tăng giá trị.
- DOWN: giảm giá trị.
- OK: lưu giá trị.
- BACK: hủy thay đổi.

### Trong màn hình hiển thị

Nhấn OK hoặc BACK để quay lại menu.

## 5. Cấu trúc chương trình

### `MenuItem`

Mỗi mục menu được biểu diễn bằng cấu trúc `MenuItem`, gồm:

- Tên hiển thị.
- Con trỏ đến hàm xử lý.
- Mục trước và sau.
- Mục cha và mục con.

Các mục cùng cấp được tổ chức thành danh sách liên kết đôi vòng. Cấu trúc này cho phép di chuyển liên tục từ mục cuối về mục đầu.

### `Action`

```cpp
using Action = void (*)();
```

`Action` là con trỏ đến hàm không có tham số và không trả về giá trị. Nhờ đó, mỗi mục menu có thể gọi một chức năng riêng, chẳng hạn bật relay hoặc hiển thị cảm biến.

### Chống dội phím

Cấu trúc `DebouncedButton` lọc hiện tượng dội phím trong khoảng 30 ms, giúp mỗi lần nhấn chỉ tạo ra một sự kiện.

### Các loại màn hình

Chương trình sử dụng bốn trạng thái:

- `View::Menu`: hiển thị menu.
- `View::Message`: hiển thị thông báo.
- `View::Live`: hiển thị dữ liệu cảm biến hoặc thời gian.
- `View::Edit`: chỉnh sửa thông số.

## 6. Đọc cảm biến

### Nhiệt độ

Giá trị analog từ LM35 được quy đổi gần đúng sang độ C:

```cpp
temperature = analogRead(A0) * 500 / 1023;
```

### Độ ẩm

Biến trở tại A1 được quy đổi thành phần trăm:

```cpp
humidity = analogRead(A1) * 100 / 1023;
```

### Thời gian

Thời gian được tính dựa trên `millis()`, không sử dụng module RTC. Vì vậy, thời gian sẽ mất khi Arduino khởi động lại.

## 7. Cài đặt và biên dịch

1. Cài đặt Visual Studio Code.
2. Cài đặt extension PlatformIO.
3. Mở thư mục dự án có file `platformio.ini`.
4. Kết nối Arduino Uno R3 hoặc chọn môi trường mô phỏng.
5. Nhấn **PlatformIO: Build** để biên dịch.

File firmware sau khi build:

```text
.pio/build/uno/firmware.hex
```

File `.hex` này có thể được nạp vào Arduino hoặc sử dụng trong Proteus.

## 8. Mô phỏng trên Proteus

Trong Proteus, kết nối các linh kiện theo bảng chân ở trên:

- Arduino Uno R3.
- LCD 16x2.
- Bốn nút nhấn.
- LED tại D13.
- Các ngõ vào analog tại A0 và A1.
- Có thể dùng LED hoặc relay giả lập cho các ngõ ra điều khiển.

Sau khi biên dịch, nạp file sau vào Arduino trong Proteus:

```text
.pio/build/uno/firmware.hex
```

## 9. Giới hạn của chương trình

- Relay và motor hiện chỉ được lưu dưới dạng trạng thái phần mềm.
- Chương trình chưa điều khiển chân GPIO riêng cho từng relay hoặc motor.
- Thời gian sử dụng `millis()` nên không phải thời gian thực.
- Giá trị nhiệt độ LM35 được tính theo công thức đơn giản, chưa có hiệu chỉnh sai số.
- LCD sử dụng giao tiếp 4-bit.

## 10. Thư viện sử dụng

- `Arduino.h`
- `LiquidCrystal.h`
- `stdint.h`
- `stdio.h`

## 11. Tệp quan trọng

```text
src/main.cpp                         Mã nguồn chính
platformio.ini                       Cấu hình PlatformIO
docs/HUONG_DAN_UNO_R3_PROTEUS.md     Hướng dẫn chi tiết
.pio/build/uno/firmware.hex          Firmware sau khi biên dịch
```
