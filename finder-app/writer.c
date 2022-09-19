#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char**argv)
{
    const char *writefile = argv[1];
    const char *writestr = argv[2];
    FILE *fout;

    openlog(NULL,0,LOG_USER);

    if((argc-1) != 2) {
        syslog(LOG_ERR, "ERROR: %d arguments received. This function requires 2.\n", (argc-1));
        return 1;
    }

    fout = fopen(writefile, "w");
    syslog(LOG_DEBUG, "Writing %s to %s\n", writestr, writefile);
    fprintf(fout, "%s\n", writestr);
    fclose(fout);
    return 0;
}