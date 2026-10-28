param()

$ErrorActionPreference = 'Stop'

$xc8Root = Join-Path $env:ProgramFiles 'Microchip\xc8'
$mplabRoot = Join-Path $env:ProgramFiles 'Microchip\MPLABX'
$xc8 = Get-ChildItem -LiteralPath $xc8Root -Directory -ErrorAction SilentlyContinue |
    Sort-Object Name -Descending | Select-Object -First 1
$dfp = Get-ChildItem -Path (Join-Path $mplabRoot '*\packs\Microchip\ATmega_DFP') -Directory -ErrorAction SilentlyContinue |
    Sort-Object Name -Descending | Select-Object -First 1

Write-Host 'Kiem tra moi truong build ATmega128...' -ForegroundColor Cyan
if (-not $xc8) {
    Write-Host 'THIEU: Microchip MPLAB XC8 (ban AVR).' -ForegroundColor Yellow
    Write-Host 'Tai tu trang chinh thuc: https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers' -ForegroundColor Yellow
} else {
    Write-Host ("OK: XC8 " + $xc8.Name) -ForegroundColor Green
}
if (-not $dfp) {
    Write-Host 'THIEU: Microchip ATmega Device Family Pack.' -ForegroundColor Yellow
    Write-Host 'Co the cai kem trong MPLAB X IDE.' -ForegroundColor Yellow
} else {
    Write-Host ("OK: ATmega_DFP " + $dfp.Name) -ForegroundColor Green
}
if ($xc8 -and $dfp) {
    Write-Host 'Moi truong da san sang. Chay .\build.ps1 de bien dich.' -ForegroundColor Green
    exit 0
}
Write-Host ''
Write-Host 'Sau khi cai xong, mo PowerShell tai thu muc nay va chay lai .\setup.ps1.' -ForegroundColor Cyan
exit 1
