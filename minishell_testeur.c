#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define TMP_OUTPUT_MINISHELL "output_minishell.txt"
#define TMP_OUTPUT_BASH "output_bash.txt"

void execute_shell_command(const char *command, const char *shell, const char *output_file) {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();

    if (pid == 0) {
        // Rediriger la sortie vers un fichier
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
        write(fd[1], "\nexit\n", 6);  // Fermer proprement minishell
        close(fd[1]);
        waitpid(pid, NULL, 0);
    }
}

int compare_outputs() {
    FILE *file1 = fopen(TMP_OUTPUT_MINISHELL, "r");
    FILE *file2 = fopen(TMP_OUTPUT_BASH, "r");
    
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

void show_difference() {
    FILE *file1 = fopen(TMP_OUTPUT_MINISHELL, "r");
    FILE *file2 = fopen(TMP_OUTPUT_BASH, "r");

    if (!file1 || !file2) {
        perror("Erreur ouverture fichier de sortie");
        return;
    }

    char line1[BUFFER_SIZE], line2[BUFFER_SIZE];
    int line_num = 1;

    printf("📌 Différences entre minishell et bash :\n");
    printf("=======================================\n");

    while (fgets(line1, BUFFER_SIZE, file1) || fgets(line2, BUFFER_SIZE, file2)) {
        if (!line1[0]) strcpy(line1, "[VIDE]\n");
        if (!line2[0]) strcpy(line2, "[VIDE]\n");

        if (strcmp(line1, line2) != 0) {
            printf("🔹 Ligne %d :\n", line_num);
            printf("   minishell: %s", line1);
            printf("   bash     : %s", line2);
        }
        line_num++;
    }

    printf("=======================================\n");

    fclose(file1);
    fclose(file2);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <minishell_path> <test_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *minishell_path = argv[1];
    const char *test_file = argv[2];

    FILE *file = fopen(test_file, "r");
    if (!file) {
        perror("Erreur ouverture fichier de test");
        return EXIT_FAILURE;
    }

    char command[BUFFER_SIZE];
    int test_number = 1;

    while (fgets(command, sizeof(command), file)) {
        command[strcspn(command, "\n")] = 0; // Supprime le saut de ligne

        printf("\n🔹 Test %d: %s\n", test_number, command);

        // Exécute la commande dans minishell et bash
        execute_shell_command(command, minishell_path, TMP_OUTPUT_MINISHELL);
        execute_shell_command(command, "/bin/bash", TMP_OUTPUT_BASH);

        // Compare les sorties
        if (compare_outputs()) {
            printf("✅ Test %d PASS\n", test_number);
        } else {
            printf("❌ Test %d FAIL\n", test_number);
            show_difference();
        }

        test_number++;
    }

    fclose(file);

    // Nettoyage
    remove(TMP_OUTPUT_MINISHELL);
    remove(TMP_OUTPUT_BASH);

    return EXIT_SUCCESS;
}
