#!/bin/bash

# Environment variables
export PHYSLIST="%PHYSLIST%"

# Execute
ATLHECTB -m ATLHECTB.mac -pl %PHYSLIST% -t 12
