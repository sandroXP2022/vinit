#include <iostream>
#include <fstream>
#include <string>

void suspend() {
    std::string state_file = "/sys/power/state";
    std::ofstream power_state(state_file);

    if (!power_state) {
        std::cerr << "Erro: Não foi possível abrir " << state_file << " para suspensão." << std::endl;
        return;
    }

    // Escreve "mem" para suspender o sistema
    power_state << "mem";
    if (!power_state) {
        std::cerr << "Erro: Falha ao suspender o sistema." << std::endl;
    }

    power_state.close();
}

int main() {
    std::cout << "Suspending system..." << std::endl;
    suspend();
    return 0;
}


