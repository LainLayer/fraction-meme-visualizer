
# About

This is a program I made to visualize this meme in OpenGL 3.3

The Y axis is treated as the top of the fraction, and the X axis as the bottom. A pixel is lit if simplifying both by one digit results in the same as `y/x`.

![the meme](./meme.webp "The Meme")

# How it looks

![screenshot](./screenshot.png)

# Compile

```console
$ cc -o main main.c src/glad.c -I./include -lX11 -lXrandr -lGL
```

# Run

```console
$ ./main
```

# Known issues

Currently this program only considers simplifying fractions by one digit. This is not intentional, I am simply
skill issue, and calculating all possible digit simplifications with multiple digits seems like an exponential
pain in the ass. If you know a cool easy way to do it, PRs welcome.

# License

CC-BY public domain
