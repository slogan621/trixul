#!/bin/sh

clear
# check to see if we are root already. if so, just run the installer
id=`id --user`
if [ $id -eq 0 ]
then
    sh weathermanager-installer
else

# ask the user if he or she wants to install as root. if the
# user wants to run as root, run via su, which prompts for a 
# password, otherwise just execute it with effective ID of
# the user executing this script

echo "*****************************************"
echo "Welcome the the weathermanager installer."
echo
echo "To install in a standard location, such as /usr/local, you must" 
echo "run the installer as root."
echo ""
while true
do
    echo "Would you like to install as root (Y/N/Q)?"
    read input
    if [ $input = 'Y' -o $input = 'y' ]
    then
        echo "Please enter the root password at the prompt."
        # run as root
        su -c "sh weathermanager-installer"
        break
    else
    if [ $input = 'N' -o $input = 'n' ]
    then
        # run with same perms as the script
        sh weathermanager-installer
        break
    else
    if [ $input = 'Q' -o $input = 'q' ]
    then
        # bail, user doesn't want to install
        break
    fi
    fi
    fi
done
fi
