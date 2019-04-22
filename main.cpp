#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdint.h>
#include <cstring>
#include <unistd.h>
#include "main.h"
using namespace std;

enum {
    add = 0,
    sub,
    xorr,
    orr,
    sll,
    srl,
    sra,
    slt,
    sltu,

    addi,
    subi,
    xori,
    ori,
    slli,
    srli,
    srai,
    slti,
    sltiu,
    lw,
    lh,
    lhu,
    lq,
    lqu,
    lb,
    lbu,
    sw,
    sh,
    sq,
    sb,

    beq,
    bne,

    j,

    jr,

    lui,
    halt
};


void copyrem(char* in, char* out) {
    for (int i = 0; i < 81; i++) {
        if (in[i] == '\n') {
            out[i] = '\0';
            return;
        }

        out[i] = in[i];

        if (in[i] == '\0') return;
    }
}

int main() {
    char temps[81];

    ifstream fromFile;
    fromFile.open("file.txt");

    vector <uint64_t> tokens;

    if (!fromFile) {
        cout << "Cannot open file";
        exit(1);
    }

    cout << "This is the text in the file:\n";

    string line;

    while (fromFile.getline(temps, 80)) {
        char* token = strtok(temps, " ");

        while (token) {
            cout << token << " ]\n";

            if (strcmp(token, "add") == 0) {
                tokens.push_back(add);
            } else if (strcmp(token, "sub") == 0) {
                tokens.push_back(sub);
            } else if (strcmp(token, "xorr") == 0) {
                tokens.push_back(xorr);
            } else if (strcmp(token, "orr") == 0) {
                tokens.push_back(orr);
            } else if (strcmp(token, "sll") == 0) {
                tokens.push_back(sll);
            } else if (strcmp(token, "srl") == 0) {
                tokens.push_back(srl);
            } else if (strcmp(token, "sra") == 0) {
                tokens.push_back(sra);
            } else if (strcmp(token, "slt") == 0) {
                tokens.push_back(slt);
            } else if (strcmp(token, "sltu") == 0) {
                tokens.push_back(sltu);
            } else if (strcmp(token, "addi") == 0) {
                tokens.push_back(addi);
            } else if (strcmp(token, "subi") == 0) {
                tokens.push_back(subi);
            } else if (strcmp(token, "xori") == 0) {
                tokens.push_back(xori);
            } else if (strcmp(token, "ori") == 0) {
                tokens.push_back(ori);
            } else if (strcmp(token, "slli") == 0) {
                tokens.push_back(slli);
            } else if (strcmp(token, "srli") == 0) {
                tokens.push_back(srli);
            } else if (strcmp(token, "srai") == 0) {
                tokens.push_back(srai);
            } else if (strcmp(token, "slti") == 0) {
                tokens.push_back(slti);
            } else if (strcmp(token, "sltiu") == 0) {
                tokens.push_back(sltiu);
            } else if (strcmp(token, "j") == 0) {
                tokens.push_back(j);
            } else if (strcmp(token, "jr") == 0) {
                tokens.push_back(jr);
            } else if (strcmp(token, "beq") == 0) {
                tokens.push_back(beq);
            } else if (strcmp(token, "bne") == 0) {
                tokens.push_back(bne);
            } else if (strcmp(token, "lw") == 0) {
                tokens.push_back(lw);
            } else if (strcmp(token, "lh") == 0) {
                tokens.push_back(lh);
            } else if (strcmp(token, "lhu") == 0) {
                tokens.push_back(lhu);
            } else if (strcmp(token, "lq") == 0) {
                tokens.push_back(lq);
            } else if (strcmp(token, "lqu") == 0) {
                tokens.push_back(lqu);
            } else if (strcmp(token, "lb") == 0) {
                tokens.push_back(lb);
            } else if (strcmp(token, "lbu") == 0) {
                tokens.push_back(lbu);
            } else if (strcmp(token, "sw") == 0) {
                tokens.push_back(sw);
            } else if (strcmp(token, "sh") == 0) {
                tokens.push_back(sh);
            } else if (strcmp(token, "sq") == 0) {
                tokens.push_back(sq);
            } else if (strcmp(token, "sb") == 0) {
                tokens.push_back(sb);
            } else if (strcmp(token, "lui") == 0) {
                tokens.push_back(lui);
            } else if (strcmp(token, "halt") == 0) {
                tokens.push_back(halt);
            } else {
                stringstream temp(token);
                int64_t x = 0;
                temp >> x;
                tokens.push_back(x);
            }

            token = strtok(NULL, " ");
        }
    }

    vector <uint8_t> output;

    for (int i = 0; i < tokens.size(); i) {
        auto name = tokens[i];

        output.push_back(name);

        if (name < addi) {
            /* R-TYPE INSTRUCTIONS */

            /* Add rs1 to output */
            output.push_back(tokens[i + 1]);

            /* Add rs2 to output */
            output.push_back(tokens[i + 2]);

            /* Add rd to output */
            output.push_back(tokens[i + 3]);

            /* Add 4 bytes for a blank imm */
            output.push_back(0);
            output.push_back(0);
            output.push_back(0);
            output.push_back(0);

            i += 4;
        } else if (name < beq) {
            /* I-TYPE INSTRUCTIONS */

            /* Add rs to output */
            output.push_back(tokens[i + 1]);

            /* Add blank byte for rs2 */
            output.push_back(0);

            /* Add rd to output */
            output.push_back(tokens[i + 2]);

            /* Add 4 byte imm */
            uint64_t temp = tokens[i + 3];

            output.push_back(temp >> 24);
            output.push_back(temp >> 16);
            output.push_back(temp >> 8);
            output.push_back(temp);

            i += 4;
        } else if (name < j) {
            /* B-TYPE INSTRUCTIONS */

            /* Add rs1 to output */
            output.push_back(tokens[i + 1]);

            /* Add rs2 to output */
            output.push_back(tokens[i + 2]);

            /* Add rd to output */
            output.push_back(tokens[i + 3]);

            /* Add 4 byte imm */
            uint64_t temp = tokens[i + 4];

            output.push_back(temp >> 24);
            output.push_back(temp >> 16);
            output.push_back(temp >> 8);
            output.push_back(temp);

            i += 5;
        } else if (name < jr) {
            /* J-TYPE INSTRUCTIONS */

            /* Add rd to output */
            output.push_back(tokens[i + 1]);

            /* Add 6 byte imm */
            uint64_t temp = tokens[i + 2];

            output.push_back(temp >> 40);
            output.push_back(temp >> 32);
            output.push_back(temp >> 24);
            output.push_back(temp >> 16);
            output.push_back(temp >> 8);
            output.push_back(temp);

            i += 3;
        } else if (name < lui) {
            /* JR-TYPE INSTRUCTION */

            /* Add rs to output */
            output.push_back(tokens[i + 1]);

            /* Add blank byte for rs2 */
            output.push_back(0);

            /* Add rd to output */
            output.push_back(tokens[i + 2]);

            /* Add 4 byte imm */
            uint64_t temp = tokens[i + 3];

            output.push_back(temp >> 24);
            output.push_back(temp >> 16);
            output.push_back(temp >> 8);
            output.push_back(temp);

            i += 4;
        } else if (name < halt) {
            /* U-TYPE INSTRUCTION */

            /* Add rs to output */
            output.push_back(tokens[i + 1]);

            /* Add 0 bytes */
            output.push_back(0);
            output.push_back(0);

            /* Add 4 byte imm */
            uint64_t temp = tokens[i + 2];

            output.push_back(temp >> 24);
            output.push_back(temp >> 16);
            output.push_back(temp >> 8);
            output.push_back(temp);

            i += 3;
        } else if (name == halt) {
            output.pop_back();
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);
            output.push_back(255);

            goto endloop;

        } else {
            cout << "Error on token  " << i;

            if (tokens.size() - 1 >= i + 3 && i - 3 >= 0) {
                cout << tokens[i - 3] << " " << tokens[i - 2] << " " << tokens[i - 1] << " " << tokens[i] << " " << tokens[i + 1] << " " << tokens[i + 2] << " " << tokens[i + 3] << "\n";
            }
        }
    }

    endloop:

    ofstream outFile("out.bex", ios::out | ios::binary);

    for (const auto &e : output) outFile << e;

    outFile.close();
    fromFile.close();
}