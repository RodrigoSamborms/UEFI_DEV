@echo -off
cls

echo "====================================================="
echo " Script UEFI .nsh con Autodeteccion de Unidad (FSx:)"
echo "====================================================="

set -v TARGET_RESET warm   
set -v COUNTER_FILE "counter.txt"
set -v MAX_REBOOTS 3

# -----------------------------------------------------------------
# DETECCIÓN AUTOMÁTICA DE LA UNIDAD DE DISCO
# -----------------------------------------------------------------
# Recorremos desde FS0 hasta FS9 buscando dónde operar.
# Si el archivo 'counter.txt' ya existe en alguna unidad, nos movemos ahí.
# Si no existe, buscamos dónde está este script (startup.nsh) para inicializarlo.

for %d in fs0 fs1 fs2 fs3 fs4 fs5 fs6 fs7 fs8 fs9 do
    if exist %d:\%COUNTER_FILE% then
        %d:
        goto FOUND_DRIVE
    endif
endfor

for %d in fs0 fs1 fs2 fs3 fs4 fs5 fs6 fs7 fs8 fs9 do
    if exist %d:\startup.nsh or exist %d:\reboot_test.nsh then
        %d:
        goto FOUND_DRIVE
    endif
endfor

# Si de plano no encontró ningún FS accesible, usamos FS0 por defecto
echo "Advertencia: No se detecto la unidad de forma automatica. Usando FS0:..."
fs0:

:FOUND_DRIVE
# -----------------------------------------------------------------
# LOGICA DEL CONTADOR DE REINICIOS
# -----------------------------------------------------------------

# Verificar si el archivo ya existe en la unidad actual
if not exist %COUNTER_FILE% then
    echo "Ciclo de ejecucion actual: 1 de %MAX_REBOOTS%"
    echo 2>%COUNTER_FILE%
    goto DO_RESET
endif

# Leer el estado guardado
for %i in %COUNTER_FILE% do
    set -v CURRENT_CYCLE %i
endfor

# Evaluar el estado actual
if %CURRENT_CYCLE% == "2" then
    echo "Ciclo de ejecucion actual: 2 de %MAX_REBOOTS%"
    echo 3>%COUNTER_FILE%
    goto DO_RESET
endif

if %CURRENT_CYCLE% == "3" then
    echo "Ciclo de ejecucion actual: 3 de %MAX_REBOOTS%"
    echo FIN>%COUNTER_FILE%
    goto DO_RESET
endif

if %CURRENT_CYCLE% == "FIN" then
    echo "¡Se ha alcanzado el limite maximo de %MAX_REBOOTS% reinicios!"
    echo "Limpiando archivos temporales y finalizando prueba..."
    
    rm %COUNTER_FILE%
    set -d TARGET_RESET
    set -d COUNTER_FILE
    set -d CURRENT_CYCLE
    goto EXIT
endif

:DO_RESET
echo "Unidad de almacenamiento actual: %lasterror%" # Muestra indirectamente dónde está parado
echo "Preparando el sistema para reinicio tipo: %TARGET_RESET%..."
echo "Reiniciando en 4 segundos (Presione Ctrl+C para cancelar)..."
stall 4000000

if %TARGET_RESET% == "warm" then
    reset -w
else
    reset -c
endif

:EXIT
echo "Prueba terminada de manera exitosa."