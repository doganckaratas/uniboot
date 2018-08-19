
FIRST_CHK=0
SECND_CHK=0

if [ $# -ne '1' ];
then
	echo "Powerloader 1.1"
	echo "Physical disk format utility"
	echo "Usage: $0 <device>"
	exit -1
fi

echo "Selected device: $1"
echo "Warning, this operation DESTROYS data on disk."
read -p "Do you proceed? (y/N)" yn

case $yn in
	[Yy]* ) FIRST_CHK=1;;
	* ) echo "No selected";;
esac

if [ $FIRST_CHK -ne 1 ];
then
	echo "Abandoning.."
	exit -2
fi

read -p "Are you REALLY SURE about this? I'm double checking for avoid UNWANTED DATA LOSS. (y/N)" yn

case $yn in
	[Yy]* ) SECND_CHK=1;;
	* ) echo "No selected";;
esac

if [ $SECND_CHK -ne 1 ];
then
	echo "Abandoning.."
	exit -3
fi

echo "Writing disk header to device $1..."
dd status=progress conv=notrunc if=stage1.bin of=$1
echo "Done..."
