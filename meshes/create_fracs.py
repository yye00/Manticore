
# First object id
vol_id=0;
# handy commands
rotate_90_about_x = 'rotate volume {} angle 90 about x'
rotate_90_about_y = 'rotate volume {} angle 90 about y'
rotate_90_about_z = 'rotate volume {} angle 90 about z'
move_along_x_axis = 'move volume {} X {}'
move_along_y_axis = 'move volume {} Y {}'
move_along_z_axis = 'move volume {} Z {}'


# well geometry
well_diameter = 0.1
well_stage = 100
create_well = 'create cylinder height {} radius {}' # height then radius
cubit.cmd(create_well.format(well_stage, well_diameter/2))
vol_id=vol_id+1
well_id=vol_id
cubit.cmd(rotate_90_about_x.format(vol_id))
cubit.cmd(move_along_y_axis.format(vol_id,well_stage/2))


# fracture geometry
# number of fractures
frac_num = 10
# spacing between fractures
frac_space = 5
frac_ids = list()
# fracture details
frac_major = 10
frac_minor = 5
frac_thickness = 0.01
create_frac = 'create cylinder height {} major radius {} minor radius {}'

for i in range(1,frac_num):
    cubit.cmd(create_frac.format(frac_thickness,frac_major,frac_minor))
    vol_id=vol_id+1
    frac_ids.append(vol_id)
    cubit.cmd(rotate_90_about_x.format(vol_id))
    cubit.cmd(move_along_y_axis.format(vol_id,frac_space*i))




