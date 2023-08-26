#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ASCII_MAGIC "asciichar"

// Magic numbers and key for encryption/decryption
// Password linuxmint
char magic[9] = {205, 220, 209, 222, 225, 208, 209, 207, 230};
char chiper_key[8] = {63, 30, 21, 43, 20, 56, 73, 45};

// Function to display help information
void print_help()
{
    printf("help            Display this information.\n");
    printf("clear           Clear screen.\n");
    printf("ls              List information about the FILEs\n");
    printf("cd              Change the current directory.\n");
    printf("encrypt         Encrypt a file.\n");
    printf("decrypt         Decrypt a file.\n");
}

// Function to perform encryption
void perform_encryption(FILE *input_file, FILE *output_file)
{
    char byte;
    int i = 0;
	fseek(input_file, 0, SEEK_END);
	int size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);
    for(int j = 0; j < size; j++)
    {
		byte = fgetc(input_file);
        char encrypted_byte = byte + chiper_key[i];
        fwrite(&encrypted_byte, 1, 1, output_file);
        i = (i + 1) % 8;
    }
}

// Function to perform decryption
void perform_decryption(FILE *input_file, FILE *output_file)
{
    char byte;
    int i = 0;
    fseek(input_file, 0, SEEK_END);
	int size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);
    for(int j = 0; j < size; j++)
    {
		byte = fgetc(input_file);
        char decrypted_byte = byte - chiper_key[i];
		//printf("%x with chiper key %x\n", decrypted_byte, chiper_key[i]);
        fwrite(&decrypted_byte, 1, 1, output_file);
        i = (i + 1) % 8;
    }
}

// Function to parse user commands
void parse_command(int argc, char *argv[])
{
	if (!strcmp(argv[0], "help"))
    {
        print_help();
    }
	else if (!strcmp(argv[0], "clear"))
    {
        system("clear");
    }
    else if (!strcmp(argv[0], "ls"))
    {
        system("ls");
    }
    else if (!strcmp(argv[0], "cd"))
    {
		if (!argv[1])
            printf("error: need an argument");
        chdir(argv[1]);
    }
    else if (!strcmp(argv[0], "encrypt"))
    {
        if (!argv[1])
            printf("error: need an argument");
        else
        {
            FILE *input_file = fopen(argv[1], "rb");
            if (input_file)
            {
                char new_filename[256];
                // ... Generate new_filename ...
				sprintf(new_filename, "%s.enc", strtok(argv[1], "."));
                FILE *output_file = fopen(new_filename, "wb");
                if (output_file)
                {
                    perform_encryption(input_file, output_file);
                    fclose(output_file);
                }
                else
                {
                    printf("encryptor: failed to create file %s\n", new_filename);
                }
                fclose(input_file);
            }
            else
            {
                printf("encryptor: file %s not found.\n", argv[1]);
            }
        }
    }
	else if (!strcmp(argv[0], "decrypt"))
    {
        if (!argv[1] || !argv[2])
            printf("error: need an argument");
        else
        {
            FILE *input_file = fopen(argv[1], "rb");
            if (input_file)
            {
                FILE *output_file = fopen(argv[2], "wb");
                if (output_file)
                {
                    perform_decryption(input_file, output_file);
                    fclose(output_file);
                }
                else
                {
                    printf("decryptor: failed to create file %s\n", argv[2]);
                }
                fclose(input_file);
            }
            else
            {
                printf("decryptor: file %s not found.\n", argv[1]);
            }
        }
    }
	else
	{
		printf("error: command not found.\n");
	}
    // ... Other commands ...
}

// Function to start the shell
void shell()
{
    printf("Encryptor v1.1 Licensed Under GNU GPL v3\n");

    char input[256];
    char cwd[1024];
    char *argv[1024];
    while (1)
    {
        int argc = 0;
        getcwd(cwd, sizeof(cwd));
        printf("%s > ", cwd);
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = 0;

        argv[argc] = strtok(input, " ");
        while (argv[argc] != NULL)
        {
            argv[++argc] = strtok(NULL, " ");
        }

        if (argc > 0)
            parse_command(argc, argv);
    }
}

// Function to decrypt ASCII magic
void decrypt_magic(char *buf, char *password)
{
    for (int i = 0; i < 9; i++)
    {
        buf[i] = magic[i] - password[i];
    }
    buf[9] = 0;
}

// Function to verify magic and start shell
void verify_magic(char *password)
{
    if (!password)
        return;

    char buf[9];
    decrypt_magic(buf, password);
    if (!strcmp(buf, ASCII_MAGIC))
    {
        shell();
        exit(0);
    }
    else
    {
        return;
    }
}

// Function to simulate a simple calculator
void calculator()
{
    printf("Simple Calculator v1.1\n");

    int a, b;
    printf("Input first number: ");
    scanf("%d", &a);

    printf("Input second number: ");
    scanf("%d", &b);

	printf("Result: %d\n", a + b);

    // ... Perform calculations ...
}

int main(int argc, char *argv[])
{
    system("clear");
    verify_magic(argv[1]);
    calculator();
    return 0;
}
