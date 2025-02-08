/*
* Kality - Kali Linux package management tool for Debian systems
*
* Author: Vandal (VandalByte)
* GitHub: https://github.com/VandalByte/kality
* License: GPL-3.0
*
* DISCLAIMER:
* This program is provided as-is. Use at your own risk.
*
* DESCRIPTION:
* Kality is a package management tool designed to integrate the Kali Linux
* repository into Debian-based systems. It allows installation of packages
* from the Kali repository that are not available in the Debian repository,
* while ensuring proper handling of keyrings and package preferences.
*
* Feel free to check out the official GitHub repository for any queries or issues.
*/


#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <regex>

using namespace std;

// ANSI COLOR CODES
namespace Color {
    const string RESET = "\033[0m";
    const string RED = "\033[31;1m";
    const string GREEN = "\033[32;1m";
    const string YELLOW = "\033[33;1m";
    const string BLUE = "\033[34;1m";
    const string CYAN = "\033[36;1m";
}


// LOGGING MACROS
#define LOG_INFO(msg) cout << Color::GREEN << "[INFO] " << Color::RESET << msg << Color::RESET << endl
#define LOG_ERROR(msg) cerr << Color::RED << "[ERROR] " << Color::RESET << msg << Color::RESET << endl


// FUNCTION PROTOTYPES
bool is_root();
void help();
void update();
void purge();
void install(vector<string> pkgs);
void uninstall(vector<string> pkgs);
vector<string> get_args(int argc, char* argv[]);
void set_keyring(bool set);
void get_keyring();
void rm_keyring();
bool is_keyring_installed(const string& pkg);
void add_file_content(const string& filePath, const string& content = "");
string find_deb_url(const string &html_content, const string &base_url);

// KEYRING URL
const string KEYRING_BASE_URL = "https://http.kali.org/kali/pool/main/k/kali-archive-keyring/";


// MAIN RUNNER
int main(int argc, char* argv[]) {
    // check if program is run as root user
    if (!is_root()) {
        LOG_ERROR("Please run with sudo privileges.\n");
        return -1;
    }

    // packages storage
    vector<string> packages;

    // check if any flag provided
    if (argc > 1) {
        string flag = argv[1];  // flag option
        // displays help menu and exits
        if (flag == "help" || flag == "h") {
            help();
        }
        // removing kality from system
        else if (flag == "purge" || flag == "p") {
            // ask for confirmation
            string confirm;
            cout << "Are you sure you want to remove Kalify? (y/n): ";
            getline(cin, confirm);
            if (confirm == "yes" || confirm == "y") {
                purge();
            }
            else {
                cout << endl;
                LOG_INFO("Purge operation cancelled!");
            }
        }
        // updating installed packages
        else if (flag == "update" || flag == "u") {
            get_keyring(); // checking if keyring needs to be installed
            set_keyring(true); // setting keyring in files
            update();
            set_keyring(false); // removing keyring in files
        }
        // installing packages
        else if (flag == "install" || flag == "i") {
            if (argc > 2) {
                packages = get_args(argc, argv);  // getting package names
                set_keyring(true);
                install(packages);
                set_keyring(false);
            }
            else {
                LOG_ERROR("Please provide package(s) to install. For help use " + Color::YELLOW + "kality help" + Color::RESET);
                return -1;
            }
        }
        // uninstalling packages
        else if (flag == "uninstall" || flag == "x") {
            if (argc > 2) {
                packages = get_args(argc, argv);
                set_keyring(true);
                uninstall(packages);
                set_keyring(false);
            }
            else {
                LOG_ERROR("Please provide package(s) to uninstall. For help use " + Color::YELLOW + "kality help" + Color::RESET);
                return -1;
            }
        }
        // if no valid flag was found
        else {
            LOG_ERROR("Invalid argument(s). For help use " + Color::YELLOW + "kality help" + Color::RESET);
            return -1;
        }
    }
    // if no valid arg given
    else {
        LOG_ERROR("No flags found. For help use " + Color::YELLOW + "kality help" + Color::RESET);
        return -1;
    }
    return 0;
}


