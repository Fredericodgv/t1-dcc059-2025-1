# ===================================================================
# SCRIPT DE EXECUÇÃO DE TESTES PARA ALGORITMOS GULOSOS (VERSÃO CORRIGIDA)
# ===================================================================

# --- PARÂMETROS DE CONFIGURAÇÃO ---
$caminhoExecutavel = ".\meu_programa.exe"
$pastaInstancias = ".\instancias\trabalho2"
$ficheiroSaida = "resultados_execucao.txt"

# --- Parâmetros para os Algoritmos ---
$iteracoesRandomizado = 30
$alfaRandomizadoA = 0.1
$alfaRandomizadoB = 0.3
$alfaRandomizadoC = 0.5
$nExecucoesReativo = 10
$iteracoesReativo = 300

# ===================================================================
# INÍCIO DO SCRIPT
# ===================================================================

if (Test-Path $ficheiroSaida) {
    Remove-Item $ficheiroSaida
    Write-Host "Ficheiro de resultados anterior ($ficheiroSaida) removido."
}

# Inicia o registo de toda a saída do terminal
Start-Transcript -Path $ficheiroSaida -Append

Write-Host "======================================================="
Write-Host "INICIANDO BATERIA DE TESTES"
Write-Host "Data e Hora: $(Get-Date)"
Write-Host "======================================================="
Write-Host ""

$ficheirosInstancia = Get-ChildItem -Path $pastaInstancias -File

foreach ($ficheiro in $ficheirosInstancia) {
    
    $caminhoInstancia = $ficheiro.FullName
    Write-Host "-------------------------------------------------------"
    Write-Host "PROCESSANDO INSTÂNCIA: $($ficheiro.Name)"
    Write-Host "-------------------------------------------------------"
    Write-Host ""

    # --- 1. Execução do Algoritmo Guloso Randomizado A ('k') ---
    Write-Host "*** EXECUTANDO ALGORITMO GULOSO RANDOMIZADO A ***"
    $inputRandomizado = "k`n$iteracoesRandomizado`n$alfaRandomizadoA`n0"
    # --- CORREÇÃO AQUI: Captura a saída para uma variável e imprime ---
    $outputRandomizado = ($inputRandomizado | & $caminhoExecutavel $caminhoInstancia | Out-String)
    Write-Host $outputRandomizado
    Write-Host "-------------------------------------------"
    Write-Host ""

    # --- 2. Execução do Algoritmo Guloso Randomizado B ('k') ---
    Write-Host "*** EXECUTANDO ALGORITMO GULOSO RANDOMIZADO B ***"
    $inputRandomizado = "k`n$iteracoesRandomizado`n$alfaRandomizadoB`n0"
    # --- CORREÇÃO AQUI: Captura a saída para uma variável e imprime ---
    $outputRandomizado = ($inputRandomizado | & $caminhoExecutavel $caminhoInstancia | Out-String)
    Write-Host $outputRandomizado
    Write-Host "-------------------------------------------"
    Write-Host ""

    # --- 3. Execução do Algoritmo Guloso Randomizado C ('k') ---
    Write-Host "*** EXECUTANDO ALGORITMO GULOSO RANDOMIZADO C ***"
    $inputRandomizado = "k`n$iteracoesRandomizado`n$alfaRandomizadoC`n0"
    # --- CORREÇÃO AQUI: Captura a saída para uma variável e imprime ---
    $outputRandomizado = ($inputRandomizado | & $caminhoExecutavel $caminhoInstancia | Out-String)
    Write-Host $outputRandomizado
    Write-Host "-------------------------------------------"
    Write-Host ""

    # --- 4. Execução do Algoritmo Guloso Reativo ('l') ---
    Write-Host "*** EXECUTANDO ALGORITMO GULOSO REATIVO ***"
    $inputReativo = "l`n$nExecucoesReativo`n$iteracoesReativo`n0"
    # --- CORREÇÃO AQUI: Captura a saída para uma variável e imprime ---
    $outputReativo = ($inputReativo | & $caminhoExecutavel $caminhoInstancia | Out-String)
    Write-Host $outputReativo
    Write-Host "-----------------------------------------"
    Write-Host ""
}

Write-Host "======================================================="
Write-Host "BATERIA DE TESTES FINALIZADA"
Write-Host "Resultados guardados em: $ficheiroSaida"
Write-Host "======================================================="

Stop-Transcript