#
# Use to build the .c and .h source based on static information
# in libdwarf.h.in and dwarf_errmsg_list.h and the dwarfdump *.list files.
# If you change any of those you should run this script
# (which, for non-linux non-unix may mean some changes of this script
# or of scripts/libbuild.sh or scripts.ddbuild.sh or baseconfig.h)
#

# This script is by David Anderson and hereby put into the public domain
# for anyone to use in any way.
#
cp scripts/baseconfig.h config.h
if [ $? -ne 0 ]
then
    echo "FAIL getting base config.h for .c .h builing.Runningfrom wrong place?"
    exit 1
fi

cd libdwarf
# We do not need to consider struct _Elf. Use standard version.
if [ $? -ne 0 ]
then
    echo "FAIL cd to libdwarf. Running buildstandardsource.sh from the wrong place"
    exit 1
fi

cp libdwarf.h.in libdwarf.h

sh ../scripts/libbuild.sh
if [ $? -ne 0 ]
then
    echo "FAIL libbuild.sh. "
    exit 1
fi
cd ..
if [ $? -ne 0 ]
then
    echo "FAIL cd back to top-level"
    exit 1
fi
cd dwarfdump
if [ $? -ne 0 ]
then
    echo "FAIL cd to dwarfdump. Running buildstandardsource.sh from the wrong place"
    exit 1
fi
sh ../scripts/ddbuild.sh
if [ $? -ne 0 ]
then
    echo "FAIL building dwarfdump .c .h source"
    exit 1
fi

cd ..
if [ $? -ne 0 ]
then
    echo "FAIL second cd back to top-level"
    exit 1
fi

rm -f config.h
rm -f libdwarf/libdwarf.h

echo "PASS. The .c and .h files are built"


exit 0
