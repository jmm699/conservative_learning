set xlabel "Iteration #"
set ylabel "log(Cost)"
set title "log(Cost) vs iteration number for various learning rates s"
set logscale y
plot '1e-1.dat' u 1:2 w lp title 's = 1e-1', \
'1e-2.dat' u 1:2 w lp title 's = 1e-2', \
'1e-3.dat' u 1:2 w lp title 's = 1e-3', \
'1e-4.dat' u 1:2 w lp title 's = 1e-4', \
'1e-5.dat' u 1:2 w lp title 's = 1e-5', \
'1e-6.dat' u 1:2 w lp title 's = 1e-6'
