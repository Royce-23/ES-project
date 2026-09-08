# ES LCD Menu — Arduino Uno R3

Project PlatformIO cho Arduino Uno R3, LCD 16x2, bốn nút UP/DOWN/OK/BACK.
Menu dùng danh sách liên kết đôi vòng và function pointer, triển khai theo sơ đồ:

- Main: Sensors, Actuators, Setup
- Sensors: Temperature, Humidity, Time; mỗi mục có Display và Setup
- Actuators: Relays, Motors, LEDs
- Relays: Relay 1..4; mỗi relay có ON/OFF
- Motors: Motor 1..2; mỗi motor có ON/OFF
- LEDs: LED; có ON/OFF

Mở thư mục chứa `platformio.ini` bằng VS Code + PlatformIO, sau đó Build.
File nạp Proteus: `.pio/build/uno/firmware.hex`.

Đọc hướng dẫn đầy đủ tại `docs/HUONG_DAN_UNO_R3_PROTEUS.md`.
