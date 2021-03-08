$sourcedir=$args[0] # root source directory
$builddir=$args[1] # root build directory

cp $sourcedir/subprojects/luajit/src/lua51.dll -Destination $builddir
