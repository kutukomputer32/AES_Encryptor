#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ASCII_MAGIC "asciichar"

// Magic numbers and key for encryption/decryption
//char magic[9] = {201, 220, 215, 213, 206, 213, 155, 145, 166};
char magic[9] = {146, 165, 150, 157, 158, 153, 159, 153, 171};
char chiper_key[8] = {63, 30, 21, 43, 20, 56, 73, 45};
int current_iteration = 0;
const int iterations_per_update = 1;  // Atur sesuai kebutuhan Anda

// Function to display help information
void print_help()
{
	printf("help			Display this information.\n");
	printf("clear		   Clear screen.\n");
	printf("ls			  List information about the FILEs\n");
	printf("cd			  Change the current directory.\n");
	printf("encrypt		 Encrypt a file.\n");
	printf("decrypt		 Decrypt a file.\n");
}

int get_file_size(FILE *f)
{
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

void update_progress_bar(int current, int total)
{
	const int bar_width = 20;  // Lebar progress bar
    float progress = (float)current / total;
    int bar_length = (int)(progress * bar_width);

    printf("\r%3.2f MB/%3.2f MB [", current / (1024.0 * 1024.0), total / (1024.0 * 1024.0));
    for (int i = 0; i < bar_width; i++)
    {
        if (i < bar_length)
            printf("=");
        else
            printf(" ");
    }
    printf("] %3.2f%%", progress * 100);
    fflush(stdout);
}


// Function to perform encryption
void encrypt_data(FILE *input_file, FILE *output_file)
{
	int i = 0;
	int size = get_file_size(input_file);
	for(int j = 0; j < size; j++)
	{
		char encrypted_byte = fgetc(input_file) + chiper_key[i];
		fwrite(&encrypted_byte, 1, 1, output_file);
		i = (i + 1) % 8;

		// Update progress bar
        if ((j + 1) % 100000 == 0 || j == size - 1)
        {
            update_progress_bar(j + 1, size);
        }
	}
}

// Function to perform decryption
void decrypt_data(FILE *input_file, FILE *output_file)
{
	int i = 0;
	int size = get_file_size(input_file);
	for(int j = 0; j < size; j++)
	{
		char decrypted_byte = fgetc(input_file) - chiper_key[i];
		fwrite(&decrypted_byte, 1, 1, output_file);
		i = (i + 1) % 8;

		// Update progress bar
        if ((j + 1) % 100000 == 0 || j == size - 1)
        {
            update_progress_bar(j + 1, size);
        }
	}
}

void encrypt(char *input_name)
{
	//printf("encrypting %s\n", input_name);
	FILE *input_file = fopen(input_name, "rb");
	if (!input_file)
	{
		printf("encryptor: file %s not found.\n", input_name);
		return;
	}

	char new_filename[256];
	char old_filename[256];
	strcpy(old_filename, input_name);
	// ... Generate new_filename ...
	sprintf(new_filename, "%s.enc", strtok(input_name, "."));
	
	FILE *output_file = fopen(new_filename, "wb");
	if (!output_file)
	{
		printf("encryptor: failed to create file %s\n", new_filename);
		return;
	}
	
	encrypt_data(input_file, output_file);
	fclose(output_file);
	fclose(input_file);

	printf("\nFile %s successfully encrypted.\n", old_filename);
}

void decrypt(char *input_name, char *output_name)
{
	//printf("decrypting %s\n", input_name);
	FILE *input_file = fopen(input_name, "rb");
	if (!input_file)
	{
		printf("encryptor: file %s not found.\n", input_name);
		return;
	}

	char old_filename[256];
	strcpy(old_filename, input_name);

	FILE *output_file = fopen(output_name, "wb");
	if (!output_file)
	{
		printf("decryptor: failed to create file %s\n", output_name);
		return;
	}
			
	decrypt_data(input_file, output_file);
	fclose(output_file);
	fclose(input_file);

	printf("\nFile %s successfully decrypted.\n", old_filename);
}

// Function to parse user commands
void parse_command(int argc, char *argv[], char *buf)
{
	if (!strcmp(argv[0], "help"))
	{
		print_help();
	}
	else if (!strcmp(argv[0], "version"))
	{
		printf("Encryptor version 0.4.1 Licensed Under GNU GPL v3\nThe GNU General Public License is a free\ncopyleft license for software and other kinds of works.\n");
	}
	else if (!strcmp(argv[0], "cd"))
	{
		if (!argv[1])
			printf("error: need an argument\n");
		chdir(argv[1]);
	}
	else if (!strcmp(argv[0], "exit"))
	{
		system("clear");
		exit(0);
	}
	else if (!strcmp(argv[0], "encrypt"))
	{
		if (!argv[1])
		{
			printf("error: need an argument\n");
			return;
		}
		encrypt(argv[1]);
	}
	else if (!strcmp(argv[0], "decrypt"))
	{
		if (!argv[1] || !argv[2])
		{
			printf("error: need an argument\n");
			return;
		}
		decrypt(argv[1], argv[2]);
	}
	else
	{
		system(buf);
	}
}

// Function to start the shell
void shell()
{
	printf("Encryptor version 0.4.1 Licensed Under GNU GPL v3\nThe GNU General Public License is a free\ncopyleft license for software and other kinds of works.\n");

	char input[256];
	char tmp[1024];
	char cwd[1024];
	char *argv[1024];
	while (1)
	{
		int argc = 0;
		getcwd(cwd, sizeof(cwd));
		printf("\033[1;94mEncryptor:\033[1;37m%s\033[1;31m $\033[0m ", cwd);
		fgets(input, sizeof(input), stdin);
		input[strlen(input) - 1] = 0;
		strcpy(tmp, input);

		argv[argc] = strtok(input, " ");
		while (argv[argc] != NULL)
		{
			argv[++argc] = strtok(NULL, " ");
		}

		if (argc > 0)
			parse_command(argc, argv, tmp);
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
	printf("Simple Calculator v0.4.1\n");

	int a, b;
	printf("Input first number: ");
	scanf("%d", &a);

	printf("Input second number: ");
	scanf("%d", &b);

	printf("Result: %d\n", a + b);

}

int main(int argc, char *argv[])
{
	system("clear");
	verify_magic(argv[1]);
	calculator();
	return 0;
}
