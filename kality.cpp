/*
 * Author: Vandal (VandalByte)
 *
 * GitHub: https://github.com/VandalByte/kality
 * License: GPL-3.0
 *
 * Disclaimer: This code is provided as-is. Use at your own risk.
 */


#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include <fstream>


 // ANSI COLOR CODES
namespace Color {
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31;1m";
    const std::string GREEN = "\033[32;1m";
    const std::string YELLOW = "\033[33;1m";
    const std::string BLUE = "\033[34;1m";
    const std::string CYAN = "\033[36;1m";
}


// LOGGING MACROS
#define LOG_INFO(msg)     std::cout << Color::GREEN << "[INFO] " << Color::RESET << msg << Color::RESET << std::endl
#define LOG_ERROR(msg)    std::cerr << Color::RED << "[ERROR] " << Color::RESET << msg << Color::RESET << std::endl


// FUNCTION PROTOTYPES
bool isRoot();
void help();
void update();
void install(std::vector<std::string> pkgs);
void uninstall(std::vector<std::string> pkgs);
std::vector<std::string> getArgs(int argc, char* argv[]);
void setKeyring(bool set);
void getKeyring();
void removeKeyring(std::string keyringPkg);
bool isKeyringInstalled(const std::string& pkg);
void addFileContent(const std::string& filePath, const std::string& content = "");


// KEYRING URL
const std::string KEYRING_URL = "https://http.kali.org/kali/pool/main/k/kali-archive-keyring/kali-archive-keyring_2024.1_all.deb";


// MAIN RUNNER
int main(int argc, char* argv[]) {
    // check if program is run as root user
    if (!isRoot()) {
        std::cout << "ERROR: Please run the program with root user privileges." << std::endl;
        return -1;
    }

    std::vector<std::string> packages;
    // check if any flag provided
    if (argc > 1) {
        std::string flag = argv[1];  // flag option
        // displays help menu and exits
        if (flag == "help" || flag == "h") {
            help();
            return 0;
        }

        else if (flag == "update" || flag == "u") {
            getKeyring(); // checking if keyring needs to be installed
            setKeyring(true); // setting keyring in files
            update();
            setKeyring(false); // removing keyring in files
        }

        else if (flag == "install" || flag == "i") {
            if (argc > 2) {
                packages = getArgs(argc, argv);  // getting package names
                setKeyring(true);
                install(packages);
                setKeyring(false);
            }
            else {
                LOG_ERROR("Please provide package(s) to install. For help use " + Color::YELLOW + "kality help" + Color::RESET);
                return -1;
            }
        }

        else if (flag == "uninstall" || flag == "x") {
            if (argc > 2) {
                packages = getArgs(argc, argv);
                setKeyring(true);
                uninstall(packages);
                setKeyring(false);
            }
            else {
                LOG_ERROR("Please provide package(s) to uninstall. For help use " + Color::YELLOW + "kality help" + Color::RESET);
                return -1;
            }
        }

        else {  // if no valid flag was found
            LOG_ERROR("Invalid argument(s). For help use " + Color::YELLOW + "kality help" + Color::RESET);
            return -1;
        }
    }

    else {  // if no valid arg given
        LOG_ERROR("No flags found. For help use " + Color::YELLOW + "kality help" + Color::RESET);
        return -1;
    }

    return 0;
}


// FUNCTIONS

// Function to sort the given arguments with required format
std::vector<std::string> getArgs(int argc, char* argv[]) {
    std::vector<std::string> packages;

    // adding package names to vector
    for (int i = 2; argv[i] != nullptr; i++) {
        std::string pkg = argv[i]; // pkg name
        // converting pkg name to lowercase (to avoid future errors)
        for (char& c : pkg) {
            c = std::tolower(c);
        }
        // check duplication of the packages from args
        bool isDuplicate = false;
        for (const auto& existingPkg : packages) {
            if (pkg == existingPkg) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            packages.push_back(pkg);
        }
    }
    return packages;
}


// Function to check whether the script excecuted with sudo privileges
bool isRoot() {
    // if effective user ID is 0 (root)
    return (geteuid() == 0);
}


// Function to display the help
void help() {
    std::cout << "\nUsage: kality [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  update (u)\t\t\t\tUpdate all installed packages\n";
    std::cout << "  install (i)\tpkg1 pkg2 ... pkgN\tInstall the provided packages if available in the repository\n";
    std::cout << "  uninstall (x)\tpkg1 pkg2 ... pkgN\tUninstall the provided packages if found installed\n";
    std::cout << "  help (h)\t\t\t\tDisplays this help message and exits\n\n";
}


