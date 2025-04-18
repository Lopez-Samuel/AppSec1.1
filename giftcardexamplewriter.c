/*
 * Gift Card Example Writer
 * Original Author: Shoddycorp's Cut-Rate Contracting
 * Maintainer: ???
 * Comments added by Justin Cappos (JAC)
 * Modification Date: 8 July 2020
 */

#include <stdio.h>
#include "giftcard.h"
#include <string.h>

struct this_gift_card examplegc;
struct gift_card_data examplegcd;
struct gift_card_record_data examplegcrd;
struct gift_card_amount_change examplegcac;
struct gift_card_program examplegcp;


// Break it up so that we don't have long functions!  Good programming style!
//  (JAC: This is so wrong.  Global variable use / initialization is a 
//  terrible thing to do.)
void setupgc() {
	examplegc.num_bytes = 1000;
	examplegc.gift_card_data = (void *) &examplegcd;
	examplegcd.merchant_id = "GiftCardz.com                   ";
	examplegcd.customer_id = "DuaneGreenes Store 1451         ";
	examplegcd.number_of_gift_card_records = 2;

	/* JAC: Something seems fishy... */
	examplegcd.gift_card_record_data = malloc(examplegcd.number_of_gift_card_records);
	/* JAC: here too! */
	examplegcd.gift_card_record_data[0] = (void *) &examplegcrd;
	examplegcrd.record_size_in_bytes = 44;
	examplegcrd.type_of_record = 2; // JAC: Should be enum!  amount_change
	examplegcac.amount_added = 2000;
	examplegcac.actual_signature = "[ insert crypto signature here ]";
	examplegcrd.actual_record = (void *)&examplegcp;
	examplegcp.message = "message";
}


// moved into separate files to avoid erroenous style checker error...
/* JAC: opening and writing a fixed filename using a global is terrible style.
 *     the style checker was right!  This code is terrible   
 */
void writegc() {
	FILE *fd1;
	// JAC: Why don't any of these check for error return codes?!?
	fd1 = fopen("cov1.gft","w");
	fwrite(&examplegc.num_bytes,4,1,fd1);
	fwrite(examplegcd.merchant_id,32,1,fd1);
	fwrite(examplegcd.customer_id,32,1,fd1);
	fwrite(&examplegcd.number_of_gift_card_records,4,1,fd1);
	fwrite(&examplegcrd.record_size_in_bytes,4,1,fd1);
	fwrite(&examplegcrd.type_of_record,4,1,fd1);
	fwrite(&examplegcac.amount_added,4,1,fd1);
	fwrite(examplegcac.actual_signature,32,1,fd1);
	unsigned char message[32] = "message";
	fwrite(message, 32, 1, fd1);
	unsigned char pgm[256] = {
		0x00,10,12,
		0x01,(10),(12),
		0x02,(10),(12),
		0x03, (10),(12),
		0x04,(10),(12),
		0x06,(10),(12),
		0x07,0xff,0xff,
		0x08,0xff,0xff,
		0x10,(10),(12),
	};
	fwrite(pgm, 256, 1, fd1);
	fclose(fd1);
}

/* JAC: No args and return -1 for no reason!?! */
int main(void) {
	setupgc();
	writegc();
	return 0;
}
