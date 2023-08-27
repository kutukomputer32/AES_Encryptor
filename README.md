# Encryptor Application in C

## Screenshot 1
![Screenshot](screenshot.png)

## Screenshot 2
![Screenshot](screenshot2.png)

This application is a simple program designed to encrypt and decrypt files using a specific algorithm. The program is written in the C programming language and includes several basic features.

## Features

- Encrypt and decrypt files using a specific key algorithm.
- Execute basic commands like `ls`, `cd`, and `clear` through a shell.
- If the password is incorrect, it opens a simple calculator.
- If the password is correct, it opens the encryptor shell.
- The simple calculator mode is used to hide the encryptor shell.
- Includes a basic calculator mode.

## Usage

Make sure you have a C compiler like GCC to compile this program.

1. Compile the program:

    ```bash
    gcc encryptor.c -o encryptor
    ```
2. Run the shell:

    ```bash
    ./encryptor password
    ```

3. Encrypt a file:

    ```bash
    encrypt file_name
    ```

   This will encrypt `file_name` and save it with the `.enc` extension.

4. Decrypt a file:

    ```bash
    decrypt encrypted_file decrypted_result
    ```

   This will decrypt `encrypted_file` and save the result as `decrypted_result`.

   You will enter the shell mode by input the correct password. Inside the shell, you can execute commands like `ls`, `cd`, `clear`, and access the simple calculator feature.

## Notes

- Make sure you understand the code well before using or further developing it.

- Password to enter the shell "123456789"

## License

This project is licensed under the GNU GPL v3. See the [LICENSE](LICENSE) file for more information.

---

Created by Kutu Komputer
