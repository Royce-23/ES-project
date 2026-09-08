# Hướng dẫn toàn bộ project menu LCD trên Arduino Uno R3

## 1. Phạm vi thiết kế

Hình người dùng gửi là sơ đồ cấu trúc menu, không phải bố cục cần hiện đồng thời trên LCD.
Vì LCD chỉ có 16 cột và 2 dòng:

- Dòng 1: tên menu cha.
- Dòng 2: mục đang chọn.
- Ký tự `>` ở đầu dòng: con trỏ chọn.
- Ký tự `>` ở cuối dòng: mục này còn menu con.

Ví dụ lúc khởi động:

```text
Main
>Sensors       >
```

## 2. Cây menu đã triển khai

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
│   │   ├── Relay 1 ── ON / OFF
│   │   ├── Relay 2 ── ON / OFF
│   │   ├── Relay 3 ── ON / OFF
│   │   └── Relay 4 ── ON / OFF
│   ├── Motors
│   │   ├── Motor 1 ── ON / OFF
│   │   └── Motor 2 ── ON / OFF
│   └── LEDs
│       └── LED ── ON / OFF
└── Setup
    ├── All outputs OFF
    └── About
```

UP và DOWN tương ứng Pre và Next trong sơ đồ. OK đi vào nhánh hoặc chạy chức năng.
BACK trở về menu cha.

## 3. Linh kiện Proteus

| Số lượng | Tên tìm trong Proteus | Vai trò |
|---:|---|---|
| 1 | ARDUINO UNO R3 | Vi điều khiển |
| 1 | LM016L | LCD 16x2 |
| 4 | BUTTON | UP, DOWN, OK, BACK |
| 1 | POT-HG 10k | Chỉnh tương phản LCD |
| 1 | LED-RED | Kiểm tra lệnh LED ON/OFF |
| 1 | RES 330R | Hạn dòng LED |
| 1 tùy chọn | LM35 | Giả lập nhiệt độ |
| 1 tùy chọn | POT-HG 10k | Giả lập độ ẩm tại A1 |
| — | VCC, GND | Nguồn logic |

Relay và motor trong bản menu này được lưu trạng thái trong RAM và báo trạng thái trên LCD.
Không nối tải công suất trực tiếp vào chân Uno. Muốn điều khiển relay/motor thật phải thêm
driver transistor/module phù hợp; phần đó không cần để kiểm tra menu.

## 4. Nối LCD 16x2

Trong code:

```cpp
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
```

Thứ tự tham số là RS, E, DB4, DB5, DB6, DB7.

| Chân LM016L | Tên | Nối |
|---:|---|---|
| 1 | VSS | GND |
| 2 | VDD | +5 V |
| 3 | VEE/V0 | Chân giữa biến trở 10k |
| 4 | RS | Uno D6 |
| 5 | RW | GND |
| 6 | E | Uno D7 |
| 7–10 | DB0–DB3 | Để trống |
| 11 | DB4 | Uno D8 |
| 12 | DB5 | Uno D9 |
| 13 | DB6 | Uno D10 |
| 14 | DB7 | Uno D11 |

Hai chân ngoài biến trở tương phản nối +5 V và GND.

## 5. Nối bốn nút

| Nút | Một phía | Phía còn lại |
|---|---|---|
| UP | D2 | GND |
| DOWN | D3 | GND |
| OK | D4 | GND |
| BACK | D5 | GND |

Code dùng `INPUT_PULLUP`, nên:

- Không nhấn: chân đọc HIGH.
- Nhấn: nút nối chân xuống GND, chân đọc LOW.
- Không cần điện trở kéo ngoài.

Nếu dùng BUTTON bốn chân, hai chân cùng một phía thường đã nối nội bộ. Hãy nối Dn ở một
phía và GND ở phía đối diện; không lấy hai chân cùng phía.

## 6. Nối LED và cảm biến tùy chọn

LED: D13 → điện trở 330 ohm → anode LED; cathode LED → GND.

LM35:

| Chân LM35 | Nối |
|---|---|
| VS | +5 V |
| VOUT | A0 |
| GND | GND |

Biến trở giả lập độ ẩm:

- Hai chân ngoài: +5 V và GND.
- Chân giữa: A1.
- Xoay biến trở làm giá trị hiển thị thay đổi 0–100%.

Nếu chưa lắp LM35 hoặc biến trở A1, menu vẫn hoạt động; chỉ số Display có thể không có ý nghĩa.

## 7. Giải thích code từ thư viện đến main

### 7.1 Các thư viện

```cpp
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdint.h>
#include <stdio.h>
```

- `Arduino.h`: cung cấp `pinMode`, `digitalRead`, `digitalWrite`, `analogRead`,
  `millis`, HIGH/LOW và A0/A1.
- `LiquidCrystal.h`: điều khiển LCD HD44780 ở chế độ song song 4 bit.
- `stdint.h`: cung cấp kiểu có kích thước rõ ràng như `uint8_t`, `uint32_t`.
- `stdio.h`: cung cấp `snprintf` để ghép số và chữ vào chuỗi an toàn.

PlatformIO tự lấy LiquidCrystal theo `platformio.ini`:

```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
lib_deps = arduino-libraries/LiquidCrystal@1.0.7
```

### 7.2 Khai báo chân

`constexpr` tạo hằng số ngay lúc biên dịch. Viết tên chân một lần giúp tránh nhầm khi sửa.
`DEBOUNCE_MS=30` là thời gian chống dội; `LIVE_REFRESH_MS=400` là chu kỳ cập nhật màn hình
Temperature/Humidity/Time.

### 7.3 Các trạng thái

```cpp
enum class Key { None, Up, Down, Ok, Back };
enum class View { Menu, Message, Live, Edit };
```

- `Key`: sự kiện phím vừa xảy ra.
- `View::Menu`: đang điều hướng menu.
- `View::Message`: báo ON/OFF hoặc About.
- `View::Live`: đang xem nhiệt độ, độ ẩm, thời gian.
- `View::Edit`: đang sửa một giá trị Setup.

Đây là state machine nhỏ. Cùng một nút UP nhưng trong Menu nó đổi node; trong Edit nó tăng số.

### 7.4 Function pointer

```cpp
using Action = void (*)();
```

`Action` là kiểu địa chỉ hàm có dạng `void tenHam()`.

```cpp
MenuItem r1On("ON", relay1On);
```

Node `r1On` giữ địa chỉ hàm `relay1On`. Khi OK tại node lá:

```cpp
else if (selected->action) selected->action();
```

Lúc đó hàm được gọi qua con trỏ. Bộ điều hướng không cần viết một chuỗi
`if (selected == ...)` cho từng relay.

### 7.5 Node và linked list

```cpp
struct MenuItem {
    const char *label;
    Action action;
    MenuItem *prev, *next, *parent, *child;
};
```

| Trường | Ý nghĩa |
|---|---|
| label | Chữ hiện trên LCD |
| action | Function pointer chạy khi OK |
| prev | Mục trước cùng cấp |
| next | Mục sau cùng cấp |
| parent | Menu cha |
| child | Mục đầu tiên của menu con |

`appendChild(parent,node)` nối node mới vào cuối danh sách con. Danh sách là vòng:
mục cuối có `next` về mục đầu và mục đầu có `prev` về mục cuối. Vì vậy:

```cpp
selected = selected->next; // DOWN/Next
selected = selected->prev; // UP/Pre
```

Không dùng `new` hoặc `malloc`: các node được tạo tĩnh, phù hợp RAM nhỏ của ATmega328P.

### 7.6 buildMenu()

`buildMenu()` thể hiện trực tiếp sơ đồ. Ví dụ:

```cpp
appendChild(root, sensors);
appendChild(root, actuators);
appendChild(root, systemSetup);
appendChild(sensors, temperature);
appendChild(temperature, tempDisplay);
appendChild(temperature, tempSetup);
```

Nghĩa là Main có Sensors/Actuators/Setup; Sensors có Temperature; Temperature có Display/Setup.
Hàm chỉ được gọi một lần trong `setup()`; gọi lại sẽ nối trùng node.

### 7.7 Chống dội nút

Nút cơ có thể dao động HIGH/LOW nhiều lần trong vài mili giây. `DebouncedButton::pressed()`
chỉ phát sự kiện khi mức tín hiệu giữ ổn định ít nhất 30 ms. Sau đó phải nhả rồi nhấn lại,
nên giữ nút không làm menu chạy liên tục.

`readKey()` cập nhật cả bốn nút. Do thứ tự mảng, khi nhiều nút được xác nhận cùng vòng lặp,
BACK có ưu tiên cao nhất.

### 7.8 Callback Display và Setup

- `showTemperature()`, `showHumidity()`, `showTime()` chuyển sang `View::Live`.
- `setupTemperature()`, `setupHumidity()`, `setupTime()` gọi `beginEdit()`.
- OK trong Edit ghi `editDraft` vào biến thật qua con trỏ `editTarget`.
- BACK trong Edit hủy thay đổi.

Công thức LM35:

```cpp
temperature = analogRead(A0) * 500 / 1023;
```

ADC 10 bit cho 0–1023. Với Vref 5 V và LM35 10 mV/°C, toàn thang tương ứng 500°C.

### 7.9 Callback actuator

Mỗi mục ON/OFF có callback riêng vì kiểu function pointer không nhận tham số. Callback gọi
hàm dùng chung `setRelay(index,on)`, `setMotor(index,on)` hoặc `setLed(on)`.

Relay/motor chỉ thay đổi mảng trạng thái và báo trên LCD. LED còn gọi `digitalWrite(D13,...)`
nên thấy LED Proteus bật/tắt thật.

### 7.10 handleMenuKey() và handleKey()

`handleMenuKey()` xử lý bốn hướng:

- UP → `prev`.
- DOWN → `next`.
- OK → có child thì đi vào child; không có child thì gọi action.
- BACK → về parent, trừ khi đang ở cấp Main.

`handleKey()` chọn cách xử lý theo `View`. Đây là lý do BACK từ màn hình Display trả về
đúng mục Display, còn BACK trong cây menu trả về menu cha.

### 7.11 Hiển thị LCD

`printLine()` luôn ghi đủ 16 ký tự. Phần còn thiếu được chèn khoảng trắng để xóa chữ cũ,
không cần `lcd.clear()` liên tục nên LCD ít nhấp nháy.

`render()` chỉ chạy khi `dirty=true`. Màn hình Live tự đặt dirty mỗi 400 ms.

### 7.12 setup() và loop()

`setup()`:

1. Cấu hình D13 OUTPUT.
2. Cấu hình bốn nút INPUT_PULLUP.
3. Khởi tạo LCD 16x2.
4. Nối toàn bộ cây menu.
5. Vẽ màn hình đầu tiên.

`loop()`:

1. Đọc sự kiện nút.
2. Xử lý state/menu.
3. Cập nhật thời gian cho màn hình Live.
4. Vẽ lại LCD nếu nội dung thay đổi.

Không dùng `delay()` và không chờ nhả phím bằng vòng `while`, nên chương trình vẫn phản hồi.

## 8. Build trên VS Code

1. Cài extension PlatformIO IDE.
2. Chọn File → Open Folder.
3. Mở đúng thư mục `ES_LCD_Menu` chứa `platformio.ini`.
4. Chọn PlatformIO → Project Tasks → uno → General → Build.
5. Khi hiện SUCCESS, lấy:

```text
.pio/build/uno/firmware.hex
```

Không bấm Run Code hoặc F5 vì đây không phải chương trình Windows.

## 9. Nạp HEX vào Proteus

1. Dừng mô phỏng.
2. Double-click Arduino Uno R3.
3. Tại Program File, chọn `.pio/build/uno/firmware.hex`.
4. Clock Frequency để 16 MHz.
5. Nhấn OK rồi Run.
6. Xoay biến trở VEE nếu LCD chỉ hiện ô vuông hoặc không thấy chữ.

Sau mỗi lần sửa code: Save → Build → Stop Proteus → chọn lại HEX nếu cần → Run.

## 10. Trình tự kiểm thử menu

### Sensors

1. Mở mô phỏng: LCD hiện Main / Sensors.
2. OK → Temperature.
3. OK → Display.
4. OK → xem nhiệt độ.
5. BACK → Display; DOWN → Setup; OK.
6. UP/DOWN chỉnh giới hạn, OK lưu hoặc BACK hủy.

### Relay

1. Về Main.
2. DOWN → Actuators; OK.
3. OK tại Relays.
4. OK tại Relay 1.
5. OK tại ON: LCD báo Relay 1: ON.
6. BACK; DOWN → OFF; OK.

### LED

1. Trong Actuators, chọn LEDs.
2. OK → LED → OK tại ON.
3. LED D13 sáng.
4. BACK → DOWN → OFF → OK; LED tắt.

## 11. Lỗi thường gặp

| Hiện tượng | Kiểm tra |
|---|---|
| LCD tối/ô vuông | Nguồn LCD, RW=GND, xoay biến trở VEE |
| LCD không có chữ | RS/E/DB4..DB7 phải đúng D6..D11 |
| Bấm nút không đổi | Nút phải nối giữa D2..D5 và GND, không nối sang +5 V |
| Một nút luôn bị nhấn | Đang dùng nhầm hai chân cùng phía của button bốn chân |
| Menu chạy nhiều bước | Kiểm tra GND chung và không nối chập các chân D2..D5 |
| Proteus vẫn chạy code cũ | Build lại, Stop, kiểm tra đúng đường dẫn firmware.hex |
| F5 báo lỗi | Dùng PlatformIO Build, không dùng trình chạy C++ trên Windows |

## 12. Giới hạn của bản mô phỏng

- Uno R3 và LCD/nút là thiết kế thật của mạch.
- Relay/motor đang là trạng thái logic trên LCD, chưa xuất ra module công suất.
- Temperature dùng LM35 A0; Humidity dùng biến trở A1 để mô phỏng.
- Time là thời gian từ lúc Uno khởi động, không phải đồng hồ RTC.
- Các giá trị Setup nằm trong RAM, reset mô phỏng sẽ về mặc định.
