enum {
        HOLDER_1,
        HOLDER_2
};

#define NTICKETS 6

void give_tickets(int how_many, int *tickets, int holder){
        int i;

        for(i = 0; i != how_many; i++)
                tickets[i] = holder;
}

/*returns the winner*/
int do_lottery(void){
        int tickets[NTICKETS], i, lucky_ticket;

        give_tickets(5, tickets, HOLDER_1);
        give_tickets(1, &tickets[5], HOLDER_2);

        lucky_ticket = get_lucky_ticket(); /*your ticket generator*/

        return tickets[lucky_ticket];
}
