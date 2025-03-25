<div align="center" width=90%>
  <img src="https://raw.githubusercontent.com/VandalByte/kality/main/assets/banner.png" alt="Banner">
</div>

**Kality** is a neat package manager 📦 I put together in C++ to make it easier for you to install tools from the Kali Linux repository on any Debian-based system. It works by adding the Kali repo to your APT sources with a low priority, this lets you access those kali tools while avoiding version clashes with similar packages from your main distribution. Feel free to give it a try and Consider giving this project a star ⭐ if you liked it 🤗

## 🤷‍♂️ Why Kality?

- **Easy to use:** Building your own pentest lab? Easily download the tools you need quickly.
- **Made as safe as possible:** The Kali keyring is only active while running Kality.
  - This ensures it doesn't interfere with your regular APT operations.
  - The priority settings prevent clashes by prioritizing your native repository packages over Kali’s.
- **Access to pentest tools:** Get access to nearly all the essential pentest tools available.

<div align="center" style="border: 1px solid gray">
  <img src="https://raw.githubusercontent.com/VandalByte/kality/main/assets/kality-showcase.png" alt="Screenshot">
</div><br>

There's also a [TODO](https://github.com/VandalByte/kality/blob/main/TODO.md) file where I've outlined plans to further optimize this tool, so feel free to check it out. If you're interested in contributing, well that's really great, check out the [contributing](https://github.com/VandalByte/kality/blob/main/CONTRIBUTING.md) guidelines to know more.

> [!IMPORTANT]
> Try it out in a virtual machine (VM) first. It’s mostly safe, but there’s a small chance it could cause issues with your system.


## ⚙️ Installation

### Prerequisites

To compile and to ensure proper working of the script, you need to have the following packages installed on your system:
```sh
sudo apt-get install g++ make wget curl
```
### Compile from Source
First, clone the kality repository and navigate into it.
```sh
git clone https://github.com/VandalByte/kality.git
cd kality
```
Now, compile the code and place the binary file in `/usr/local/bin/` for system-wide access and give executable permission to the binary.
```sh
sudo g++ kality.cpp -o /usr/local/bin/kality
sudo chmod +x /usr/local/bin/kality
```
Finally, run the update to install the keyring into your system.
```sh
kality update
```
You're all set! Check it out.


## 📋 How to use?
For the current release you can install, remove and update packages with kality.

```bash
# EXAMPLE: Updating the packages
kality update

# EXAMPLE: Installing the packages
# Here I'm installing hashcat and John the Ripper.
kality install hashcat john

# EXAMPLE: Uninstalling the packages
kality uninstall hashcat john

# EXAMPLE: Displaying the help menu
kality help

# EXAMPLE: Removing kality
# This will undo the changes done by kality, but the packages 
# you installed will still be there, so remember to uninstall them first.
kality purge
```
