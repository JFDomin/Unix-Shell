// The MIT License (MIT)
// 
// Copyright (c) 2024 Trevor Bakker 
// Copyright (c) 2024 Joshua Dominguez
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define WHITESPACE " \t\n" 

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 32

int main( int argc, char * argv[] )
{
  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );
  char error_message[30] = "An error has occurred\n";
  FILE *batch_file;
  //batch mode
  if(argc == 2)
  {
    batch_file = fopen(argv[1],"r");
    if(batch_file == NULL) // bad batch file, print error and exit(1)
    {
      write(STDERR_FILENO, error_message, strlen(error_message)); 
      exit(1);
    }
  }
  else if( argc > 2) // more than one input file is given, print error and exit
  {
    write(STDERR_FILENO, error_message, strlen(error_message)); 
    exit(1);
  }
  while( 1 )
  {
    // Print out the msh prompt
    if(argc == 1)
    {
      printf ("msh> ");
    }

    // Read the command from the commandi line.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something.
    //interactive mode
    if(argc == 1)
    {
      while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );
    }
    //batch mode
    else
    {
      while( !fgets (command_string, MAX_COMMAND_SIZE, batch_file))
      {
        if(feof(batch_file))
        {
          fclose(batch_file);
          exit(0);
        }
      }   
    }
    
    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_pointer;                                         
                                                           
    char *working_string  = strdup( command_string );                

    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    
    char *head_ptr = working_string;
    
    // Tokenize the input with whitespace used as the delimiter
    while ( ( (argument_pointer = strsep(&working_string, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_pointer, MAX_COMMAND_SIZE );
      // increment token_count only if the length of the last token length was > 0, otherwise NULLs the token
      // this allows for whitespace to be ignored before the command is given
      if( strlen( token[token_count] ) > 0 )
      {
        token_count++;
      }
      else
      {
        token[token_count] = NULL;
      }
    }

    // Now print the tokenized input as a debug check

    //built in commands exit and cd 
    if(token[0] == NULL)
    {
      continue;
    }
    if((strcmp(token[0], "exit") == 0) && token[1] != NULL)
    {
      write(STDERR_FILENO, error_message, strlen(error_message)); 
    }
    else if((strcmp(token[0], "exit") == 0))
    {
      exit(0);
    }
    else if( (strcmp(token[0], "cd") == 0) && token[1] == NULL )
    {
      write(STDERR_FILENO, error_message, strlen(error_message)); 
    }
    else if((strcmp(token[0], "cd") == 0) && token[2] != NULL)
    {
      write(STDERR_FILENO, error_message, strlen(error_message));
    }
    else if((strcmp(token[0], "cd") == 0))
    {
      //call chdir with the specified location, check return value of chdir to print error if it failed
      int dir_err = chdir(token[1]);
      if(dir_err != 0)
      {
        write(STDERR_FILENO, error_message, strlen(error_message));
      }
    }
    else
    {
      pid_t pid = fork();
      //child
      if(pid == 0)
      {
        int i;
        for( i=1; i<token_count; i++ )
        {
          if(token[i] == NULL)
          {
            break;
          }
          if( strcmp( token[i], ">" ) == 0 )
          {
            int fd = open( token[i+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
            // > is used but there is no output file specified, print error and exit 
            if(token[i+1] == NULL)
            {
              write(STDERR_FILENO, error_message, strlen(error_message)); 
              exit(0);
            }
            // > is used and there is more than one output file, print error and exit
            else if(token[i+2] != NULL)
            {
              write(STDERR_FILENO, error_message, strlen(error_message));
              exit(0);
            }
            if( fd < 0)
            {
              write(STDERR_FILENO, error_message, strlen(error_message)); 
            }
            dup2( fd, 1 );
            close( fd );
            // Trim off the > output part of the command
            token[i] = NULL;
           }
        }
          execvp(token[0], &token[0]);
          //checking return value of execution
          if(execvp(token[0], &token[0]) != 0)
          {
            write(STDERR_FILENO, error_message, strlen(error_message));
          }
      }
      else if( pid > 0) //parent process waits 
      {
        int status;
        wait(&status);
      }
      else //fork process failed
      {
        write(STDERR_FILENO, error_message, strlen(error_message));
      }
    }
    free( head_ptr );
  }
  return 0;
}
