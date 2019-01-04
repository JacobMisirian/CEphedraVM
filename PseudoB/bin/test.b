main () {
    putchar (4000, addd (97, 2));
}

addd (x, y) {
    return x + y;
}

putchar (loc, c) {
    *loc = c;
}
