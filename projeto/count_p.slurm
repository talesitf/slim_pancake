#!/bin/bash
#SBATCH --job-name=count_proteins      # Nome do job
#SBATCH --output=count_proteins.txt    # Gera um novo arquivo de saída a cada execução
#SBATCH --ntasks=4                     # Número de tarefas (4 processos MPI)
#SBATCH --cpus-per-task=8              # Número de threads OpenMP por processo
#SBATCH --mem=4GB                      # Memória total alocada por nó
#SBATCH --time=01:00:00                # Tempo máximo de execução (até 1 hora)
#SBATCH --partition=espec             # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Arquivo de entrada com sequência de RNA
RNA_FILE="output_rna.fa"

# Executa o programa híbrido MPI + OpenMP
mpirun -np $SLURM_NTASKS ./count_p $RNA_FILE