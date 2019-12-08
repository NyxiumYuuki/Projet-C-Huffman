# **Projet-C-Huffman**
**Projet C Huffman commencé le 2 Décembre 2019 (Compression et décompression d'un fichier)**.

*Collaborateurs : Yûki VACHOT, Guillaume XIUME, Wilfried VALLEE, Florian COQUILLAT*.

## **COMMENT COMMIT AVEC VISUAL STUDIO CODE :**
*(Commit : soumettre, valider des fichiers)*

1. Si ce n'est pas déjà fait télécharger **[Visual Studio Code](https://code.visualstudio.com/Download)** *(Windows, Linux ou Mac)*.
2. Créer un compte sur **[GitHub](https://github.com/)**.
3. Donner à **Yûki** votre nom de compte GitHub ou votre adresse e-mail associée (Je vous rajouterais au répertoire).
4. Véfifier si vous avez déjà **Git** avec un terminal en mettant `git --version`.
   - Vérifier que vous avez une version supérieure à 2.0.
   - Sinon télécharger **[Git](https://git-scm.com/downloads)**.
5. Aller sur VSCode puis créez/ouvrez un dossier nommé "**github-projet**" par exemple pour le projet *(CTRL + K ou O par défaut)*.
6. Aller dans l'onglet "Contrôle de Code Source" ![Img VSCode Contrôle de Code Source VSCode](https://cdn.vachot.fr/img/vscode_icon_controle_code_source_25x22.png) *(CTRL + MAJ + G par défaut)* .
   - Note : vérifier que **Git est activé** dans les paramètres de Git sur VSCode. 
     - `Fichier -> Préférences -> Paramètres -> Utilisateur -> Extensions -> Git -> Enabled -> Yes or No`
7. Ouvrir votre dossier dans l'onglet "Contrôle de Code Source" si ce n'est pas déjà fait avec le petit bouton à sa droite.
8. Dans la partie haut gauche mettez un message clair pour définir le commit puis pressez sur Entrer.
   - Le changement ne s'est pas encore effectué sur le dossier dans **GitHub**, il ne l'est que localement pour l'instant. Donc, il faut associer votre dossier au répertoire sur **GitHub** (Vous ne pourrez pas encore "push" vos changements).

9. Ajouter le répertoire distant
   - Ouvrir la palette de commande *(CTRL + MAJ + P par défaut)*
   - Rechercher `Git Add Remote` et pressez entrer
   - Mettez un nom pour vous afin de reconnaître le dépôt
   - Puis mettez ce lien : **https://github.com/NyxiumYuuki/Projet-C-Huffman.git**
   - Récupérer les fichiers déjà existant dans le répertoire de **GitHub** en allant dans le "Contrôle de Code Source" ![Img VSCode Contrôle de Code Source VSCode](https://cdn.vachot.fr/img/vscode_icon_controle_code_source_25x22.png) *(CTRL + MAJ + G par défaut)*
     - Plus d'actions... -> Pull (Rebaser)
       - !!ATTENTION!! Il est possible qu'il vous manque des clés pour pouvoir récupérer les fichiers du projet (Contactez Yûki pour ça)
   - Publier enfin vos changements en appuyant sur le bouton en bas à droite ![Img VSCode Publier Modifications](https://cdn.vachot.fr/img/vscode_icon_publish_modifications_25x21.png) *(Il faudra vous connecter à votre compte GitHub)*
   - ***Félicitations vous avez enfin fait votre premier commit !***
10. Vous pouvez maintenant voir les changements entre les différents fichiers, voir l'avancement des autres collaborateurs et vous pouvez aussi commit vos changements en répétant les **étapes 7 et 8**


## **TRAVAIL A FAIRE ET ORGANISATION DES FICHIERS**

### TRAVAIL A FAIRE
1. **GESTION DES FICHIERS**
   - Structure Gérer  les  échanges  avec  le  disque,  mémorisant  notamment l’état des échanges en cours 
   - Fonction Ouvrir un fichier binaire en lecture ou écriture
   - Fonction Ecrire un bit dans un fichier binaire
   - Fonction Lire un fichier binaire
   - Fonction Fermer un fichier
2. **ARBRE DE CODAGE**
   - Fonction Fréquence d'apparition des caractères dans un fichier
     - *Tri à faire pour les occurences des caractères(Quicksort)*
   - Implémentation de l'algorithme de construction de l'arbre de codage (Module Arbres_Binaires)
     - *Tri à faire (Bulle)*

3. **COMPRESSION**
   - Fonction de compression d'un fichier texte dans un fichier binaire
4. **DECOMPRESSION (EXTRACTION)**
   - Fonction d'extraction d'un fichier texte depuis un fichier compressé

### ORGANISATION DES FICHIERS

- **Racine**
  - Makefile
  - main_compress.c
  - main_decompress.c
  - **/gestion_des_fichiers**
    - gestion_fichiers.h
    - gestion_fichiers.c
  - **/arbre_de_codage**
    - arbre_binaire.h
    - arbre_binaire.c
    - liste.h
      *(Utile pour stocker le nombre d'occurence d'un caractère)*
    - liste.c 

## **INSTRUCTION A RESPECTER**
- Lors du Commit rajouter votre prénom au début
- Commenter les fonctions et autres lignes de code pour que tout le monde puisse comprendre
- *Autres idée ?*