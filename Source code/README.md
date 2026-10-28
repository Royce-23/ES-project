# ES Project - build ATmega128

## Cài đặt lần đầu

Cài **MPLAB XC8 bản AVR** và **MPLAB X IDE** (MPLAB X cung cấp ATmega Device Family Pack), sau đó chạy:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\setup.ps1
```

Không cần cài Python, CMake, Make hoặc npm.

## Biên dịch

Build mặc định (P1.8):

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\build.ps1
```

Chọn bài khác bằng tham số `-Source`, ví dụ:

```powershell
.\build.ps1 -Source 'P1.3 main.c'
```

File nạp Proteus được tạo tại `build\ES-project.hex`.

## Mô phỏng

Proteus chỉ cần cài nếu muốn mô phỏng. Nếu chỉ cần lấy file HEX mẫu, xem thư mục `release`.
