# cstring

[![license](https://img.shields.io/github/license/qlead001/cstring.svg)](LICENSE)

Simple, _safe[\-ish\*](#safety)_, minimal library for strings in c

## Table of Contents

* [About](#about)
  * [Safety](#safety)
* [License](#license)

## About

This library was created for use in a simple compiler. It is mostly
a wrapper around the standard [string.h] with some extra safety. It
adds a length and capacity to its string structure.

### Safety

Safety is not now, nor will ever be, guaranteed. For most use cases this
library is probably perfectly safe, but I make no promises.

## License

[MIT © qlead001.](LICENSE)

[string.h]: <https://pubs.opengroup.org/onlinepubs/007908799/xsh/string.h.html>