// Function to update the packages
// #### TODO: add log file to update only necessary packages
void update() {
    // the update command
    int out = std::system("apt-get upgrade -y");
    if (out != 0) {
        LOG_ERROR("Failed to update packages.");
        return;
    }
    LOG_INFO("All packages have been successfully updated!");
}

// TODO: Check for a package, if available or not and give user the info
// Function to install the packages
void install(std::vector<std::string> pkgs) {
    std::string pkgList = "";
    for (const auto& pkg : pkgs) {
        pkgList += pkg + " ";
    }
    // the install command
    std::string cmd = "apt-get install " + pkgList + "-y";
    int out = std::system(cmd.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to install packages.");
        return;
    }
    LOG_INFO("All packages have been successfully installed!");
}


// Function to uninstall the packages
void uninstall(std::vector<std::string> pkgs) {
    std::cout << "Uninstall operation started..." << std::endl;
    std::string pkgList = "";
    for (const auto& pkg : pkgs) {
        pkgList += pkg + " ";
    }
    // the install command
    std::string cmd = "apt-get remove --purge " + pkgList + "-y";
    int out = std::system(cmd.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to uninstall packages.");
        return;
    }
    LOG_INFO("All packages have been successfully uninstalled!");
}


// Function to remove the kali keyring
void removeKeyring(std::string keyringPkg) {
    std::cout << "Removing the Kali keyring from the system...";
    std::string cmd = "dpkg --purge " + keyringPkg;

    int out = std::system(cmd.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to remove Kali keyring.");
        return;
    }
    LOG_INFO("Kali keyring removed successfully!");
}


// Function to set keyring file and preference file
void setKeyring(bool set) {
    std::string keyContent = "deb https://http.kali.org/kali kali-rolling main non-free contrib";
    std::string prefContent =
        "Package: *\n"
        "Pin: release a=kali-rolling\n"
        "Pin-Priority: 50\n";

    if (set) {
        // adding kali apt configuration to file
        addFileContent("/etc/apt/sources.list.d/kali.list", keyContent);
        // adding priority preference to the file
        // - must be in lower priority to avoid conflict between similar packages of main repo
        addFileContent("/etc/apt/preferences.d/kali.pref", prefContent);
        LOG_INFO("Kali keyring: SET");
    }
    else {
        addFileContent("/etc/apt/sources.list.d/kali.list");
        addFileContent("/etc/apt/preferences.d/kali.pref");
        LOG_INFO("Kali keyring: RELEASED");
    }
}


// Function to add the given content to the file specified (write)
void addFileContent(const std::string& filePath, const std::string& content) {
    std::ofstream outfile(filePath);

    if (!outfile.is_open()) {
        LOG_ERROR("Can't open the file '" + Color::BLUE + filePath + Color::RESET + "'");
        return;
    }

    outfile << content;

    if (!outfile.good()) {
        LOG_ERROR("Failed writing to the file '" + Color::BLUE + filePath + Color::RESET + "'");
    }

    outfile.close();
}


// Function to download and install Kali keyring
void getKeyring() {
    // Check if the keyring package is already installed
    if (isKeyringInstalled("kali-archive-keyring")) {
        LOG_INFO("Kali keyring is already installed. Skipping...");
        return;
    }
    // downloading the keyring
    std::cout << "Downloading and installing Kali keyring..." << std::endl;
    std::string wgetCmd = "wget " + KEYRING_URL;
    if (system(wgetCmd.c_str()) != 0) {
        LOG_ERROR("Failed to download the keyring.");
        return;
    }
    // installing the keyring
    std::string pkgFile = KEYRING_URL.substr(KEYRING_URL.find_last_of("/") + 1);
    std::string dpkgCmd = "sudo dpkg -i " + pkgFile;
    if (system(dpkgCmd.c_str()) != 0) {
        LOG_ERROR("Failed to install the keyring.");
        return;
    }
    // removing the keyring .deb file after installation
    if (remove(pkgFile.c_str()) != 0) {
        LOG_ERROR("Failed to remove the keyring file.");
        return;
    }
    LOG_INFO("Kali keyring installed successfully.");
}


// Function to check if a keyring is installed
bool isKeyringInstalled(const std::string& pkg) {
    std::string cmd = "dpkg-query -W -f='${Status}' " + pkg + " 2>/dev/null | grep -q 'install ok installed'";
    return (system(cmd.c_str()) == 0);
}
