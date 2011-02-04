this = {}

player = Get_player()
Entity_set_name(player, "I_Am_The_Player")

--x,y = Entity_get_position(player)
Entity_set_position_tile(player, 5, 7)


function Initialize_entity(instance)
	this[instance] = {}
	this[instance].dt = 0
end


function Set_attribute(instance, attribute, value)
	this[instance][attribute] = value;
end
