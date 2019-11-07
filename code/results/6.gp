set xlabel "Iteration #"
set title "Behavior with s=1e-6"
set logscale y
plot '1e-6long.dat' u 1:2 w lp title 'Cost', \
'1e-6long.dat' u 1:3 w lp title 'Gradient norm', \
'1e-6long.dat' u 1:4 w lp title 'Weight distance', \
