$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $root

$sizes = 100, 200, 300, 400, 600, 800, 1000, 1200, 1600, 2000

Remove-Item "benchmark.csv" -ErrorAction SilentlyContinue

if (-not (Test-Path "Lab1.exe")) {
    Write-Host "ERROR: Lab1.exe not found in $root" -ForegroundColor Red
    exit 1
}

foreach ($n in $sizes) {
    Write-Host "=== Running N = $n ===" -ForegroundColor Cyan

    $content = Get-Content "gen.py" -Raw
    $newContent = $content -replace '(?m)^SIZE\s*=\s*\d+', "SIZE = $n"
    if ($content -eq $newContent) {
        Write-Host "  WARNING: SIZE line not found or not changed" -ForegroundColor Yellow
    }
    Set-Content -Path "gen.py" -Value $newContent -NoNewline

    Select-String -Path "gen.py" -Pattern "SIZE" | ForEach-Object { Write-Host "  $_" }

    # Сгенерировать матрицы
    py gen.py
   
    .\Lab1.exe
}

Write-Host "Done. Check benchmark.csv" -ForegroundColor Green