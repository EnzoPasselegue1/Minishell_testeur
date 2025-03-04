#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD_LEN 1024
#define TMP_MINISHELL_OUTPUT "minishell_output.txt"
#define TMP_BASH_OUTPUT "bash_output.txt"

// Fonction pour exécuter une commande et enregistrer sa sortie dans un fichier
void execute_command(const char *command, const char *shell, const char *output_file) {
    char full_command[MAX_CMD_LEN];
    snprintf(full_command, sizeof(full_command), "%s -c \"%s\" > %s 2>&1", shell, command, output_file);
    system(full_command);
}

// Fonction pour comparer deux fichiers de sortie
int compare_outputs(const char *file1, const char *file2) {
    char cmd[MAX_CMD_LEN];
    snprintf(cmd, sizeof(cmd), "diff -q %s %s > /dev/null", file1, file2);
    return system(cmd) == 0; // Renvoie 1 si les fichiers sont identiques, sinon 0
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <minishell_path> <test_file>\n", argv[0]);
        return 1;
    }

    char *minishell_path = argv[1];
    char *test_file = argv[2];
    FILE *fp = fopen(test_file, "r");
    if (!fp) {
        perror("Erreur ouverture fichier de test");
        return 1;
    }

    char command[MAX_CMD_LEN];
    int test_count = 0, passed_count = 0;

    while (fgets(command, sizeof(command), fp)) {
        // Supprime le saut de ligne
        command[strcspn(command, "\n")] = 0;
        if (strlen(command) == 0) continue; // Ignore les lignes vides

        printf("Test %d: %s\n", ++test_count, command);
        execute_command(command, minishell_path, TMP_MINISHELL_OUTPUT);
        execute_command(command, "/bin/bash", TMP_BASH_OUTPUT);

        if (compare_outputs(TMP_MINISHELL_OUTPUT, TMP_BASH_OUTPUT)) {
            printf("\033[1;32m[OK]\033[0m\n");
            passed_count++;
        } else {
            printf("\033[1;31m[FAIL]\033[0m\n");
        }
    }

    fclose(fp);
    printf("\nRésultats: %d/%d tests réussis.\n", passed_count, test_count);
    return 0;
}
