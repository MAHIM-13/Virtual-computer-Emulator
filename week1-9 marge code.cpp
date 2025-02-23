#include <iostream>
#include <bitset>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

const int MEMORY_SIZE = 256;
const int REGISTER_COUNT = 8;

vector<string> memory(MEMORY_SIZE, "00000000");
string registers[REGISTER_COUNT] = {"00000000"};
int programCounter = 0;
string instructionRegister = "00000000";

map<string, int> opcodeMap = {
    {"ADD", 1}, {"SUB", 2}, {"LOAD", 3}, {"STORE", 4},
    {"JMP", 5}, {"JZ", 6}, {"JNZ", 7}, {"IN", 8}, {"OUT", 9}, {"HLT", 10}
};

string decimalToBinary(int num) {
    return bitset<8>(num).to_string();
}

int binaryToDecimal(string bin) {
    return stoi(bin, nullptr, 2);
}

void executeInstruction(int opcode, int operand) {
    switch (opcode) {
        case 1: registers[0] = decimalToBinary(binaryToDecimal(registers[0]) + binaryToDecimal(registers[operand])); break; // ADD
        case 2: registers[0] = decimalToBinary(binaryToDecimal(registers[0]) - binaryToDecimal(registers[operand])); break; // SUB
        case 3: registers[0] = memory[operand]; break; // LOAD
        case 4: memory[operand] = registers[0]; break; // STORE
        case 5: programCounter = operand; break; // JMP
        case 6: if (binaryToDecimal(registers[0]) == 0) programCounter = operand; break; // JZ
        case 7: if (binaryToDecimal(registers[0]) != 0) programCounter = operand; break; // JNZ
        case 8: {
            int input;
            cin >> input;
            registers[operand] = decimalToBinary(input);
            break;
        } // IN
    case 9: cout << "Output (Binary): " << registers[operand] << " | Output (Decimal): " << binaryToDecimal(registers[operand]) << endl; break; // OUT
    case 10: exit(0); // HLT
        default: cout << "Invalid instruction!" << endl;
    }
}

void runProgram(vector<string> &program) {
    while (programCounter < program.size()) {
        instructionRegister = program[programCounter++];
        int opcode = binaryToDecimal(instructionRegister.substr(0, 4));
        int operand = binaryToDecimal(instructionRegister.substr(4, 4));
        executeInstruction(opcode, operand);
    }
}

vector<string> assemble(vector<string> assemblyCode) {
    vector<string> machineCode;
    for (string line : assemblyCode) {
        stringstream ss(line);
        string instr;
        int operand;
        ss >> instr >> operand;
        int opcode = opcodeMap[instr];
        string binaryInstr = decimalToBinary(opcode).substr(4, 4) + decimalToBinary(operand).substr(4, 4);
        machineCode.push_back(binaryInstr);
    }
    return machineCode;
}

int main() {
    vector<string> assemblyCode = {"IN 1", "LOAD 1", "ADD 1", "OUT 0", "HLT 0"};
    vector<string> machineCode = assemble(assemblyCode);
    runProgram(machineCode);
    return 0;
}
