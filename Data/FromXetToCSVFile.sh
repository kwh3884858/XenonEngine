#!/bin/bash
read -n2 -p "Old CSV will be covored, are you sure? [Y/n]" answer
case $answer in
Y | y)
    cp XenonScriptAssemblerMachineInstrction.xet XenonScriptAssemblerMachineInstrction.xet.csv
    cp XenonScriptAssemblerDelimiter.xet XenonScriptAssemblerDelimiter.xet.csv
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