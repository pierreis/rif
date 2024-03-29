#!/bin/sh
# Copyright (c) 2013 Luca Barbato
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

VERSION_FILE="src/include/rif/util/rif_version.h"

if [ ! -f $VERSION_FILE ]; then
    echo "abi_version.sh: error: $VERSION_FILE does not exist" 1>&2
    exit 1
fi

CURRENT=`egrep '^#define +RIF_ABI_VERSION_CURRENT +[0-9]+$' $VERSION_FILE`
REVISION=`egrep '^#define +RIF_ABI_VERSION_REVISION +[0-9]+$' $VERSION_FILE`
AGE=`egrep '^#define +RIF_ABI_VERSION_AGE +[0-9]+$' $VERSION_FILE`

if [ -z "$CURRENT" -o -z "$REVISION" -o -z "$AGE" ]; then
    echo "abi_version.sh: error: could not extract version from $VERSION_FILE" 1>&2
    exit 1
fi

CURRENT=`echo $CURRENT | awk '{ print $3 }'`
REVISION=`echo $REVISION | awk '{ print $3 }'`
AGE=`echo $AGE | awk '{ print $3 }'`

case $1 in
    -libtool)
        printf '%s' "$CURRENT:$REVISION:$AGE"
    ;;
    *)
        printf '%s' "$CURRENT.$REVISION.$AGE"
    ;;
esac
