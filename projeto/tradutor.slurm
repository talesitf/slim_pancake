#!/bin/bash
#SBATCH --job-name=translate_rna_proteins       # Nome do job
#SBATCH --output=translate_rna_proteins_%j.log  # Arquivo de saída (%j será substituído pelo ID do job)
#SBATCH --error=translate_rna_proteins_%j.err   # Arquivo de erro
#SBATCH --ntasks=4                                # Número de tarefas MPI (ajuste conforme necessário)
#SBATCH --cpus-per-task=8                         # Número de threads OpenMP por processo MPI
#SBATCH --mem=8GB                                 # Memória total alocada por nó (ajuste conforme necessário)
#SBATCH --time=00:20:00                           # Tempo máximo de execução (20 minutos)
#SBATCH --partition=espec                         # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Lista de arquivos DNA (assumindo que estão no diretório atual)
# Ajuste conforme necessário
DNA_FILES=(/home/talesitdf/SCRATCH/projeto/arquivos/chr*.subst.fa)

# Executa o programa híbrido MPI + OpenMP passando os arquivos como argumentos
mpirun -np $SLURM_NTASKS ./tradutor "${DNA_FILES[@]}"