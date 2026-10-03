$sizes = 100, 200, 300

Remove-Item "benchmark.csv" -ErrorAction SilentlyContinue

foreach ($n in $sizes) {
    Write-Host "=== Running N = $n ==="

    (Get-Content gen.py) -replace '^SIZE\s*=\s*\d+', "SIZE = $n" | Set-Content gen.py

    py gen.py

    .\Lab1.exe
}