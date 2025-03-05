#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define TMP_OUTPUT_MINISHELL "output_minishell.txt"
#define TMP_OUTPUT_BASH "output_bash.txt"
#define CLEAN_OUTPUT_MINISHELL "clean_minishell.txt"
#define CLEAN_OUTPUT_BASH "clean_bash.txt"
#define TEST_RESULTS_FILE "test_results.log"  // 🔹 Fichier où sont enregistrées les différences

// 🔹 Liste des fichiers de test disponibles
const char *test_files[] = {
    "test/all.txt",
    "test/cat.txt",
    "test/echo.txt",
    "test/env.txt",
    "test/export.txt",
    "test/invalid.txt",
    "test/mytests.txt"
};
const int num_tests = sizeof(test_files) / sizeof(test_files[0]);

// 🔹 Exécute une commande dans minishell ou bash et redirige la sortie vers un fichier
void execute_shell_command(const char *command, const char *shell, const char *output_file) {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid == 0) {
        int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd[0], STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        dup2(out_fd, STDERR_FILENO);
        close(fd[0]);
        close(fd[1]);
        close(out_fd);

        execlp(shell, shell, NULL);
        perror("execlp failed");
        exit(EXIT_FAILURE);
    } else {
        close(fd[0]);
        write(fd[1], command, strlen(command));
        write(fd[1], "\nexit\n", 6);
        close(fd[1]);
        waitpid(pid, NULL, 0);
    }
}

// 🔹 Nettoie les fichiers de sortie pour enlever le prompt "minishell$>"
void clean_output(const char *input_file, const char *clean_file) {
    FILE *in = fopen(input_file, "r");
    FILE *out = fopen(clean_file, "w");

    if (!in || !out) {
        perror("Erreur ouverture fichier de nettoyage");
        return;
    }

    char line[BUFFER_SIZE];

    while (fgets(line, BUFFER_SIZE, in)) {
        if (strstr(line, "minishell$>") == NULL) {
            fputs(line, out);
        }
    }

    fclose(in);
    fclose(out);
}

// 🔹 Compare les fichiers nettoyés de minishell et bash
int compare_outputs() {
    FILE *file1 = fopen(CLEAN_OUTPUT_MINISHELL, "r");
    FILE *file2 = fopen(CLEAN_OUTPUT_BASH, "r");
    
    if (!file1 || !file2) {
        perror("Erreur ouverture fichier de sortie");
        return 0;
    }

    char line1[BUFFER_SIZE], line2[BUFFER_SIZE];
    int match = 1;

    while (fgets(line1, BUFFER_SIZE, file1) && fgets(line2, BUFFER_SIZE, file2)) {
        if (strcmp(line1, line2) != 0) {
            match = 0;
            break;
        }
    }

    if (match && (fgets(line1, BUFFER_SIZE, file1) || fgets(line2, BUFFER_SIZE, file2))) {
        match = 0;
    }

    fclose(file1);
    fclose(file2);
    
    return match;
}

// 🔹 Enregistre les différences dans `test_results.log`
void log_difference(FILE *log_file, int test_number, const char *command) {
    FILE *file1 = fopen(CLEAN_OUTPUT_MINISHELL, "r");
    FILE *file2 = fopen(CLEAN_OUTPUT_BASH, "r");

    if (!file1 || !file2) {
        fprintf(log_file, "Erreur ouverture des fichiers de sortie.\n");
        return;
    }

    fprintf(log_file, "🔹 Test %d: %s\n", test_number, command);
    fprintf(log_file, "📌 Différences entre minishell et bash :\n");
    fprintf(log_file, "=======================================\n");

    char line1[BUFFER_SIZE], line2[BUFFER_SIZE];
    int line_num = 1;

    while (fgets(line1, BUFFER_SIZE, file1) || fgets(line2, BUFFER_SIZE, file2)) {
        if (!line1[0]) strcpy(line1, "[VIDE]\n");
        if (!line2[0]) strcpy(line2, "[VIDE]\n");

        if (strcmp(line1, line2) != 0) {
            fprintf(log_file, "🔹 Ligne %d :\n", line_num);
            fprintf(log_file, "   minishell: %s", line1);
            fprintf(log_file, "   bash     : %s", line2);
        }
        line_num++;
    }

    fprintf(log_file, "=======================================\n\n");

    fclose(file1);
    fclose(file2);
}

// 🔹 Fonction principale : boucle sur les tests et enregistre les résultats
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <minishell_path> <numéro_test>\n", argv[0]);
        printf("🔹 Liste des tests disponibles :\n");
        for (int i = 0; i < num_tests; i++) {
            printf("   %d -> %s\n", i + 1, test_files[i]);
        }
         printf("\n🔹 EXEMPLE D'UTILISATION :\n");
        printf("    ./test_minishell ./minishell 3\n");
        printf("    (Exécute les tests de `echo.txt` dans minishell)\n");

        printf("\n📌 Le résultat des tests sera enregistré dans `%s`\n\n", TEST_RESULTS_FILE);
        return EXIT_FAILURE;
    }

    const char *minishell_path = argv[1];
    int test_choice = atoi(argv[2]) - 1;

    if (test_choice < 0 || test_choice >= num_tests) {
        fprintf(stderr, "❌ Erreur : numéro de test invalide.\n");
        return EXIT_FAILURE;
    }

    const char *test_file = test_files[test_choice];

    FILE *file = fopen(test_file, "r");
    if (!file) {
        perror("Erreur ouverture fichier de test");
        return EXIT_FAILURE;
    }

    FILE *log_file = fopen(TEST_RESULTS_FILE, "w");
    if (!log_file) {
        perror("Erreur ouverture fichier de log");
        fclose(file);
        return EXIT_FAILURE;
    }

    printf("\n🛠️ Exécution des tests depuis : %s\n", test_file);

    char command[BUFFER_SIZE];
    int test_number = 1;

    while (fgets(command, sizeof(command), file) != NULL) {
        command[strcspn(command, "\n")] = 0;

        execute_shell_command(command, minishell_path, TMP_OUTPUT_MINISHELL);
        execute_shell_command(command, "/bin/bash", TMP_OUTPUT_BASH);

        clean_output(TMP_OUTPUT_MINISHELL, CLEAN_OUTPUT_MINISHELL);
        clean_output(TMP_OUTPUT_BASH, CLEAN_OUTPUT_BASH);

        if (compare_outputs()) {
            printf("✅ Test %d PASS\n", test_number);
            fprintf(log_file, "✅ Test %d: %s - PASS\n", test_number, command);
        } else {
            printf("❌ Test %d FAIL\n", test_number);
            fprintf(log_file, "❌ Test %d: %s - FAIL\n", test_number, command);
            log_difference(log_file, test_number, command);
        }

        test_number++;
    }

    fclose(file);
    fclose(log_file);

    remove(TMP_OUTPUT_MINISHELL);
    remove(TMP_OUTPUT_BASH);
    remove(CLEAN_OUTPUT_MINISHELL);
    remove(CLEAN_OUTPUT_BASH);

    printf("\n📄 Résumé des tests enregistré dans `%s`\n", TEST_RESULTS_FILE);
    return EXIT_SUCCESS;
}
