
cubit.cmd("reset")

import random

# First object id
vol_id=0;
# handy commands
# angles
rotate_ang_about_x = 'rotate volume {} angle {} about x'
rotate_ang_about_y = 'rotate volume {} angle {} about y'
rotate_ang_about_z = 'rotate volume {} angle {} about z'
# 90 degrees
rotate_90_about_x = 'rotate volume {} angle 90 about x'
rotate_90_about_y = 'rotate volume {} angle 90 about y'
rotate_90_about_z = 'rotate volume {} angle 90 about z'
# move
move_along_x_axis = 'move volume {} X {}'
move_along_y_axis = 'move volume {} Y {}'
move_along_z_axis = 'move volume {} Z {}'

def random_angle():
    return random.random()*180-90

def random_dist():
    return random.random()*35-12.5

def random_major_radius():
    return random.random()*2+4

def random_minor_radius():
    return random.random()*2+1


# fracture geometry
# number of fractures
frac_num = 20
# spacing between fractures
frac_space = 5
frac_ids = list()
# fracture details
frac_major = 10
frac_minor = 5
frac_thickness = 0.1
create_frac = 'create cylinder height {} major radius {} minor radius {}'

for i in range(1,frac_num):
    cubit.cmd(create_frac.format(frac_thickness,random_major_radius(),random_minor_radius()))
    vol_id=vol_id+1
    frac_ids.append(vol_id)
    cubit.cmd(rotate_ang_about_x.format(vol_id,random_angle()))
    cubit.cmd(rotate_ang_about_y.format(vol_id,random_angle()))
    cubit.cmd(rotate_ang_about_z.format(vol_id,random_angle()))
    cubit.cmd(move_along_x_axis.format(vol_id,random_dist()))
    cubit.cmd(move_along_y_axis.format(vol_id,random_dist()))
#    cubit.cmd(move_along_z_axis.format(vol_id,random_dist()))


cubit.cmd("unite vol all")

# well geometry
well_diameter = 0.1
well_stage = 200
create_well = 'create cylinder height {} radius {}' # height then radius
cubit.cmd(create_well.format(well_stage, well_diameter/2))
vol_id=vol_id+1
well_id=vol_id
cubit.cmd(rotate_90_about_x.format(vol_id))
#cubit.cmd(move_along_y_axis.format(vol_id,well_stage))

