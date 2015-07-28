/*
 ============================================================================
 Name        : lsmc_daemon.c
 Author      : Rakshit-Vamsee-Raviteja
 ============================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>

#define BUF_SIZE 80

int daemon_send (void)
{
	char buff [BUF_SIZE] = {0};
	char src_buff [BUF_SIZE] = {0};
    char * retval ;
	int FILE_FLAG = 1;
	/* 0 --- hash_algorithm used
	   1 --- integrity_files
	   2 --- integrity_app */

    /* Open the file that we wish to transfer */
	int fp_dest = open("/proc/lsmc",O_WRONLY);
	if (fp_dest == -1) {
        syslog (LOG_INFO, "Proc file open error:");
        return -1;
    }

	while (FILE_FLAG < 3) {
		switch (FILE_FLAG) {
			case 1 : strcpy (src_buff, "/etc/lsmc/integrity_files.txt");
					break;
			case 2 : strcpy (src_buff, "/etc/lsmc/integrity_app.txt");
					break;

		}

		FILE *fp_src = fopen (src_buff, "r");
		if (fp_src == NULL) {
		    syslog (LOG_INFO, "Config file open error:");
			close (fp_dest); //closing previously opened dest file.
		    return -1;
		}

		/* Read data from file and send it */
		while (1) {
			/*while doing fgets .. also check for \n that is present in the end*/

			retval = fgets (buff, BUF_SIZE, fp_src);

		    /* If read was success, send data. */
		    if (retval != NULL) {
		        syslog (LOG_INFO, "Sending \n");
				buff [strlen (buff)] = '\0'; //removing the appended '\n'
		        write (fp_dest,buff, strlen(buff)+1);
				syslog (LOG_INFO, "writing :- %s\n",buff);
				/*waiting for ack from dest*/
				read (fp_dest,buff, 4);
			}

		    /*
		     * If there is something tricky going on with read ..
		     * Either there was error, or we reached end of file.
		     */
		    if (retval == NULL)
		    {
		        if (feof (fp_src)) {
		        	if (FILE_FLAG == 2) {
						write (fp_dest,"APP_END\0", 8);
						syslog (LOG_INFO, "writing :- %s\n",buff);
						read (fp_dest,buff, 4);
					}
		        	else if (FILE_FLAG == 1) {
		        		write (fp_dest,"FILE_END\0", 9);
						syslog (LOG_INFO, "writing :- %s\n",buff);
		        		read (fp_dest,buff, 4);
		        	}
		        	syslog (LOG_INFO, "End of file\n");
		        }

		        else if (ferror (fp_src))
		            syslog (LOG_INFO, "Error reading\n");
		        break;
		    }
			memset (buff, '\0', BUF_SIZE);
		}
		fclose (fp_src);
		FILE_FLAG++;
	}
	close (fp_dest);
    return 0;
}

int main (void) {

	daemon_send();
	return EXIT_SUCCESS;
}
