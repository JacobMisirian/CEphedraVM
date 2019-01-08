main () {
    if (3 != 4) {
        putchar (4000, 'a');
    }
    else {
        putchar (4000, 'b');
    }
}

putchar (loc, c) {
    *loc = c;
}
