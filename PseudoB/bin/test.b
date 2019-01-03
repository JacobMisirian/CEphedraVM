main () {
    auto c = "hello world!";
    putchar (4000, *c);
}

putchar (loc, c) {
    *loc = c;
}
