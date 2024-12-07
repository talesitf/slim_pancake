#!/bin/bash
#SBATCH --job-name=dna_count             # Nome do job
#SBATCH --output=dna_count_%j.txt        # Arquivo de saída (%j será substituído pelo job ID)
#SBATCH --ntasks=4                       # Número de tarefas MPI (4 processos)
#SBATCH --cpus-per-task=8                # Número de threads OpenMP por processo
#SBATCH --mem=4GB                        # Memória total alocada (ajuste conforme necessário)
#SBATCH --time=01:00:00                  # Tempo máximo de execução (1 hora)
#SBATCH --partition=espec               # Fila do cluster

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Lista de arquivos FASTA
FILES=("arquivos/chr1.subst.fa" "arquivos/chr2.subst.fa" "arquivos/chr3.subst.fa" "arquivos/chr4.subst.fa")

# Verifica se o número de arquivos corresponde ao número de processos
if [ "${#FILES[@]}" -ne $SLURM_NTASKS ]; then
    echo "Erro: O número de arquivos FASTA não corresponde ao número de processos MPI."
    exit 1
fi

# Executa o programa usando MPI, passando os arquivos como argumentos
mpirun -np $SLURM_NTASKS ./contagem "${FILES[@]}"
