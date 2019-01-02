main () {
    if (3 < 4) {
        putchar (4001, 101);
    }
    else {
        putchar (4002, 98);
    }
}

putchar (l, c) {
    *l = c;
}
