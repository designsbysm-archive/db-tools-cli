type color =
  | Black
  | Blue
  | Cyan
  | Green
  | Magenta
  | Red
  | Yellow
  | White;

type style =
  | Blink
  | Bold
  | Dim
  | Hidden
  | Italic
  | Reset
  | Reverse
  | Strike
  | Underscore;

let background = (color: color) =>
  switch (color) {
  | Black => "\027[40m"
  | Blue => "\027[44m"
  | Cyan => "\027[46m"
  | Green => "\027[42m"
  | Magenta => "\027[45m"
  | Red => "\027[41m"
  | Yellow => "\027[43m"
  | White => "\027[47m"
  };

let foreground = (color: color) =>
  switch (color) {
  | Black => "\027[30m"
  | Blue => "\027[34m"
  | Cyan => "\027[36m"
  | Green => "\027[32m"
  | Magenta => "\027[35m"
  | Red => "\027[31m"
  | Yellow => "\027[33m"
  | White => "\027[37m"
  };

let style = (style: style) =>
  switch (style) {
  | Blink => "\027[5m"
  | Bold => "\027[1m"
  | Dim => "\027[2m"
  | Hidden => "\027[8m"
  | Italic => "\027[3m"
  | Reset => "\027[0m"
  | Reverse => "\027[7m"
  | Strike => "\027[9m"
  | Underscore => "\027[4m"
  };