```
 _  __     _ _ _        
| |/ /__ _| (_) |_ _  _ 
| ' </ _` | | |  _| || |
|_|\_\__,_|_|_|\__|\_, |   By Vandal -`♡´-
                   |__/ 

 A Kali Linux package management tool for Debian systems
```

### 🤷‍♂️ About
So I'm sure you're wondering what this is. It's a somewhat decent package manager I developed because, when I started with cybersecurity, I wanted a clean OS build with only essential packages. Initially, I had to manually download each package since they weren't available in the Debian repository, which was quite tedious. Eventually, I got the idea to integrate the Kali repository into Debian. Everyone in every forum said it was a terrible idea, but I had to try, and this is what I came up with. I'm not saying it's a work of art but it gets the job done.

Feel free to give it a try! For those wondering why it's written in C++ instead of Python, well, I just felt like it. There's also a [TODO](https://github.com/VandalByte/kality/blob/main/TODO.md) file where I've outlined plans to further optimize this tool, so feel free to check it out. If you're interested in contributing, your help would be greatly appreciated.

**PS:** I really think you should try it out in a virtual machine (VM). While it's usually fine, there's still a 1% chance it could mess up your system. Just a heads up, don't blame me if something goes wrong. I'll deny it.

<div align="center" style="border: 1px solid gray">
  <img width=90% src="https://raw.githubusercontent.com/VandalByte/kality/main/assets/kality-showcase.png" alt="Screenshot">
</div>


### 🛠️ Prerequisites
To compile and to ensure proper working of the script, you need to have the following packages installed on your system:
```bash
sudo apt install g++ make wget
```

### 🚀 Compilation and Installation
```bash
# First, clone the kality repository and navigate into it
git clone https://github.com/VandalByte/kality.git

# Now, compile the code and place the binary file in /usr/local/bin/ for system-wide access
sudo g++ kality.cpp -o /usr/local/bin/kality

# Now, give executable permission to the file
sudo chmod +x /usr/local/bin/kality

# Now run the update to install the keyring to the system
kality update

# You're all set! Check it out now
kality help
```

### 📋 How to use?
For the current release you can install, remove and update packages with kality

```bash
# EXAMPLE: Updating packages
kality update

# EXAMPLE: Installing packages
# Here I'm installing hashcat and John the Ripper
kality install hashcat john

# EXAMPLE: Uninstalling packages
kality uninstall hashcat john

# EXAMPLE: Displaying help menu
kality help

# EXAMPLE: Removing kality
# This will undo the changes done by kality, but the packages you installed will still be there, so uninstall them first
kality purge
```