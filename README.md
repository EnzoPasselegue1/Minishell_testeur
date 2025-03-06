# Minishell Testeur 🚀

Un testeur automatique pour **minishell**, qui compare les résultats de ton minishell avec le vrai `bash`.

---

## 🌟 Fonctionnalités  
✅ Exécute automatiquement des tests sur `minishell`  
✅ Compare les sorties avec `/bin/bash`  
✅ Affiche seulement **PASS** ou **FAIL** dans la console  
✅ Stocke les différences dans **test_results.log** pour analyse  
✅ Menu interactif si l'utilisateur entre une commande invalide  

---

## 📥 Installation  

Clone le repository et compile le programme :  
Clone le repo dans ton projet ! 
Le testeur ce lance dans le repo !
```bash
git clone https://github.com/EnzoPasselegue1/Minishell_testeur.git
cd Minishell_testeur
gcc test_minishell.c -o test_minishell
```
## 🆘🆘Il faut apsolument changer readline comme cela : readline("minishell$>")🆘🆘  
## 🎯 Utilisation
1️⃣ Lancer un test
Utilise la commande suivante pour exécuter un test :

```bash
./test_minishell <chemin_minishell> <numéro_test>
```
📌 Exemple :
```bash
./test_minishell ./../minishell 3
```
👉 Cela exécutera les tests du fichier echo.txt.

## 📜 Liste des tests disponibles  

| Numéro | Fichier de test       | Description                          |
|--------|-----------------------|--------------------------------------|
| 1      | **all.txt**           | Tous les tests combinés             |
| 2      | **cat.txt**           | Tests liés à la commande `cat`      |
| 3      | **echo.txt**          | Tests pour la commande `echo`       |
| 4      | **env.txt**           | Vérification des variables d’environnement |
| 5      | **export.txt**        | Tests sur la commande `export`      |
| 6      | **invalid.txt**       | Tests de commandes invalides        |
| 7      | **mytest.txt**        | T'es propres tests                  |

##📄 Résultats des tests  
Dans la console : uniquement PASS ou FAIL  
Les différences sont enregistrées dans test_results.log  

📌 Consulter les résultats détaillés :

```bash
cat test_results.log
```
🔧 Exemple de sortie :

```bash
$ ./test_minishell ./../minishell 3

🛠️ Exécution des tests depuis : echo.txt

✅ Test 1 PASS  
✅ Test 2 PASS  
❌ Test 3 FAIL  
✅ Test 4 PASS  

📄 Résumé des tests enregistré dans test_results.log  
📌 Si un test échoue (**FAIL**), vérifie **test_results.log** pour voir les différences ave
```
🆘 Problèmes et Débogage  
❌ Erreur : "numéro de test invalide"  
👉 Vérifie que tu utilises un numéro entre 1 et 6.  
👉 Si tu veux ajouter un fichier personnalisé, modifie le code source.  

❌ Erreur : "minishell introuvable"  
👉 Vérifie que ton minishell est bien compilé et exécutable à l'emplacement donné.  

❌ Les tests ne correspondent pas à bash  
👉 Vérifie que ton minishell gère bien les commandes testées.  
👉 Assure-toi que le prompt est bien supprimé pour éviter des différences inutiles.  

🛠️ Améliorations possibles  
✅ Ajouter des tests supplémentaires (ex : mytests.txt)  
✅ Vérifier les codes de retour ($?)  
✅ Ignorer les espaces en trop pour éviter les différences mineures  

👨‍💻 Auteurs
EnzoPasselegue1 - GitHub

📝 Licence
Ce projet est sous licence MIT – Utilisation libre et open-source ! 🚀







