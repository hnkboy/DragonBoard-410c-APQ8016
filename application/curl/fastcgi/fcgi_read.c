#include <stdio.h>
#include <fcgi_stdio.h>
#include <stdlib.h>


int main()
{
    int count = 0;
    while (FCGI_Accept() >= 0)
    {
        printf("Content-type: text/html\r\n"
                "\r\n"
                ""
                "FastCGI Hello!"
                "Request number %d running on host%s "
                "Process ID: %d\n", ++count, getenv("SERVER_NAME"), getpid());
		
    }
    return 0;
}

