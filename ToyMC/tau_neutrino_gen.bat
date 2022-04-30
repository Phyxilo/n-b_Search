goto new

:n_depend

root -b -q tau_neutrino_gen.C(400,2.0,0.8)
root -b -q tau_neutrino_gen.C(400,2.5,0.8)
root -b -q tau_neutrino_gen.C(400,3.0,0.8)
root -b -q tau_neutrino_gen.C(400,3.5,0.8)
root -b -q tau_neutrino_gen.C(400,4.0,0.8)
root -b -q tau_neutrino_gen.C(400,4.5,0.8)
root -b -q tau_neutrino_gen.C(400,5.0,0.8)
root -b -q tau_neutrino_gen.C(400,5.5,0.8)
root -b -q tau_neutrino_gen.C(400,6.0,0.8)

:new
root -b -q tau_neutrino_gen.C(400,2.0,1.0)
root -b -q tau_neutrino_gen.C(400,2.5,1.0)
root -b -q tau_neutrino_gen.C(400,3.0,1.0)
root -b -q tau_neutrino_gen.C(400,3.5,1.0)
root -b -q tau_neutrino_gen.C(400,4.0,1.0)
root -b -q tau_neutrino_gen.C(400,4.5,1.0)
root -b -q tau_neutrino_gen.C(400,5.0,1.0)
root -b -q tau_neutrino_gen.C(400,5.5,1.0)
root -b -q tau_neutrino_gen.C(400,6.0,1.0)

root -b -q tau_neutrino_gen.C(400,2.0,0.6)
root -b -q tau_neutrino_gen.C(400,2.5,0.6)
root -b -q tau_neutrino_gen.C(400,3.0,0.6)
root -b -q tau_neutrino_gen.C(400,3.5,0.6)
root -b -q tau_neutrino_gen.C(400,4.0,0.6)
root -b -q tau_neutrino_gen.C(400,4.5,0.6)
root -b -q tau_neutrino_gen.C(400,5.0,0.6)
root -b -q tau_neutrino_gen.C(400,5.5,0.6)
root -b -q tau_neutrino_gen.C(400,6.0,0.6)

goto end


:b_depend_n2
root -b -q tau_neutrino_gen.C(400,2.0,0.4)
root -b -q tau_neutrino_gen.C(400,2.0,0.6)
root -b -q tau_neutrino_gen.C(400,2.0,1.0)
root -b -q tau_neutrino_gen.C(400,2.0,1.2)
root -b -q tau_neutrino_gen.C(400,2.0,1.4)
root -b -q tau_neutrino_gen.C(400,2.0,1.6)

:b_depend_n2.5
root -b -q tau_neutrino_gen.C(400,2.5,0.4)
root -b -q tau_neutrino_gen.C(400,2.5,0.6)
root -b -q tau_neutrino_gen.C(400,2.5,1.0)
root -b -q tau_neutrino_gen.C(400,2.5,1.2)
root -b -q tau_neutrino_gen.C(400,2.5,1.4)
root -b -q tau_neutrino_gen.C(400,2.5,1.6)

:b_depend_n3
root -b -q tau_neutrino_gen.C(400,3.0,0.4)
root -b -q tau_neutrino_gen.C(400,3.0,0.6)
root -b -q tau_neutrino_gen.C(400,3.0,1.0)
root -b -q tau_neutrino_gen.C(400,3.0,1.2)
root -b -q tau_neutrino_gen.C(400,3.0,1.4)
root -b -q tau_neutrino_gen.C(400,3.0,1.6)

:b_depend_n3.5
root -b -q tau_neutrino_gen.C(400,3.5,0.4)
root -b -q tau_neutrino_gen.C(400,3.5,0.6)
root -b -q tau_neutrino_gen.C(400,3.5,1.0)
root -b -q tau_neutrino_gen.C(400,3.5,1.2)
root -b -q tau_neutrino_gen.C(400,3.5,1.4)
root -b -q tau_neutrino_gen.C(400,3.5,1.6)

:b_depend_n4
root -b -q tau_neutrino_gen.C(400,4.0,0.4)
root -b -q tau_neutrino_gen.C(400,4.0,0.6)
root -b -q tau_neutrino_gen.C(400,4.0,1.0)
root -b -q tau_neutrino_gen.C(400,4.0,1.2)
root -b -q tau_neutrino_gen.C(400,4.0,1.4)
root -b -q tau_neutrino_gen.C(400,4.0,1.6)

:b_depend_n4.5
root -b -q tau_neutrino_gen.C(400,4.5,0.4)
root -b -q tau_neutrino_gen.C(400,4.5,0.6)
root -b -q tau_neutrino_gen.C(400,4.5,1.0)
root -b -q tau_neutrino_gen.C(400,4.5,1.2)
root -b -q tau_neutrino_gen.C(400,4.5,1.4)
root -b -q tau_neutrino_gen.C(400,4.5,1.6)

:b_depend_n5
root -b -q tau_neutrino_gen.C(400,5.0,0.4)
root -b -q tau_neutrino_gen.C(400,5.0,0.6)
root -b -q tau_neutrino_gen.C(400,5.0,1.0)
root -b -q tau_neutrino_gen.C(400,5.0,1.2)
root -b -q tau_neutrino_gen.C(400,5.0,1.4)
root -b -q tau_neutrino_gen.C(400,5.0,1.6)

:end
