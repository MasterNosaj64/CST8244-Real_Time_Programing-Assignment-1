#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <string.h>
#include <unistd.h>
#include "../../des_controller/src/des.h"

/*
 * Serves as client to Controller
 */

int main(int argc, char* argv[]) {

	person_t person;
	pid_t serverpid;
	int input_coid;

	//PHASE I

	if (argc != 2) {

		//print usage message
		fprintf(stderr, "Missing PID to controller\n");
		exit(EXIT_FAILURE);

	}

	serverpid = atoi(argv[1]);

	input_coid = ConnectAttach(ND_LOCAL_NODE, serverpid, 1, _NTO_SIDE_CHANNEL,
			0);

	if (input_coid == -1) {

		fprintf(stderr, "ConnectAttach had an error connecting to %d\n",
				serverpid);
		exit(EXIT_FAILURE);

	}

	person.direction = NONE;
	person.state = ACCEPTING;

	//PHASE II
	while (1) {

		printf(
				"Enter the event type (ls = left scan, rs = right scan, ws = weight scale, lo = left open, ro = right open, lc = left closed, rc = right closed , gru = guard right unlock, grl = guard right lock, gll = guard left lock, glu = guard left unlock, exit = exit programs)\n");
		scanf("%s", &person.msg);
		//clear buffer
		scanf("%*[^\n]");

		//check for scan
		if (strcmp(person.msg, "ls") == 0 || strcmp(person.msg, "rs") == 0) {

			printf("Enter the Person's ID:\n");
			scanf("%d", &person.person_id);

			//check for weight scale
		} else if (strcmp(person.msg, "ws") == 0) {

			printf("Enter the Person's weight:\n");
			scanf("%d", &person.weight);
		}

		//send person object
		//no reply object
		if (MsgSend(input_coid, &person, sizeof(person_t), &person,
				sizeof(person_t)) == -1L) {

			fprintf(stderr, "MsgSend had an error\n");
			exit(EXIT_FAILURE);

		}

		if (strcmp(person.msg, "exit") == 0) {
			break;
		}

		sleep(1);
	}

	sleep(1);
	//PHASE III
	ConnectDetach(input_coid);
	exit(EXIT_SUCCESS);
}
