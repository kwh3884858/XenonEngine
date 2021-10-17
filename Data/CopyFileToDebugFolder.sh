#!/bin/bash
# read -n2 -p "Copy xet to debug folder? [Y/n]" answer
# case $answer in
# Y | y)
    cp XenonScriptAssemblerMachineInstrction.xet ../Debug/XenonScriptAssemblerMachineInstrction.xet
    cp XenonScriptAssemblerDelimiter.xet ../Debug/XenonScriptAssemblerDelimiter.xet
    cp Main.xea ../Debug/Main.xea
    cp cube.obj ../Debug/cube.obj
    cp KeQing.obj ../Debug/KeQing.obj
    cp KeQing.mtl ../Debug/KeQing.mtl
#     echo "Operation Complete"
#     ;;
# N | n)
#     echo "Ok, Good Bye"
#     ;;
# *)
#     echo "Error Choice"
#     ;;
# esac

# exec /bin/bash