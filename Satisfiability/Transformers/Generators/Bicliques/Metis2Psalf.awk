#! /usr/bin/awk -f

/^%/ {printf "#%s\n", $0; next}
{ if (li == 0) printf "# %s\n", $0;
  else printf "%d %s\n", li, $0;
  ++li
}
