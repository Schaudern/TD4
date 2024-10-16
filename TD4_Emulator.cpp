#include <iostream>
#include <bitset>
#include <istream>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <array>
#include <fstream>

class TD4
{
private:
    uint8_t register_a;
    uint8_t register_b;
    bool flag_c;
    uint8_t program_counter;
    uint8_t input_port;
    uint8_t output_port;
    std::array<uint8_t, 32> instructions;
    int frequency;
public:
    TD4()
    {
        register_a = 0;
        register_b = 0;
        flag_c = 0;
        program_counter = 0;
        input_port = 0;
        output_port = 0;
        instructions.fill(0);
        frequency = -1;
    }

    void execute(int& number)
    {
        switch (instructions[number])
        {
        case 0b1011: //OUT Im
            flag_c = 0;
            output_port = instructions[number + 1];
            number += 2;
            break;
        case 0b1001: //OUT B
            flag_c = 0;
            output_port = register_b;
            number += 2;
            break;
        case 0b1000:
            flag_c = 0;
            output_port = register_b;
            number += 2;
            break;
        case 0b1010: //OUT A
            flag_c = 0;
            output_port = register_a;
            number += 2;
            break;
        case 0b0011: //MOV A,Im
            flag_c = 0;
            register_a = instructions[number + 1];
            number += 2;
            break;
        case 0b0111: //MOV B,Im
            flag_c = 0;
            register_b = instructions[number + 1];
            number += 2;
            break;
        case 0b0001: //MOV A,B
            flag_c = 0;
            register_a = register_b + instructions[number + 1];
            if (register_a > 15)
            {
                register_a -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b0100: //MOV B,A
            flag_c = 0;
            register_b = register_a + instructions[number + 1];
            if (register_b > 15)
            {
                register_b -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b0000: //ADD A,Im
            flag_c = 0;
            register_a = register_a + instructions[number + 1];
            if (register_a > 15)
            {
                register_a -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b0101: //ADD B,Im
            flag_c = 0;
            register_b = register_b + instructions[number + 1];
            if (register_b > 15)
            {
                register_b -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b1100: //ADD A,B
            flag_c = 0;
            register_a += register_b;
            if (register_a > 15)
            {
                register_a -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b1101: //ADD B,A
            flag_c = 0;
            register_b += register_a;
            if (register_b > 15)
            {
                register_b -= 16;
                flag_c = 1;
            }
            number += 2;
            break;
        case 0b1111: //JMP Im
            flag_c = 0;
            program_counter = instructions[number + 1] - 1;
            number = instructions[number + 1] * 2;
            break;
        case 0b1110: //JNC Im
            if (flag_c == 0)
            {
                program_counter = instructions[number + 1] - 1;
                number = instructions[number + 1] * 2;
            }
            flag_c = 0;
            break;
        case 0b0010: //IN A
            std::cout << '\n' << "Enter Register A: ";
            std::cin >> std::hex >> register_a;
            number += 2;
            break;
        case 0b0110: //IN B
            std::cout << '\n' << "Enter Register B: ";
            std::cin >> std::hex >> register_b;
            number += 2;
            break;
        default:
            break;
        }
        program_counter++;
        if (program_counter > 15)
        {
            program_counter = 0;
            number = 0;
        }
    }

    void start()
    {
        std::cout << '\n' << "Enter a path to file: ";
        std::string str;
        std::cin >> str;
        std::ifstream file(str, std::ios::binary);
        if (!file)
        {
            std::cout << '\n' << "No such file -_-";
            return;
        }
        unsigned char arr[16];
        file.read(reinterpret_cast<char*>(arr), 16);
        int i = 0;
        for (int j = 0; j < 32; j += 2)
        {
            instructions[j] = arr[i] / 16;
            instructions[j + 1] = arr[i] % 16;
            i++;
        }

        std::cout << '\n' << "Enter a clock frequency (enter 0 if you need a step-by-step execution): ";
        std::cin >> frequency;
        if (frequency != 0 and frequency != 1 and frequency != 10)
            frequency = 0;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int number = 0;
        while (true)
        {
            system("clear");
            std::cout << *this;
            if (frequency == 10)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            else if (frequency == 1)
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            else
            {
                char c;
                c = getchar();
            }
            execute(number);
        }
    }


    friend std::ostream& operator<<(std::ostream& stream, TD4 t);
};

std::ostream& operator<<(std::ostream& stream, TD4 t)
{
    stream << "Register A: " << std::bitset<4>(t.register_a) << '\n';
    stream << "Reguster B: " << std::bitset<4>(t.register_b) << '\n';
    stream << "Flag C: " << t.flag_c << '\n';
    stream << "Program Counter: " << std::bitset<4>(t.program_counter) << '\n';
    stream << "Input port: " << std::bitset<4>(t.input_port) << '\n';
    stream << "Output port: " << std::bitset<4>(t.output_port) << '\n';
    if (t.frequency == 0)
    {
        std::cout << '\n' << "Press enter to continue";
    }
    return stream;
}

int main()
{
    TD4 td4;
    system("clear");
    std::cout << td4;
    while (true)
        td4.start();
    return 0;
}


