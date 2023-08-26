# Encryptor Application in C

![Screenshot](screenshot.png)

This application is a simple program designed to encrypt and decrypt files using a specific algorithm. The program is written in the C programming language and includes several basic features.

## Features

- Encrypt and decrypt files using a specific key algorithm.
- Execute basic commands like `ls`, `cd`, and `clear` through a shell.
- If the password is incorrect or not provided, it opens a simple calculator.
- If the password is correct, it opens the encryptor shell.
- The simple calculator mode is used to conceal the encryptor shell.
- Includes a basic calculator mode.

## Usage

Make sure you have a C compiler like GCC to compile this program.

1. Compile the program:

    ```bash
    gcc encryptor.c -o encryptor
    ```

2. Encrypt a file:

    ```bash
    ./encryptor encrypt file_name
    ```

   This will encrypt `file_name` and save it with the `.enc` extension.

3. Decrypt a file:

    ```bash
    ./encryptor decrypt encrypted_file decrypted_result
    ```

   This will decrypt `encrypted_file` and save the result as `decrypted_result`.

4. Run the shell:

    ```bash
    ./encryptor password
    ```

   You will enter the shell mode by providing the correct password. Inside the shell, you can execute commands like `ls`, `cd`, `clear`, and access the simple calculator feature.

## Notes

- **Important**: This application is a simple example and is not intended for real security. Do not use this code for critical or sensitive purposes.

- The password is used to enter the shell mode, but there is no strong security mechanism in this code. This is just a simple example.

- Make sure you understand the code well before using or further developing it.

## License

This project is licensed under the GNU GPL v3. See the [LICENSE](LICENSE) file for more information.

---

Created by Mr. Gart
