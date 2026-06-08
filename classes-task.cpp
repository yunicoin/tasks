#include <iostream>
#include <string>

class MorseCode {
private:
    char* data;
    size_t size;
    const char* morse_dict[26]; 
    void initMorseDict() {
        morse_dict[0] = ".-";   // A
        morse_dict[1] = "-..."; // B
        morse_dict[2] = "-.-."; // C
        morse_dict[3] = "-..";  // D
        morse_dict[4] = ".";    // E
        morse_dict[5] = "..-."; // F
        morse_dict[6] = "--.";  // G
        morse_dict[7] = "...."; // H
        morse_dict[8] = "..";   // I
        morse_dict[9] = ".---"; // J
        morse_dict[10] = "-.-";  // K
        morse_dict[11] = ".-.."; // L
        morse_dict[12] = "--";   // M
        morse_dict[13] = "-.";    // N
        morse_dict[14] = "---";  // O
        morse_dict[15] = ".--."; // P
        morse_dict[16] = "--.-"; // Q
        morse_dict[17] = ".-.";  // R
        morse_dict[18] = "...";  // S
        morse_dict[19] = "-";    // T
        morse_dict[20] = "..-";  // U
        morse_dict[21] = "...-"; // V
        morse_dict[22] = ".--";  // W
        morse_dict[23] = "-..-"; // X
        morse_dict[24] = "-.--"; // Y
        morse_dict[25] = "--.."; // Z
    }

public:
    MorseCode() : data(nullptr), size(0) {
        initMorseDict();
    }

    MorseCode(const MorseCode& other) : data(nullptr), size(0) {
        initMorseDict();
        if (other.data) {
            size = other.size;
            data = new char[size + 1];
            for (size_t i = 0; i <= size; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    ~MorseCode() {
        delete[] data;
    }
    MorseCode& operator=(const MorseCode& other) {
        if (this != &other) {
            delete[] data;
            size = 0;
            data = nullptr;

            if (other.data) {
                size = other.size;
                data = new char[size + 1];
                for (size_t i = 0; i <= size; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    MorseCode operator+(const MorseCode& other) const {
        MorseCode result = *this;
        result += other;
        return result;
    }

    MorseCode& operator+=(const MorseCode& other) {
        size_t otherSize = 0;
        if (other.data) {
            while (other.data[otherSize] != '\0') {
                ++otherSize;
            }
        }

        size_t newSize = size + otherSize;
        char* newData = new char[newSize + 1];

        for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        for (size_t i = 0; i < otherSize; ++i) {
            newData[size + i] = other.data[i];
        }
        newData[newSize] = '\0';

        delete[] data;
        data = newData;
        size = newSize;
        return *this;
    }

    // Операции сравнения
    bool operator==(const MorseCode& other) const {
        if (size != other.size) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const MorseCode& other) const {
        return !(*this == other);
    }

    std::string encode(const std::string& text) {
        std::string morse_code;
        for (char c : text) {
            if (c >= 'A' && c <= 'Z') {
                morse_code += std::string(morse_dict[c - 'A']) + ' ';
            }
        }

        delete[] data;
        size = morse_code.size();
        data = new char[size + 1];
        for (size_t i = 0; i <= size; ++i) {
            data[i] = morse_code[i];
        }

        return morse_code;
    }

    std::string decode() const {
        if (!data || size == 0) return "";

        std::string decoded;
        std::string currentCode;

        for (size_t i = 0; i <= size; ++i) {
            char c = data[i];
            if (c == ' ' || c == '\0' || c == '/') {
                if (!currentCode.empty()) {
                    for (int j = 0; j < 26; ++j) {
                        // Корректное сравнение строк
                        if (std::string(morse_dict[j]) == currentCode) {
                            decoded += static_cast<char>('A' + j);
                            break;
                        }
                    }
                    currentCode.clear();
                }
            } else {
                currentCode += c;
            }
        }
        return decoded;
    }

    friend std::ostream& operator<<(std::ostream& os, const MorseCode& mc) {
        if (mc.data) os << mc.data;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, MorseCode& mc) {
        std::string input;
        is >> input;
        mc.encode(input);
        return is;
    }
}; 
