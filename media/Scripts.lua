map = Get_map()
castor = Create_entity(map)
Entity_set_name(player, "Castor")
Entity_set_solid(castor, true)
Entity_set_position_tile(castor, 5, 3)
Entity_set_script(castor, "UPDATE", "Castor_update")
Entity_set_animations(castor, "media/Gryphon_fly.adf")

function Bertil_update(instance, dt)
	this[instance].dt = this[instance].dt + dt

	if this[instance].dt >= 1 then
		this[instance].dt = this[instance].dt - 1

		x,y = Entity_get_position(instance)
		x = x + math.random() * 20 - 10
		y = y + math.random() * 20 - 10
		if x < 100 then
		  x = 200
		end
		if y < 100 then
		  y = 200
		end
		if x > 200 then
		  x = 100
		end
		if y > 200 then
		  y = 100
		end
		Entity_set_position(instance, x, y)
	end
end


function Castor_update(instance, dt)
	this[instance].dt = this[instance].dt + dt

	if this[instance].dt >= 1 then
		this[instance].dt = this[instance].dt - 1

		x = math.random(-1, 1)
		y = math.random(-1, 1)
		Entity_set_movement(instance, x, y)
	end
end


function Standard_pick_up(instance, container)
	Transfer_entity(instance, container);
end


function Endless_supply(instance, container)
	Create_entity(container);
end


function Maploader(instance, container)
	if this[instance].require then
		entities = Get_container_entities(container)
		has_key = false
		for i,v in ipairs(entities) do
			if Entity_get_name(v) == this[instance].require then
				has_key = true
			end
		end
	
		if has_key == false then
			return
		end
	end
	
	destname = this[instance].dest_name
	Load_map(this[instance].map);

	-- Set the destination by specifying the name of the entity to target as the
	-- "landing" spot.
	if this[instance].dest_name then
		player = Get_player()
		Entity_set_position_name(player, destname)
		return
	end

	-- Set the destination coordinates by specifying its tile
	if this[instance].dest_tilex and this[instance].dest_tiley then
		player = Get_player()
		Entity_set_position_tile(player, this[instance].dest_tilex, this[instance].dest_tiley)
		return
	end

	-- Set the destination coordinates by specifying the absolute x/y values
	if this[instance].dest_x and this[instance].dest_y then
		player = Get_player()
		Entity_set_position(player, this[instance].dest_x, this[instance].dest_y)
		return
	end
end
