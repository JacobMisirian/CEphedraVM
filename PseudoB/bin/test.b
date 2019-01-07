main () {
    auto a = 97;
    a += 3;
    putchar (4000, a);
}

putchar (loc, c) {
    *loc = c;
}
