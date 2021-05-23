// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "./dintwm.h"

struct timerequest *create_timer(unsigned long unit)
{
	/* return a pointer to a timer request.  If any problem, return NULL */
	struct MsgPort *timerport;
	signed char error;
	struct timerequest *TimerIO;
	unsigned char tdevice[] = "timer.device";

	timerport = CreatePort(0, 0);

	if (timerport == NULL) {
		return (NULL);
	}

	TimerIO =
	    (struct timerequest *)CreateExtIO(timerport, //-V2545
					      sizeof(struct timerequest));

	if (TimerIO == NULL) {
		DeletePort(timerport);	/* Delete message port */
		return (NULL);
	}

	error = OpenDevice(tdevice, unit, (struct IORequest *)TimerIO, 0L); //-V2545

	if ((int)error != 0) {
		delete_timer(TimerIO);
		return (NULL);
	}

	return (TimerIO);
}

void time_delay(struct timerequest *tr, struct timeval *tv) {
	unsigned short traddreq = TR_ADDREQUEST;
	tr->tr_node.io_Command = traddreq;	/* add a new timer request */

	/* structure assignment */
	tr->tr_time = *tv;

	/* post request to the timer -- will go to sleep till done */
	(void)DoIO((struct IORequest *)tr); //-V2545
}

void delete_timer(struct timerequest *tr) {
	if (tr != 0) {
		struct MsgPort *tp;
		tp = tr->tr_node.io_Message.mn_ReplyPort;

		if (tp != 0) {
			DeletePort(tp);
		}

		CloseDevice((struct IORequest *)tr); //-V2545
		DeleteExtIO((struct IORequest *)tr); //-V2545
	}
}
