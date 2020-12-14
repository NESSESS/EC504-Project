gnuplot -persist <<EOF
set multiplot layout 4,3
set title "BST Insert"
plot 'data' u 1:2 w lp t "BST Insert"
set title "BST Search"
plot 'data' u 1:5 w lp t "BST Search"
set title 'BST Successor"
plot 'data' u 1:8 w lp t "BST Successor"

set title "X-fast Insert"
plot 'data' u 1:3 w lp t"Xfast Inseert"
set title "X-fast Search"
plot 'data' u 1:6 w lp t"Xfast Search"
set title "X-fast Successor"
plot 'data' u 1:9 w lp t"Xfast Successor"

set title "Yfast Insert"
plot 'data' u 1:4 w lp t "Yfast Insert"
set title "Yfast Search"
plot 'data' u 1:7 w lp t "Yfast Search"
set title "Yfast Successor"
plot 'data' u 1:10 w lp t "Yfast Successor"

EOF

