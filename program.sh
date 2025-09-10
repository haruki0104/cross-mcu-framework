#!/bin/bash
# Script to program TI C2000 F28P55x output file to EVB using dslite and ccxml
# Usage: ./program.sh <output_file.hex|output_file.out>


# Default paths for TI tools and configuration
C2000WARE_ROOT="/opt/ti/c2000/C2000Ware_6_00_00_00"
CCXML_FILE="$C2000WARE_ROOT/device_support/f28p55x/common/targetConfigs/TMS320F28P550SJ9_LaunchPad.ccxml"
DSLite_PATH="/home/patrick/ti/uniflash_9.2.0/dslite.sh"

if [ $# -ne 1 ]; then
    echo "Usage: $0 <output_file.hex|output_file.out>"
    exit 1
fi

OUTPUT_FILE="$1"

if [ ! -f "$OUTPUT_FILE" ]; then
    echo "Error: Output file '$OUTPUT_FILE' not found."
    exit 2
fi

if [ ! -f "$CCXML_FILE" ]; then
    echo "Error: CCXML config file '$CCXML_FILE' not found."
    exit 3
fi

if [ ! -x "$DSLite_PATH" ]; then
    echo "Error: dslite not found at '$DSLite_PATH'. Please update the DSLite_PATH variable."
    exit 4
fi

"$DSLite_PATH" --config="$CCXML_FILE" -f "$OUTPUT_FILE" -v -u 
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo "Programming successful."
else
    echo "Programming failed with exit code $EXIT_CODE."
fi
exit $EXIT_CODE
