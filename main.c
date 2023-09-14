#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <openssl/aes.h>

#define ASCII_MAGIC "asciichar"
#define VERSION_NUMBER "0.4.2"
#define BUFFER_SIZE 16

#define AES128 0x10
#define AES192 0x18
#define AES256 0x20

typedef struct metadata {
    char filename[128];        // Nama file
    size_t filesize;           // Ukuran file dalam byte
    time_t created_timestamp;  // Waktu pembuatan file
    char encryption_algorithm; // Algoritma enkripsi yang digunakan
    char encryption_key[16];   // Kunci enkripsi (disimpan secara aman)
    char description[256];    // Deskripsi file
	char reserved[8];
} metadata_t;

// IV (Initialization Vector, 16 bytes)
static const uint8_t iv[16] = {
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f
};

char encrypted_magic[9] = {146, 165, 150, 157, 158, 153, 159, 153, 171};
char magic[9] = "asciichar";
char default_description[256] = "Encrypted with encryptor " VERSION_NUMBER " with AES128 encription algorithm";

void print_version()
{
	printf("Encryptor version " VERSION_NUMBER " Licensed Under GNU GPL v3\nThe GNU General Public License is a free\ncopyleft license for software and other kinds of works.\n\n");
}

// Function to display help information
void print_help()
{
	printf("help		Display this information.\n");
	printf("clear		Clear screen.\n");
	printf("ls			List information about the FILEs\n");
	printf("cd			Change the current directory.\n");
	printf("encrypt		Encrypt a file.\n");
	printf("decrypt		Decrypt a file.\n");
	printf("getmagic	Get encrypted magic array from ascii.\n");
	printf("getchiper	Get new chiper array from ascii.\n");
}

