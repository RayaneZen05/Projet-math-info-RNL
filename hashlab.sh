#!/bin/bash

# Ce script a pour objectif de hash chaque fichier de media, et de les comparer entre eux
# pour vérifier si des fichiers sont identiques
FILE_LIST=()
HASH_LIST=()
cd media
for file in laby*; do
    echo "hashing $file"
    if [ -f "$file" ]; then
        HASH=$(sha256sum "$file" | cut -d ' ' -f 1)
        # verifier si le hash est deja dans la liste
        for i in "${!HASH_LIST[@]}"; do
            if [ "${HASH_LIST[$i]}" == "$HASH" ]; then
                echo "Le fichier $file est identique au fichier ${FILE_LIST[$i]}"
                continue 2 # skip la boucle externe
            fi
        done
        FILE_LIST+=("$file")
        HASH_LIST+=("$HASH")
    fi
done
echo "terminé"
# si labuniques existe deja, on le supprime
if [ -d "labuniques" ]; then
    rm -r labuniques
fi
mkdir labuniques
cd ..
for i in "${!FILE_LIST[@]}"; do
    cp "media/${FILE_LIST[$i]}" "media/labuniques/${FILE_LIST[$i]}"
done
echo "${FILE_LIST[@]}" > labuniques.txt