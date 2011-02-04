this = {}

--bertil = Create_entity()
--Entity_set_position(bertil, 100, 30)
--Entity_set_script(bertil, "UPDATE", "Bertil_update")
--Entity_set_animations(bertil, "media/Gryphon_fly.adf")

castor = Create_entity()
Entity_set_position(castor, 150, 100)
Entity_set_script(castor, "UPDATE", "Castor_update")
Entity_set_animations(castor, "media/Gryphon_fly.adf")

--Items you can pick up, can pick up all at once
pickmeup = Create_entity()
Entity_set_position(pickmeup, 48, 48)
Entity_set_script(pickmeup, "PICK_UP", "Standard_pick_up")

pickmeup = Create_entity()
Entity_set_position(pickmeup, 48, 80)
Entity_set_script(pickmeup, "PICK_UP", "Standard_pick_up")

--Entities that gives you an item on interaction, can only interact with one at a time
interact = Create_entity()
Entity_set_position(interact, 112, 48)
Entity_set_script(interact, "INTERACT", "Endless_supply")

interact = Create_entity()
Entity_set_position(interact, 112, 80)
Entity_set_script(interact, "INTERACT", "Endless_supply")

function Bertil_update(instance, dt)
	if this[instance] == nil then
		this[instance] = {}
		this[instance].dt = 0
	end

	this[instance].dt = this[instance].dt + dt
	
	if this[instance].dt >= 1 then
		this[instance].dt = this[instance].dt -1

		x,y = Entity_get_position(instance)
		x = x+math.random()*20-10
		y = y+math.random()*20-10
		if x<100 then
		  x = 200
		end
		if y<100 then
		  y = 200
		end
		if x>200 then
		  x = 100
		end
		if y>200 then
		  y = 100
		end
		Entity_set_position(instance, x, y)
	end
end

function Castor_update(instance, dt)
	if this[instance] == nil then
		this[instance] = {}
		this[instance].dt = 0
	end

	this[instance].dt = this[instance].dt + dt
	
	if this[instance].dt >= 1 then
		this[instance].dt = this[instance].dt -1

		x = math.random(-1, 1)
		y = math.random(-1, 1)
		Entity_set_movement(instance, x, y)
	end
end

function Standard_pick_up(instance, container)
	Transfer_entity(instance, container);
end

function Endless_supply(instance, container)
	soda = Create_entity();
	Transfer_entity(soda, container);
end
