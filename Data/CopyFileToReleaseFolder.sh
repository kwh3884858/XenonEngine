#!/bin/bash
# read -n2 -p "Copy xet to Release folder? [Y/n]" answer
# case $answer in
# Y | y)
    cp XenonScriptAssemblerMachineInstrction.xet ../Release/XenonScriptAssemblerMachineInstrction.xet
    cp XenonScriptAssemblerDelimiter.xet ../Release/XenonScriptAssemblerDelimiter.xet
    cp Main.xea ../Release/Main.xea
    cp cube.obj ../Release/cube.obj
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