// FUNCTIONS

// Function to find the .deb file
string find_deb_url(const string &html_content, const string &base_url) {
    regex deb_regex("href=\"(.*?\\.deb)\"");
    smatch matches;

    if (regex_search(html_content, matches, deb_regex)) {
        return base_url + matches.str(1);
    } else {
        return "";  // return null string if not found (highly unlikely) TODO: might need a check
    }
}

// Function to sort the given arguments with required format
vector<string> get_args(int argc, char* argv[]) {
    vector<string> packages;

    // adding package names to vector
    for (int i = 2; argv[i] != nullptr; i++) {
        string pkg = argv[i]; // pkg name
        // converting pkg name to lowercase (to avoid future errors)
        for (char& c : pkg) {
            c = tolower(c);
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
bool is_root() {
    // if effective user ID is 0 (root)
    return (geteuid() == 0);
}


// Function to display the help
void help() {
    cout << "  _  __     _ _ _        \n"
        " | |/ /__ _| (_) |_ _  _ \n"
        " | ' </ _` | | |  _| || |\n"
        " |_|\\_\\__,_|_|_|\\__|\\_, |   By " + Color::CYAN + "Vandal\n" + Color::RESET + ""
        "                    |__/ \n";
    cout << "\n Usage: kality [options]\n\n";
    cout << Color::GREEN + " OPTIONS:\n\n" + Color::RESET;
    cout << "  update (u)\t\t\tUpdate all installed packages\n";
    cout << "  install (i)\t\t\tInstall the provided packages if available in the repository\n";
    cout << Color::YELLOW + "  \t\t\t\te.g. kality install pkg1 pkg2 ... pkgN\t\n" + Color::RESET;
    cout << "  uninstall (x)\t\t\tUninstall the provided packages if found installed\n";
    cout << Color::YELLOW + "  \t\t\t\te.g. kality uninstall pkg1 pkg2 ... pkgN\t\n" + Color::RESET;
    cout << "  purge (p)\t\t\tRemoves kality from the system\n";
    cout << "  help (h)\t\t\tDisplays this help message and exits\n\n";
}


// Function to update the packages
void update() {
    // the update command
    int out = system("apt-get upgrade -y");
    if (out != 0) {
        LOG_ERROR("Failed to update packages.");
        return;
    }
    LOG_INFO("All packages have been successfully updated!");
}


// Function to remove all kality changes
void purge() {
    string srcFile = "/etc/apt/sources.list.d/kali.list";
    string preFile = "/etc/apt/preferences.d/kali.pref";
    // removing keyring pkg
    rm_keyring();
    // removing the kali.list file
    if (remove(srcFile.c_str()) != 0) {
        LOG_ERROR("Failed to remove kali.list file.");
    }
    // removing the kali.pref file
    if (remove(preFile.c_str()) != 0) {
        LOG_ERROR("Failed to remove kali.pref file.");
    }
    int out = system("apt-get update -y");
    if (out != 0) {
        LOG_ERROR("Failed to update packages.");
        return;
    }
    LOG_INFO("Kality modified files have been removed!");
    cout << Color::BLUE + "[NOTE]" + Color::RESET + " Now run the following to remove the bin file:\n";
    cout << Color::YELLOW + "       sudo rm /usr/local/bin/kality\n" + Color::RESET;
}


// Function to install the packages
void install(vector<string> pkgs) {
    string pkgList = "";
    for (const auto& pkg : pkgs) {
        pkgList += pkg + " ";
    }
    // the install command
    string cmd = "apt-get install " + pkgList + "-y";
    int out = system(cmd.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to install packages.");
        return;
    }
    LOG_INFO("All packages have been successfully installed!");
}


// Function to uninstall the packages
void uninstall(vector<string> pkgs) {
    string pkgList = "";
    for (const auto& pkg : pkgs) {
        pkgList += pkg + " ";
    }
    // the install command
    string cmdPurge = "apt-get remove --purge " + pkgList + "-y";
    int out = system(cmdPurge.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to uninstall packages.");
        return;
    }
    string cmdAutoremove = "apt-get autoremove -y";
    out = system(cmdAutoremove.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to remove dependency packages.");
        return;
    }
    LOG_INFO("All packages have been successfully uninstalled!");
}


// Function to remove the kali keyring
void rm_keyring() {
    LOG_INFO("Removing the Kali keyring from the system...");
    // string cmd = "dpkg --purge " + keyringPkg;
    // purge command
    string cmd = "dpkg --purge kali-archive-keyring";

    int out = system(cmd.c_str());
    if (out != 0) {
        LOG_ERROR("Failed to remove Kali keyring.");
        return;
    }
    LOG_INFO("Kali keyring removed successfully!");
}


// Function to set keyring file and preference file
void set_keyring(bool set) {
    string keyContent = "deb https://http.kali.org/kali kali-rolling main non-free contrib";
    string prefContent =
        "Package: *\n"
        "Pin: release a=kali-rolling\n"
        "Pin-Priority: 50\n";

    if (set) {
        // adding kali apt configuration to file
        add_file_content("/etc/apt/sources.list.d/kali.list", keyContent);
        // adding priority preference to the file
        // : must be in lower priority to avoid conflict between similar packages of main repo
        add_file_content("/etc/apt/preferences.d/kali.pref", prefContent);
        LOG_INFO("Kali keyring: SET");
    }
    else {
        add_file_content("/etc/apt/sources.list.d/kali.list");
        add_file_content("/etc/apt/preferences.d/kali.pref");
        LOG_INFO("Kali keyring: RELEASED");
    }
}


// Function to add the given content to the file specified (write)
void add_file_content(const string& filePath, const string& content) {
    ofstream outfile(filePath);

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
void get_keyring() {
    // Check if the keyring package is already installed
    if (is_keyring_installed("kali-archive-keyring")) {
        LOG_INFO("Kali keyring is already installed. Skipping...");
        return;
    }

    // Latest keyring check from the web page
    LOG_INFO("Finding the latest Kali keyring...");
    string temp_filename = "temp_keyring_page.html";
    string KEYRING_URL;
    string curl_cmd = "curl -s " + KEYRING_BASE_URL + " -o " + temp_filename;
    int res = system(curl_cmd.c_str());

    // Check if curl command was successful
    if (res == 0) {
        ifstream fin(temp_filename);
        if (!fin) {
            LOG_ERROR("Failed to open temp keyring page for parsing.");
            return;
        }

        // Read the content of the downloaded web page into a string
        string html_content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
        fin.close();

        // Find the .deb file URL by concatenating the base URL with the .deb file path
        KEYRING_URL = find_deb_url(html_content, KEYRING_BASE_URL); // deb file URL
        remove(temp_filename.c_str());
    } else {
        LOG_ERROR("Failed to download the webpage using curl.");
        return;
    }
    
    // Downloading the keyring
    LOG_INFO("Downloading and installing Kali keyring...");
    string wgetCmd = "wget " + KEYRING_URL;
    if (system(wgetCmd.c_str()) != 0) {
        LOG_ERROR("Failed to download the keyring.");
        return;
    }
    // Installing the keyring
    string pkgFile = KEYRING_URL.substr(KEYRING_URL.find_last_of("/") + 1);
    string dpkgCmd = "sudo dpkg -i " + pkgFile;
    if (system(dpkgCmd.c_str()) != 0) {
        LOG_ERROR("Failed to install the keyring.");
        return;
    }
    // Removing the keyring .deb file after installation
    if (remove(pkgFile.c_str()) != 0) {
        LOG_ERROR("Failed to remove the keyring file.");
        return;
    }
    LOG_INFO("Kali keyring installed successfully.");
}


// Function to check if a keyring is installed
bool is_keyring_installed(const string& pkg) {
    string cmd = "dpkg-query -W -f='${Status}' " + pkg + " 2>/dev/null | grep -q 'install ok installed'";
    return (system(cmd.c_str()) == 0);
}
