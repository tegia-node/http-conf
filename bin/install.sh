#!/bin/bash


# ////////////////////////////////////////////////////////////////////////////////
#
# VARIABLES
#
# ////////////////////////////////////////////////////////////////////////////////


RED=`tput setaf 1`
GREEN=`tput setaf 2`
RESET=`tput sgr0`

_OK_="${GREEN}[OK]  ${RESET}"
_ERR_="${RED}[ERR] ${RESET}"


# ////////////////////////////////////////////////////////////////////////////////
#
# VENDORS
#
# ////////////////////////////////////////////////////////////////////////////////


sudo apt install -y libfcgi libfcgi-dev nginx

wget -N http://mirrors.kernel.org/ubuntu/pool/universe/libc/libcgicc/libcgicc5v5_3.2.9-3ubuntu1_amd64.deb
sudo dpkg -i libcgicc5v5_3.2.9-3ubuntu1_amd64.deb
sudo apt --fix-broken install -y

wget -N http://mirrors.kernel.org/ubuntu/pool/universe/libc/libcgicc/libcgicc5-dev_3.2.9-3ubuntu1_amd64.deb
sudo dpkg -i libcgicc5-dev_3.2.9-3ubuntu1_amd64.deb

exit 0
