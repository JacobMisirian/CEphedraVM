main () {
    for (auto c = 0; c < 10; c = c + 1) {
        putchar (4000 + c, 'a' + c);
    }
}

putchar (loc, c) {
    *loc = c;
}
