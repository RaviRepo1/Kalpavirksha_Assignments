int countTriples(int n) {

    bool *isSquare = (bool*)calloc(n * n + 1, sizeof(bool));
    for (int i = 1; i <= n; i++) {
        isSquare[i * i] = true;
    }

    int count = 0;

   
    for (int a = 1; a <= n; a++) {
        for (int b = 1; b <= n; b++) {
            int sum = a * a + b * b;
            if (sum <= n * n && isSquare[sum]) {
                count++;
            }
        }
    }

    free(isSquare);
    return count;
}
