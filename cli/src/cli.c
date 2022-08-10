/*****************************************************************************
 *
 *    cli/src/cli.c
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "cli/cli_parameters.h"
#include "cli/cli.h"

void handle_cli (int argc, char **argv) {
    parameter_handler (argc, argv); 
}

