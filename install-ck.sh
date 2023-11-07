#!/bin/bash
install_ck() {
    mkdir ~/.local/share/venvs > /dev/null 2>&1
    mkdir ~/.local/share/venvs/cutekit > /dev/null 2>&1
    python -m venv ~/.local/share/venvs/cutekit > /dev/null 2>&1
    cd ~/.local/share/venvs/cutekit/ > /dev/null 2>&1
    chmod +x ~/.local/share/venvs/cutekit/bin/activate > /dev/null 2>&1
    ~/.local/share/venvs/cutekit/bin/activate > /dev/null 2>&1
    git clone https://github.com/cute-engineering/cutekit > /dev/null 2>&1
    cd cutekit > /dev/null 2>&1
    ../bin/pip install -r requirements.txt > /dev/null 2>&1
    ../bin/pip install -e . > /dev/null 2>&1
    echo "CuteKit has been sucessfully installed!"
    echo "You need to add CuteKit manually to your PATH. Add this line in your shell initialization script (~/.bashrc for Bash) :"
    echo "'PATH=$PATH:~/.local/share/venvs/cutekit/bin'"
}

if [ "$1" == "install" ]; then
    if [ -d "~/.local/share/venvs/cutekit" ]; then
        echo "CuteKit is already installed"
    else
        echo "Installing CuteKit. Please wait..."
        install_ck
    fi
elif [ "$1" == "reinstall" ]; then
    read -p "CuteKit will be reinstalled. Are you sure? (y/n)" -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        # do dangerous stuff
        rm -rf ~/.local/share/venvs/cutekit/
        install_ck
    fi
elif [ "$1" == "uninstall" ]; then
    read -p "CuteKit will be uninstalled, but won't be removed from your PATH. Are you sure? (y/n)" -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        # do dangerous stuff
        rm -rf ~/.local/share/venvs/cutekit/
    fi
else
    echo "Invalid arguments."
    echo ""
    echo "Usage: ./install-ck.sh <install|uninstall|reinstall>"
fi