int get_file_size(FILE *f)
{
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

void update_progress_bar(int current, int total, clock_t start_time)
{
    const int bar_width = 10;  // Lebar progress bar
    float progress = (float)current / total;
    int bar_length = (int)(progress * bar_width);

    clock_t current_time = clock();
    double elapsed_seconds = (double)(current_time - start_time) / CLOCKS_PER_SEC;
    int minutes = (int)(elapsed_seconds / 60);
    int seconds = (int)(elapsed_seconds) % 60;

    printf("\e[?25l\r%02d:%02d [", (int)progress * 100, minutes);
    for (int i = 0; i < bar_width; i++)
    {
        if (i < bar_length)
            printf("=");
        else
            printf(" ");
    }
    printf("] %3.2f%%\n", progress * 100);
	printf("%3.2f MB/%3.2f MB ", current / (1024.0 * 1024.0), total / (1024.0 * 1024.0));
	printf("\033[1A");
	if(current == total)
		printf("\n\n\e[?25h");
    fflush(stdout);
}

int write_encrypted_bytes(FILE *input_file, FILE *output_file, AES_KEY wctx)
{
	unsigned char buffer[BUFFER_SIZE];
	unsigned char encrypted_buffer[BUFFER_SIZE];
	size_t bytes_to_read = fread(buffer, 1, sizeof(buffer), input_file);
	AES_encrypt(buffer, encrypted_buffer, &wctx);
	fwrite(encrypted_buffer, 1, BUFFER_SIZE, output_file);
	return bytes_to_read;
}

int write_decrypted_bytes(FILE *input_file, FILE *output_file, int size, AES_KEY wctx)
{
	unsigned char buffer[BUFFER_SIZE];
	unsigned char decrypted_buffer[BUFFER_SIZE];
	size_t bytes_to_read = fread(buffer, 1, BUFFER_SIZE, input_file);
	AES_decrypt(buffer, decrypted_buffer, &wctx);
	fwrite(decrypted_buffer, 1, size, output_file);
	return bytes_to_read;
}

void encrypt_metadata(unsigned char *dest, unsigned char *src, size_t size, AES_KEY wctx)
{
	long i = 0;
    while (i < size)
	{
		AES_encrypt(src + i, dest + i, &wctx);
		i += 16;
	}
}

void decrypt_metadata(unsigned char *dest, unsigned char *src, size_t size, AES_KEY wctx)
{
	long i = 0;
    while (i < size)
	{
		AES_decrypt(src + i, dest + i, &wctx);
		i += 16;
	}
}

/*
 * Write metadata information to encrypted file
 */
void write_metadata (FILE *input_file, FILE *output_file, char *filename, char *password, AES_KEY wctx)
{
	uint8_t encrypted_metadata[1024];

	time_t rawtime;
	time(&rawtime);

	// Allocate memory for metadata struct
	metadata_t *m = malloc(sizeof(metadata_t));

	strcpy(m->filename, filename);					// Copy input file name to metadata
	strcpy(m->encryption_key, password);			// Copy encription key to metadata
	strcpy(m->description, default_description);	// Write decription to metadata

	m->created_timestamp = rawtime;					// Write timestamp to metadata
	m->filesize = get_file_size(input_file);		// Write decrypted file size to metadata
	m->encryption_algorithm = AES128;				// Write encryption algorithm to metadata

	encrypt_metadata(encrypted_metadata, (uint8_t*)m, sizeof(metadata_t), wctx);
	fwrite((char*)encrypted_metadata, 1, sizeof(metadata_t), output_file);		// Write metadata to file

	free(m);
}

/*
 * Read encrypted metadata from encrypted file
 * and decrypt metadata
 */
metadata_t * read_metadata (FILE *f, AES_KEY wctx)
{
	uint8_t tmp[1024];

	// Allocate metadata struct
	metadata_t *m = (metadata_t*) malloc(sizeof(metadata_t));

	// Read metadata from encrypted file
	fread(tmp, 1, sizeof(metadata_t), f);

	// Decrypt metadata
	decrypt_metadata((uint8_t*)m, tmp, sizeof(metadata_t), wctx);

	return m;
}

void print_metadata (metadata_t *m)
{
	char timestamp[80];
	struct tm *local_time = localtime(&m->created_timestamp);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S %d-%m-%Y", local_time);
	printf("name		: %s\n", m->filename);
	printf("size		: %d\n", (int)m->filesize);
	printf("created		: %s\n", timestamp);
	printf("Encryption	: AES%d\n", m->encryption_algorithm * 8);
	printf("key		: ********\n");
	printf("description	: %s\n", m->description);
}

void get_password (char *buf)
{
	while(1)
	{
		printf("password: ");
		memset(buf, 0, 128);
		fgets(buf, 128, stdin);
		buf[strlen(buf) - 1] = 0;
		if (strlen(buf) > 16)
			printf("password must not exceed 16 characters.\n");
		else if (strlen(buf) < 8)
			printf("password must be at least 8 characters.\n");
		else
			break;
	}
}

int verify_password	(char *password, FILE *input_file)
{
	AES_KEY wctx;
    AES_set_decrypt_key(password, 128, &wctx);

	fseek(input_file, 0, SEEK_SET);

	metadata_t *m = read_metadata(input_file, wctx);

	fseek(input_file, 0, SEEK_SET);

	if (!strcmp(m->encryption_key, password))
	{
		free(m);
		return 0;
	}
	else
	{
		free(m);
		return -1;
	}
}

void input_password (char *password, FILE *input_file)
{
	while(1)
	{
		printf("password: ");
		memset(password, 0, 128);
		fgets(password, 128, stdin);
		password[strlen(password) - 1] = 0;
		if(verify_password(password, input_file) == -1)
			printf("Error: wrong password, try again.\n");
		else
			break;
	}
}

void encrypt_data(char *filename, FILE *input_file, FILE *output_file)
{
	char password[128];
	get_password(password);

	clock_t start_time = clock();

	AES_KEY wctx;
    AES_set_encrypt_key(password, 128, &wctx);

    long file_size = get_file_size(input_file);

	write_metadata(input_file, output_file, filename, password, wctx);

    long bytes_processed = 0;
    while (bytes_processed < file_size)
	{
        bytes_processed += write_encrypted_bytes(input_file, output_file, wctx);
		if (bytes_processed % 4194304 == 0 || bytes_processed == file_size)
        {
            update_progress_bar(bytes_processed, file_size, start_time);
        }
	}

	clock_t current_time = clock();
    double elapsed_seconds = (double)(current_time - start_time) / CLOCKS_PER_SEC;
	printf("Encrypted in %8.2f seconds\n", elapsed_seconds);
}

void decrypt_data(FILE *input_file, FILE *output_file)
{
	char password[128];
	input_password(password, input_file);

	clock_t start_time = clock();

	AES_KEY wctx;
    AES_set_decrypt_key(password, 128, &wctx);

	metadata_t *m = read_metadata(input_file, wctx);
    long file_size = m->filesize;

    long bytes_processed = 0;
	int size = 16;
    while (bytes_processed < file_size)
	{
		if((file_size - bytes_processed) < 16)
			size = file_size - bytes_processed;
        bytes_processed += write_decrypted_bytes(input_file, output_file, size, wctx);
		if (bytes_processed % 4194304 == 0 || bytes_processed == file_size)
        {
            update_progress_bar(bytes_processed, file_size, start_time);
        }
	}
	clock_t current_time = clock();
    double elapsed_seconds = (double)(current_time - start_time) / CLOCKS_PER_SEC;
	printf("Decrypted in %8.2f seconds\n", elapsed_seconds);
	free(m);
}

void print_metadata_information(char *name)
{
	FILE *f = fopen(name, "rb");
	if (!f)
	{
		printf("encryptor: file %s not found.\n", name);
		return;
	}

	char password[128];
	memset(password, 0, 128);
	input_password(password, f);
	AES_KEY wctx;
    AES_set_decrypt_key(password, 128, &wctx);

	metadata_t *m = read_metadata(f, wctx);
	print_metadata(m);
	free(m);
	fclose(f);
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

	char new_filename[128];
	char old_filename[128];
	strcpy(old_filename, input_name);
	// ... Generate new_filename ...
	sprintf(new_filename, "%s.enc", strtok(input_name, "."));
	
	FILE *output_file = fopen(new_filename, "wb");
	if (!output_file)
	{
		printf("encryptor: failed to create file %s\n", new_filename);
		return;
	}
	
	encrypt_data(input_name, input_file, output_file);
	fclose(output_file);
	fclose(input_file);

	printf("File %s successfully encrypted.\n", old_filename);
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

	char old_filename[128];
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
		print_version();
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
	else if (!strcmp(argv[0], "metadata"))
	{
		if (!argv[1])
		{
			printf("error: need an argument\n");
			return;
		}
		print_metadata_information(argv[1]);
	}
	else
	{
		system(buf);
	}
}

// Function to start the shell
void shell()
{
	print_version();

	char input[128];
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

// Function to decrypt ASCII encrypted_magic
void decrypt_magic(char *buf, char *password)
{
	for (int i = 0; i < 9; i++)
	{
		buf[i] = encrypted_magic[i] - password[i];
	}
	buf[9] = 0;
}

// Function to verify encrypted_magic and start shell
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
	printf("Simple Calculator v" VERSION_NUMBER "\n");

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
