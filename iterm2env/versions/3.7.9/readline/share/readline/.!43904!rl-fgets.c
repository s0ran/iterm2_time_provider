/*
Date: Tue, 16 Mar 2004 19:38:40 -0800
From: Harold Levy <Harold.Levy@synopsys.com>
Subject: fgets(stdin) --> readline() redirector
To: chet@po.cwru.edu

Hi Chet,

Here is something you may find useful enough to include in the readline
distribution.  It is a shared library that redirects calls to fgets(stdin)
to readline() via LD_PRELOAD, and it supports a custom prompt and list of
command names.  Many people have asked me for this file, so I thought I'd
pass it your way in hope of just including it with readline to begin with.

Best Regards,

-Harold
*/

/******************************************************************************
*******************************************************************************
  
  FILE NAME:    fgets.c                  TARGET:   libfgets.so
  AUTHOR:       Harold Levy              VERSION:  1.0
                hlevy@synopsys.com
  
  ABSTRACT:  Customize fgets() behavior via LD_PRELOAD in the following ways:
  
    -- If fgets(stdin) is called, redirect to GNU readline() to obtain
       command-line editing, file-name completion, history, etc.
  
    -- A list of commands for command-name completion can be configured by
       setting the environment-variable FGETS_COMMAND_FILE to a file containing
       the list of commands to be used.
  
    -- Command-line editing with readline() works best when the prompt string
       is known; you can set this with the FGETS_PROMPT environment variable.
  
    -- There special strings that libfgets will interpret as internal commands:
  
           _fgets_reset_    reset the command list
  
           _fgets_dump_     dump status
  
           _fgets_debug_    toggle debug messages

  HOW TO BUILD:  Here are examples of how to build libfgets.so on various
  platforms; you will have to add -I and -L flags to configure access to
  the readline header and library files.

  (32-bit builds with gcc)
  AIX:   gcc -fPIC fgets.c -shared -o libfgets.so -lc -ldl -lreadline -ltermcap
  HP-UX: gcc -fPIC fgets.c -shared -o libfgets.so -lc -ldld -lreadline
  Linux: gcc -fPIC fgets.c -shared -o libfgets.so -lc -ldl -lreadline
  SunOS: gcc -fPIC fgets.c -shared -o libfgets.so -lc -ldl -lgen -lreadline

  (64-bit builds without gcc)
  SunOS: SUNWspro/bin/cc -D_LARGEFILE64_SOURCE=1 -xtarget=ultra -xarch=v9 \
           -KPIC fgets.c -Bdynamic -lc -ldl -lgen -ltermcap -lreadline
  
  HOW TO USE:  Different operating systems have different levels of support
  for the LD_PRELOAD concept.  The generic method for 32-bit platforms is to
  put libtermcap.so, libfgets.so, and libreadline.so (with absolute paths)
  in the LD_PRELOAD environment variable, and to put their parent directories
  in the LD_LIBRARY_PATH environment variable.  Unfortunately there is no
  generic method for 64-bit platforms; e.g. for 64-bit SunOS, you would have
  to build both 32-bit and 64-bit libfgets and libreadline libraries, and
  use the LD_FLAGS_32 and LD_FLAGS_64 environment variables with preload and
  library_path configurations (a mix of 32-bit and 64-bit calls are made under
  64-bit SunOS).
  
  EXAMPLE WRAPPER:  Here is an example shell script wrapper around the
  program "foo" that uses fgets() for command-line input:

      #!/bin/csh
      #### replace this with the libtermcap.so directory:
      set dir1 = "/usr/lib"
      #### replace this with the libfgets.so directory:
      set dir2 = "/usr/fgets"
      #### replace this with the libreadline.so directory:
      set dir3 = "/usr/local/lib"
      set lib1 = "${dir1}/libtermcap.so"
      set lib2 = "${dir2}/libfgets.so"
      set lib3 = "${dir3}/libreadline.so"
      if ( "${?LD_PRELOAD}" ) then
        setenv LD_PRELOAD "${lib1}:${lib2}:${lib3}:${LD_PRELOAD}"
      else
        setenv LD_PRELOAD "${lib1}:${lib2}:${lib3}"
      endif
      if ( "${?LD_LIBRARY_PATH}" ) then
        setenv LD_LIBRARY_PATH "${dir1}:${dir2}:${dir3}:${LD_LIBRARY_PATH}"
      else
        setenv LD_LIBRARY_PATH "${dir1}:${dir2}:${dir3}"
      endif
      setenv FGETS_COMMAND_FILE "${dir2}/foo.commands"
      setenv FGETS_PROMPT       "foo> "
      exec "foo" $*
  
