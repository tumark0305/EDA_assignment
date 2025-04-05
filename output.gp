reset
set title "result"
set xlabel "X"
set ylabel "Y"
set object 1 rect from 3876,3684 to 10976,10284 lw 1 fs solid fc rgb "#c080ff"
set label "Attack" at 7426,6984 center
set object 2 rect from 6879,8754 to 13979,15354 lw 1 fs solid fc rgb "#c080ff"
set label "Armored" at 10429,12054 center
set object 3 rect from 13358,7182 to 20458,13782 lw 1 fs solid fc rgb "#c080ff"
set label "Colossal" at 16908,10482 center
set object 4 rect from 1069,5354 to 8169,11954 lw 1 fs solid fc rgb "#c080ff"
set label "Female" at 4619,8654 center
set object 5 rect from 7265,11842 to 14365,18442 lw 1 fs solid fc rgb "#c080ff"
set label "Beast" at 10815,15142 center
set object 6 rect from -523,-367 to 6577,6233 lw 1 fs solid fc rgb "#c080ff"
set label "Hammer" at 3027,2933 center
set object 7 rect from 2483,1483 to 9583,8083 lw 1 fs solid fc rgb "#c080ff"
set label "Jaw" at 6033,4783 center
set object 8 rect from 4257,2154 to 11357,8754 lw 1 fs solid fc rgb "#c080ff"
set label "Pure" at 7807,5454 center
set xtics 10000
set ytics 10000
set size square
set grid
plot [0:10000][0:10000]0
set terminal png size 3840,2160
set output "output.png"
replot
replot