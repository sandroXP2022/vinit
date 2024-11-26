#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <toml.hpp>
#include <vector>
#include <string>
#include <map>
#include <sys/mount.h>

#define CONFIG_FILE "/etc/init"

// Função para iniciar o daemon udev
void start_udev() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro de Execução");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Processo filho: Executa o daemon do udev
        execlp("udevd", "udevd", "--daemon", nullptr);
        perror("Erro ao executar udevd");
        exit(EXIT_FAILURE);
    }

    // Processo pai aguarda o daemon udev
    waitpid(pid, nullptr, 0);

    // Dispara eventos udev para inicializar dispositivos conectados
    system("udevadm trigger");
    system("udevadm settle");  // Aguarda que todos os eventos de udev sejam processados
}

void mount_filesystems() {
    printf("Montando sistemas de arquivos básicos...\n");
    if (mount("proc", "/proc", "proc", 0, nullptr) < 0) {
        perror("Falha ao montar /proc");
        exit(EXIT_FAILURE);
    }
    if (mount("sysfs", "/sys", "sysfs", 0, nullptr) < 0) {
        perror("Falha ao montar /sys");
        exit(EXIT_FAILURE);
    }
    if (mount("devtmpfs", "/dev", "devtmpfs", 0, nullptr) < 0) {
        perror("Falha ao montar /dev");
        exit(EXIT_FAILURE);
    }
}


// Função para iniciar um serviço com variáveis de ambiente
void start_service(const std::string &service, const std::map<std::string, std::string> &env_vars) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        // Configura as variáveis de ambiente
        std::vector<char *> envp;
        for (const auto &pair : env_vars) {
            std::string env_var = pair.first + "=" + pair.second;
            envp.push_back(strdup(env_var.c_str()));
        }
        envp.push_back(nullptr); // Finaliza a lista

        // Executa o serviço
        execve(service.c_str(), nullptr, envp.data());
        perror("execve");  // Se execve falhar
        exit(EXIT_FAILURE);
    }
    // Processo pai
    wait(nullptr);  // Aguarda o serviço terminar
}

// Função para ler o arquivo de configuração TOML e iniciar os serviços
void parse_and_start_services() {
    // Lê o arquivo de configuração
    auto config = toml::parse(CONFIG_FILE);

    // Analisa e coleta os serviços a serem iniciados
    auto init_services = toml::find<toml::table>(config, "init");
    std::vector<std::string> services;
    for (const auto &pair : init_services) {
        services.push_back(toml::get<std::string>(pair.second));
    }

    // Analisa e configura as variáveis de ambiente
    auto env_vars = toml::find<toml::table>(config, "env");
    std::map<std::string, std::string> env_map;
    for (const auto &pair : env_vars) {
        env_map[pair.first] = toml::get<std::string>(pair.second);
    }

    // Inicia os serviços
    for (const auto &service : services) {
        start_service(service, env_map);
    }
}

int main() {
    printf("Custom init process started.\n");

    mount_filesystems();

    // Inicializa o udev
    start_udev();
    
    // Inicia outros serviços configurados
    parse_and_start_services();

    // Manter o processo init ativo e gerenciar serviços
    while (1) {
        sleep(30);  // Espera para evitar o uso excessivo da CPU
    }

    return 0;
}

