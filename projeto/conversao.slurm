#!/bin/bash
#SBATCH --job-name=translate_dna_rna        # Nome do job
#SBATCH --output=translate_dna_rna_%j.txt   # Arquivo de saída (%j será substituído pelo ID do job)
#SBATCH --ntasks=4                            # Número de tarefas MPI (ajuste conforme necessário)
#SBATCH --cpus-per-task=8                     # Número de threads OpenMP por processo MPI
#SBATCH --mem=8GB                             # Memória total alocada por nó (ajuste conforme necessário)
#SBATCH --time=00:20:00                       # Tempo máximo de execução (20 minutos)
#SBATCH --partition=espec                     # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Lista de arquivos DNA (ajuste o caminho conforme necessário)
# Supondo que os arquivos estejam no diretório /home/talesitdf/SCRATCH/projeto/arquivos/
DNA_FILES=(/home/talesitdf/SCRATCH/projeto/arquivos/chr*.subst.fa)

# Verificar se há arquivos para processar
if [ ${#DNA_FILES[@]} -eq 0 ]; then
    echo "Nenhum arquivo DNA encontrado para processar."
    exit 1
fi

# Executa o programa híbrido MPI + OpenMP passando os arquivos como argumentos
mpirun -np $SLURM_NTASKS ./dna_rna "${DNA_FILES[@]}"
