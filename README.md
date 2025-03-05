# Minishell_testeur
Testeur Minishell
Un testeur automatique pour minishell, qui compare les résultats de ton minishell avec le vrai bash.

🚀 Fonctionnalités
✅ Exécute automatiquement des tests sur minishell
✅ Compare les sorties avec /bin/bash
✅ Affiche seulement PASS ou FAIL dans la console
✅ Stocke les différences dans test_results.log pour analyse
✅ Menu interactif si l'utilisateur entre une commande invalide

📥 Installation
Clone le repository et compile le programme :

bash
Copier
Modifier
git clone https://github.com/<ton-utilisateur>/testeur-minishell.git
cd testeur-minishell
gcc test_minishell.c -o test_minishell
🎯 Utilisation
1️⃣ Lancer un test
Utilise la commande suivante pour exécuter un test :

bash
Copier
Modifier
./test_minishell <chemin_minishell> <numéro_test>
📌 Exemple :

bash
Copier
Modifier
./test_minishell ./minishell 3
👉 Cela exécutera les tests du fichier echo.txt.

📜 Liste des tests disponibles
Numéro	Fichier de test
1	all.txt
2	cat.txt
3	echo.txt
4	env.txt
5	export.txt
6	invalid.txt
📄 Résultats des tests
Dans la console : uniquement PASS ou FAIL
Les différences sont enregistrées dans test_results.log
📌 Consulter les résultats détaillés :

bash
Copier
Modifier
cat test_results.log
🔧 Exemple de sortie
bash
Copier
Modifier
$ ./test_minishell ./minishell 3

🛠️ Exécution des tests depuis : echo.txt

✅ Test 1 PASS
✅ Test 2 PASS
❌ Test 3 FAIL
✅ Test 4 PASS

📄 Résumé des tests enregistré dans `test_results.log`
📌 Si un test échoue (FAIL), vérifie test_results.log pour voir les différences avec bash.

🆘 Problèmes et Débogage
Erreur : "numéro de test invalide"
👉 Vérifie que tu utilises un numéro entre 1 et 6. Le 7 pour t'es tests

Erreur : "minishell introuvable"
👉 Vérifie que ton minishell est bien compilé et accessible à l'emplacement donné.

Les tests ne correspondent pas à mon bash
👉 Vérifie que ton minishell gère bien les commandes testées.

🛠️ Améliorations possibles
✅ Ajouter des tests supplémentaires sur le fichier mytests (7)
✅ Vérifier les codes de retour ($?)
✅ Ignorer les espaces en trop pour éviter les différences mineures

👨‍💻 Auteurs
EnzoPasselegue1 - GitHub
📝 Licence
Ce projet est sous licence MIT – Utilisation libre et open-source ! 🚀
