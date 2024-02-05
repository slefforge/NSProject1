#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>
#include<unistd.h>
#include <limits.h>
#include <signal.h>
#include <malloc.h>
#include<string.h>

/*Error Handling*/
 #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

       static char *buffer;

       static void
       handler(int sig, siginfo_t *si, void *unused)
       {
           /* Note: calling printf() from a signal handler is not safe
              (and should not be done in production programs), since
              printf() is not async-signal-safe; see signal-safety(7).
              Nevertheless, we use printf() here as a simple way of
              showing that the handler was called. */

           printf("Got SIGSEGV at address: 0x%lx\n",
                   (long) si->si_addr);
           exit(EXIT_FAILURE);
       }
/*Main function*/
int main(int argc, char *argv[])
       {
           char *p,*p_new,*buffer;
	   char c;
           int pagesize;
	   int i=0,size;
           struct sigaction sa;

           sa.sa_flags = SA_SIGINFO;
           sigemptyset(&sa.sa_mask);
           sa.sa_sigaction = handler;
           if (sigaction(SIGSEGV, &sa, NULL) == -1)
               handle_error("sigaction");

           pagesize = sysconf(_SC_PAGE_SIZE);  /* Initializing Pagesize, here pagesize=4096 Bytes*/
           if (pagesize == -1)
               handle_error("sysconf");

    /* Allocate a buffer; it will have the default
       protection of PROT_READ|PROT_WRITE. */
    size=pagesize*10;
    p = memalign(pagesize,size);          /*Allocating buffer'p' of size = ten pages*/
    if (p == NULL)
    handle_error("memalign");

    memset(p,0x00,size);                     /*Copying 'B' to whole buffer*/
    memset(p,0x41,size); 
    
    for(i=0;i<10;i++)
    {
	printf("Address of %d Page: %lx\n",i+1,p+(i*4096));	/*Printing all pages first  bytes from first page. The usage of %d format specifier causes compilation warnings. Can you figure out why?*/
	
    }

// Can start writing code here and can define variables for functions above

// PART 1
// Writing data
   buffer=p;                      /*pointing buffer, 'buffer' to starting address of p*/
   i=8*pagesize;			 
   *(buffer+i) = 'S';    
   *(buffer+(i+1)) = 'A';
   *(buffer+(i+2)) = 'M';
   *(buffer+i+pagesize) = 'S';    /*Trying to Overwrite first and last 3 bytes of first page */
   *(buffer+(i+1)+pagesize) = 'A';
   *(buffer+(i+2)+pagesize) = 'M';
   
// Printing data
    for(i=8*pagesize;i<(8*pagesize+3);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);	/*Printing first 3 bytes from nineth page*/
    }
    for(i=9*pagesize;i<(9*pagesize+3);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);	/*Printing first 3 bytes from tenth page*/
    }

// PART 2
// Giving Permissions
if (mprotect(p+pagesize*6, pagesize*2, PROT_READ|PROT_WRITE)==-1) /*using mprotect on 7th and 8th page with read and write access*/
	{handle_error("mprotect");
        }

// Writing Data
   buffer=p;                      /*pointing buffer, 'buffer' to starting address of p*/
   i=6*pagesize;			 
   *(buffer+i) = 'L';    
   *(buffer+(i+1)) = 'E';
   *(buffer+(i+2)) = 'F';
   *(buffer+(i+3)) = 'F';
   *(buffer+(i+4)) = 'O';
   *(buffer+(i+5)) = 'R';
   *(buffer+(i+6)) = 'G';
   *(buffer+(i+7)) = 'E';
   *(buffer+i+pagesize) = 'L';    
   *(buffer+(i+1+pagesize)) = 'E';
   *(buffer+(i+2+pagesize)) = 'F';
   *(buffer+(i+3+pagesize)) = 'F';
   *(buffer+(i+4+pagesize)) = 'O';
   *(buffer+(i+5+pagesize)) = 'R';
   *(buffer+(i+6+pagesize)) = 'G';
   *(buffer+(i+7+pagesize)) = 'E';

// Printing Data
    for(i=6*pagesize;i<(6*pagesize+8);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);
    }
    for(i=7*pagesize;i<(7*pagesize+8);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);
    }

// PART 3
// Giving Permissions
if (mprotect(p+pagesize*4, pagesize*2, PROT_WRITE)==-1) /*using mprotect on 5th and 6th page with write access only*/
	{handle_error("mprotect");
        }

// Writing Data
   buffer=p;                      /*pointing buffer, 'buffer' to starting address of p*/
   i=4*pagesize;			 
   *(buffer+i) = 'p';    
   *(buffer+(i+1)) = 'l';
   *(buffer+(i+2)) = 'e';
   *(buffer+(i+3)) = 'f';
   *(buffer+(i+4)) = 'f';
   *(buffer+(i+5)) = 'o';
   *(buffer+(i+6)) = 'r';
   *(buffer+(i+7)) = 'g';
   *(buffer+(i+8)) = 'e';
   *(buffer+(i+9)) = '3';
   *(buffer+i+pagesize) = 'p';    
   *(buffer+(i+1+pagesize)) = 'l';
   *(buffer+(i+2+pagesize)) = 'e';
   *(buffer+(i+3+pagesize)) = 'f';
   *(buffer+(i+4+pagesize)) = 'f';
   *(buffer+(i+5+pagesize)) = 'o';
   *(buffer+(i+6+pagesize)) = 'r';
   *(buffer+(i+7+pagesize)) = 'g';
   *(buffer+(i+8+pagesize)) = 'e';
   *(buffer+(i+9+pagesize)) = '3';

// Printing Data
    for(i=4*pagesize;i<(4*pagesize+10);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);
    }
    for(i=5*pagesize;i<(5*pagesize+10);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p+i),p+i);
    }

// Part 4
// Creating a buffer
    p_new = memalign(pagesize,pagesize*2);          /*Allocating buffer'p_new' of size = two pages*/
    if (p_new == NULL)
    handle_error("memalign");

// Writing to buffer
    memcpy(p_new, p+pagesize*6, pagesize*2);

// Printing Data
    for(i=0;i<(8);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p_new+i),p_new+i);
    }
    for(i=pagesize;i<(pagesize+8);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p_new+i),p_new+i);
    }

// Part 5
// Writing to buffer
    memcpy(p_new, p+pagesize*5, pagesize);
    memcpy(p_new+pagesize, p+pagesize*8, pagesize);

// Printing Data
    for(i=0;i<(10);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p_new+i),p_new+i);
    }
    for(i=pagesize;i<(pagesize+3);i++) {
	    printf("%d=%c, %lx\n",i+1,*(p_new+i),p_new+i);
    }

   
           exit(EXIT_SUCCESS);
       }


