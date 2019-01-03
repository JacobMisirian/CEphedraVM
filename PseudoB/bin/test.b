main () {
    for (auto c = 0; c < 10; c = c + 1) {
        if (c > 3) {
            break;
        }
    }
    putchar (4000, 'a' + c);
}

putchar (loc, c) {
    *loc = c;
}
