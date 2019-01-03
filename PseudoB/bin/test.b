main () {
    auto c = 0;
    while (c < 10) {
        putchar (4000 + c, 'A' + c);
        c = c + 1;
    }
}

putchar (loc, c) {
    *loc = c;
}
