#!/bin/bash

# ----------------------------------------------------------------
#
# VARIABLES
#
# ----------------------------------------------------------------


BOLD=`tput bold`
RED=`tput setaf 1`
GREEN=`tput setaf 10`
RESET=`tput sgr0`

tegia_folder=$(realpath ../../../)
configuration_folder=$(realpath ../)

mysql_host=$(awk -F= '/^host/ {print $2; exit}' $tegia_folder/tegia.cnf)
mysql_port=$(awk -F= '/^port/ {print $2; exit}' $tegia_folder/tegia.cnf)
mysql_user=$(awk -F= '/^user/ {print $2; exit}' $tegia_folder/tegia.cnf)
mysql_password=$(awk -F= '/^password/ {print $2; exit}' $tegia_folder/tegia.cnf)


# ----------------------------------------------------------------
#
# CONFIG FILE
#
# ----------------------------------------------------------------


sed -e "s|{CONFIGURATION_FOLDER}|$configuration_folder|g" \
    -e "s|{MYSQL_HOST}|$mysql_host|g" \
    -e "s|{MYSQL_PORT}|$mysql_port|g" \
    -e "s|{MYSQL_USER}|$mysql_user|g" \
    -e "s|{MYSQL_PASSWORD}|$mysql_password|g" \
    http.config-example.json > http.config.json

# validate

if jq -e . http.config.json >/dev/null; then
    echo "http.config.json is all good!"
else
    echo "ERROR IN http.config.json! CANNOT VALIDATE THE JSON!"
    exit 1
fi


# ----------------------------------------------------------------
#
# VENDORS
#
# ----------------------------------------------------------------


sudo apt install -y libfcgi libfcgi-dev nginx

wget -N http://mirrors.kernel.org/ubuntu/pool/universe/libc/libcgicc/libcgicc5v5_3.2.9-3ubuntu1_amd64.deb
sudo dpkg -i libcgicc5v5_3.2.9-3ubuntu1_amd64.deb
sudo apt --fix-broken install -y

wget -N http://mirrors.kernel.org/ubuntu/pool/universe/libc/libcgicc/libcgicc5-dev_3.2.9-3ubuntu1_amd64.deb
sudo dpkg -i libcgicc5-dev_3.2.9-3ubuntu1_amd64.deb


# ----------------------------------------------------------------
#
# DATABASE
#
# ----------------------------------------------------------------


for file in ../db/install/*.sql
do
	mysql --defaults-extra-file=$tegia_folder/tegia.cnf < "$file"
done


for file in ../db/update/*.sql
do
	mysql --defaults-extra-file=$tegia_folder/tegia.cnf < "$file"
done


exit 0
