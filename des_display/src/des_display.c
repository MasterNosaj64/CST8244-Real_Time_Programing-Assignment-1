#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <process.h>
#include "../../des_controller/src/des.h"
/*
 * Serves as Server to Controller
 *
 */

int main(int argc, char* argv[]) {

	int rpid;
	int rcvid;		//indicates who we should reply to
	int chid;		// channel ID
	display_t display;

	//PHASE 1
	chid = ChannelCreate(0);
	if (chid == -1) {
		perror("failed to create the channel.\n");
		exit(EXIT_FAILURE);
	}
	//print the PID
	printf("The display is running as PID: %d\n", getpid());

	//PHASE 2
	while (1) {
		rcvid = MsgReceive(chid, &display, sizeof(display_t), NULL);

		if (rcvid == -1) {
			fprintf(stderr, "Error no message received from controller\n");
			exit(EXIT_FAILURE);
		}

		rpid = MsgReply(rcvid, EOK, NULL, 0);

		if (rpid == -1) {
			fprintf(stderr, "Error replying to Controller\n");
			exit(EXIT_FAILURE);
		}

		if (display.msg == ID_SCAN) {

			fprintf(stderr,"%s %d\n", outMessage[display.msg], display.person_id);

		} else if (display.msg == WEIGHED) {

			fprintf(stderr,"%s %d\n", outMessage[display.msg], display.person_weight);

		} else {
			fprintf(stderr,"%s\n", outMessage[display.msg]);

			if (display.msg == EXITING) {
				break;
			}
		}

	}		//end of while loop

	//PHASE 3
	ChannelDestroy(chid);
	return EXIT_SUCCESS;
}
