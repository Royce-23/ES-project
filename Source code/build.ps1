param(
    [switch]$Clean,
    [string]$Source = 'P1.8 main.c'
)

$ErrorActionPreference = 'Stop'

$projectDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $projectDir 'build'

if ($Clean) {
    $projectFullPath = [System.IO.Path]::GetFullPath($projectDir)
    $buildFullPath = [System.IO.Path]::GetFullPath($buildDir)
    if (-not $buildFullPath.StartsWith($projectFullPath + [System.IO.Path]::DirectorySeparatorChar)) {
        throw 'Duong dan build khong hop le.'
    }
    if (Test-Path -LiteralPath $buildFullPath) {
        Remove-Item -LiteralPath $buildFullPath -Recurse -Force
    }
    Write-Host 'Da xoa folder build.' -ForegroundColor Green
    exit 0
}

$xc8Base = Join-Path $env:ProgramFiles 'Microchip\xc8'
$xc8Version = Get-ChildItem -LiteralPath $xc8Base -Directory -ErrorAction Stop |
    Sort-Object { [version]($_.Name -replace '^v', '') } -Descending |
    Select-Object -First 1

if (-not $xc8Version) {
    throw 'Khong tim thay MPLAB XC8 trong Program Files\Microchip\xc8.'
}

$compiler = Join-Path $xc8Version.FullName 'bin\xc8-cc.exe'
$objcopy = Join-Path $xc8Version.FullName 'bin\avr-objcopy.exe'

$mplabBase = Join-Path $env:ProgramFiles 'Microchip\MPLABX'
$dfpVersions = foreach ($mplabVersion in Get-ChildItem -LiteralPath $mplabBase -Directory -ErrorAction Stop) {
    $dfpBase = Join-Path $mplabVersion.FullName 'packs\Microchip\ATmega_DFP'
    if (Test-Path -LiteralPath $dfpBase) {
        Get-ChildItem -LiteralPath $dfpBase -Directory
    }
}
$dfpVersion = $dfpVersions |
    Sort-Object { [version]$_.Name } -Descending |
    Select-Object -First 1

if (-not $dfpVersion) {
    throw 'Khong tim thay Microchip ATmega Device Family Pack.'
}

$dfpPath = Join-Path $dfpVersion.FullName 'xc8'
$projectFullPath = [System.IO.Path]::GetFullPath($projectDir)
$sourceFullPath = [System.IO.Path]::GetFullPath((Join-Path $projectDir $Source))
if (-not $sourceFullPath.StartsWith($projectFullPath + [System.IO.Path]::DirectorySeparatorChar)) {
    throw 'File nguon phai nam trong thu muc project.'
}
if (-not (Test-Path -LiteralPath $sourceFullPath -PathType Leaf)) {
    throw "Khong tim thay file nguon: $Source"
}
if ([System.IO.Path]::GetExtension($sourceFullPath).ToLowerInvariant() -ne '.c') {
    throw 'File nguon phai co phan mo rong .c.'
}
$source = $sourceFullPath
$elf = Join-Path $buildDir 'ES-project.elf'
$hex = Join-Path $buildDir 'ES-project.hex'
$map = Join-Path $buildDir 'ES-project.map'

New-Item -ItemType Directory -Path $buildDir -Force | Out-Null

Write-Host 'Dang bien dich cho ATmega128...' -ForegroundColor Cyan
& $compiler `
    '-mcpu=ATmega128' `
    '-x' 'c' `
    "-mdfp=$dfpPath" `
    '-O1' `
    '-ffunction-sections' `
    '-fdata-sections' `
    '-fshort-enums' `
    '-fno-common' `
    '-funsigned-char' `
    '-funsigned-bitfields' `
    '-Wall' `
    '-gdwarf-3' `
    '-mno-const-data-in-progmem' `
    '-Wl,--gc-sections' `
    "-Wl,-Map=$map" `
    '-o' $elf `
    $source `
    '-Wl,--start-group' `
    '-Wl,-lm' `
    '-Wl,--end-group'

if ($LASTEXITCODE -ne 0) {
    throw "Bien dich that bai (ma loi $LASTEXITCODE)."
}

& $objcopy '-O' 'ihex' $elf $hex
if ($LASTEXITCODE -ne 0) {
    throw "Tao file HEX that bai (ma loi $LASTEXITCODE)."
}

Write-Host ''
Write-Host 'BUILD THANH CONG' -ForegroundColor Green
Write-Host "File nap Proteus: $hex" -ForegroundColor Green
