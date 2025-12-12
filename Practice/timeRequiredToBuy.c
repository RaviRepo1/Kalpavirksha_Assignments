int timeRequiredToBuy(int* tickets, int ticketsSize, int k) {
    long long total = 0;
    int target = tickets[k];   // number of tickets person k needs

    for (int i = 0; i < ticketsSize; i++) {

        if (i <= k) {
            // people before or at k buy min(tickets[i], tickets[k])
            if (tickets[i] < target)
                total += tickets[i];
            else
                total += target;
        }
        else {
            // people after k buy min(tickets[i], tickets[k] - 1)
            if (tickets[i] < target - 1)
                total += tickets[i];
            else
                total += (target - 1);
        }
    }

    return total;
}
