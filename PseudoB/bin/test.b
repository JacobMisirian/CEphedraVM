main () {
    auto a = 97;
    auto b = 0;
    while (a < 100) {
        putchar (4000 + b, a);
        a = a + 1;
        b = b + 1;
    }
}

putchar (l, c) {
    *l = c;
}
