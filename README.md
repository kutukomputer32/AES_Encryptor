# Encryptor Application in C

![Screenshot](screenshot.png)

**A simple application for file encryption and decryption written in C.**

## Features 🚀

- Encrypt and decrypt files using a custom key algorithm.
- Execute basic commands like `ls`, `cd`, and `clear` through a shell.
- Utilizes a simple calculator interface to access the shell.
- A basic calculator mode for additional functionality.

## Usage 🛠️

Ensure you have a C compiler like GCC to compile this program.

1. **Compile the program**:

    ```bash
    gcc encryptor.c -o encryptor
    ```

2. **Encrypt a file**:

    ```bash
    ./encryptor encrypt file_name
    ```

    This will encrypt `file_name` and save it with the `.enc` extension.

3. **Decrypt a file**:

    ```bash
    ./encryptor decrypt encrypted_file decrypted_result
    ```

    This will decrypt `encrypted_file` and save the result as `decrypted_result`.

4. **Run the shell**:

    ```bash
    ./encryptor password
    ```

    Enter the shell mode by providing the correct password. Inside the shell, you can execute commands like `ls`, `cd`, `clear`, and access the simple calculator feature.

## Notes 📝

- **Important**: This application is a simple example and is not intended for real security. Do not use this code for critical or sensitive purposes.

- The password is used to enter the shell mode, but there is no strong security mechanism in this code. This is just a simple example.

- Make sure you understand the code well before using or further developing it.

## License 📜

This project is licensed under the GNU GPL v3. See the [LICENSE](LICENSE) file for more information.

---

Created by Mr. Gart
