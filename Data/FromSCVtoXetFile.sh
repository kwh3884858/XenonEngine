#!/bin/bash
read -n2 -p "Old Xet will be covored, are you sure? [Y/n]" answer
case $answer in
Y | y)
    cp XenonScriptAssemblerMachineInstrction.xet.csv XenonScriptAssemblerMachineInstrction.xet
    cp XenonScriptAssemblerDelimiter.xet.csv XenonScriptAssemblerDelimiter.xet
    echo "Operation Complete"
    ;;
N | n)
    echo "ok, good bye"
    ;;
*)
    echo "error choice"
    ;;
esac

exec /bin/bash