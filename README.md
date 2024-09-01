<div align="center" width=90%>
  <img src="https://raw.githubusercontent.com/VandalByte/kality/main/assets/banner.png" alt="Banner">
</div>

# 🤷‍♂️ About
Kality is a neat package manager I put together in C++ to make it easier for you to install tools from the Kali Linux repository on any Debian-based system. It works by adding the Kali repo to your APT sources with a low priority, this lets you access those kali tools while avoiding version clashes with similar packages from your main distribution. Feel free to give it a try!

There's also a [TODO](https://github.com/VandalByte/kality/blob/main/TODO.md) file where I've outlined plans to further optimize this tool, so feel free to check it out. If you're interested in contributing, well that's really great, check out the [contributing](https://github.com/VandalByte/kality/blob/main/CONTRIBUTING.md) guidelines to know more.

> [!IMPORTANT]
> You should try it out in a virtual machine (VM) first, it's usually fine, there's still a 1% chance it could mess up your system. Just a heads up.

<div align="center" style="border: 1px solid gray">
  <img src="https://raw.githubusercontent.com/VandalByte/kality/main/assets/kality-showcase.png" alt="Screenshot">
</div>


# 🛠️ Prerequisites
To compile and to ensure proper working of the script, you need to have the following packages installed on your system:
```bash
sudo apt install g++ make wget
```

# 🚀 Compilation and Installation

First, clone the kality repository and navigate into it.
```bash
git clone https://github.com/VandalByte/kality.git
```
Now, compile the code and place the binary file in `/usr/local/bin/` for system-wide access.
```bash
sudo g++ kality.cpp -o /usr/local/bin/kality
```
Now, give executable permission to the file.
```bash
sudo chmod +x /usr/local/bin/kality
```
Now run the update to install the keyring to the system.
```bash
kality update
```
You're all set! Check it out.


# 📋 How to use?
For the current release you can install, remove and update packages with kality.

```bash
# EXAMPLE: Updating packages
kality update

# EXAMPLE: Installing packages
# Here I'm installing hashcat and John the Ripper.
kality install hashcat john

# EXAMPLE: Uninstalling packages
kality uninstall hashcat john

# EXAMPLE: Displaying the help menu
kality help

# EXAMPLE: Removing kality
# This will undo the changes done by kality, but the packages 
# you installed will still be there, so remember to uninstall them first.
kality purge
```

# 💡 What's Next?

I'm still trying to finish up my TODO list and add the rest of the functions.