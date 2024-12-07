#!/bin/bash

# Base URL para download
BASE_URL="ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask"

# Loop para X de 1 a 22
for X in {1..22}
do
    # Nome do arquivo
    FILE="chr${X}.subst.fa.gz"
    
    # Verifica se o arquivo já foi baixado
    if [ ! -f "$FILE" ]; then
        echo "Baixando $FILE..."
        wget "${BASE_URL}/${FILE}" -q --show-progress
    else
        echo "$FILE já foi baixado."
    fi

    # Verifica se o arquivo precisa ser descompactado
    if [ -f "$FILE" ]; then
        echo "Descompactando $FILE..."
        gunzip -k "$FILE"  # O "-k" mantém o arquivo original compactado
    else
        echo "Erro: $FILE não encontrado para descompactar."
    fi
done

echo "Download e descompactação concluídos!"
