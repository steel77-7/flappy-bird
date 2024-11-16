#include <iostream>
#include <fstream>
#include <vector>

int main() {
    // File name or path
    const std::string filePath = "hello.txt"; // Replace with your file path

    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filePath << std::endl;
        return 1; // Exit with an error code
    }

    std::cout << "File opened successfully!" << std::endl;

    // Read the entire file into a vector
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Print the file contents as raw characters (not readable for binary files like images)
    for (char byte : buffer) {
        std::cout << byte; // This will print raw bytes (not readable for binary files)
    }

    file.close();
    return 0; // Exit successfully
}
