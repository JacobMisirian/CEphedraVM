main () {
    auto a = "HEllo, World!";
    putchar (4000, *a);
    putchar (4005, *(a + 1));
}

putchar (loc, c) {
    *loc = c;
}
