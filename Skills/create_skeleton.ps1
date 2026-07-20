param(
    [Parameter(Mandatory=$true)]
    [string]$TargetPath,

    [Parameter(Mandatory=$false)]
    [string]$Name = "Project"
)

# Crea la carpeta destino si no existe y copia las plantillas desde la carpeta Skills/templates
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$templatesDir = Join-Path $scriptDir "templates"

if (-not (Test-Path $templatesDir)) {
    Write-Error "No se encontró la carpeta de plantillas: $templatesDir"
    exit 1
}

if (-not (Test-Path $TargetPath)) {
    New-Item -ItemType Directory -Path $TargetPath -Force | Out-Null
    Write-Host "Creada carpeta: $TargetPath"
}

# Mapeo de plantillas a nombres de salida
$files = @(
    @{tpl="efi.c.tpl"; out="efi.c"},
    @{tpl="efi.h.tpl"; out="efi.h"},
    @{tpl="asm.s.tpl"; out="asm.s"}
)

foreach ($f in $files) {
    $src = Join-Path $templatesDir $f.tpl
    $dst = Join-Path $TargetPath $f.out
    if (-not (Test-Path $src)) { Write-Warning "Plantilla faltante: $src"; continue }
    $content = Get-Content -Raw -Path $src
    $content = $content -replace "\{\{NAME\}\}", $Name
    Set-Content -Path $dst -Value $content -Encoding UTF8
    Write-Host "Generado: $dst"
}

Write-Host "Esqueletos generados en: $TargetPath"
