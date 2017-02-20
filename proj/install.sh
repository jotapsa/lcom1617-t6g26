if [ "$(id -u)" != "0" ]; then
   echo "ERROR: this script must be run as root!" 1>&2
   exit 1
fi
cd src

echo -e "\t downloading liblm.a"
curl -o liblm.a https://www.dropbox.com/s/1qytkudx9faya3e/liblm.a?dl=1 -L -O -k -s

echo -e "\t cleaning previous build files..."
make clean

echo -e "\t invoking make..."
make

echo -e "\t configuring tron as system service..."
cp /home/lcom/lcom1617-t6g26/proj/conf/tron /etc/system.conf.d/

echo -e "\t dowloading images..."
cd -
curl https://www.dropbox.com/s/y6mic7s1rivtiey/images.zip -L -O -k -s

echo -e "\t extracting images..."
unzip -q images.zip
rm -r images.zip

chmod 777 run.sh

echo -e "Tron has been successfully installed, enjoy your new game!
Just type ./run.sh while in root!"
