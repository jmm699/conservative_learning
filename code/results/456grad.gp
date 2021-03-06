set xlabel "Iteration #"
set ylabel "log(norm(gradient))"
set title "Gradient norm vs iteration for various learning rates s"
set logscale y
plot '1e-4.dat' u 1:3 w lp title 's = 1e-4', \
'1e-5.dat' u 1:3 w lp title 's = 1e-5', \
'1e-6.dat' u 1:3 w lp title 's = 1e-6'
