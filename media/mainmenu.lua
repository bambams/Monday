function Main_menu
#Menu_create("MAIN", "Main Menu", "media/menubg.png" 
#Menu_background_pic("media/menubg.pic");
Menu_add_button("Play Game", MONDAY_ACTION_CONTINUE_GAME)
Menu_add_button("New Game", MONDAY_ACTION_NEWGAME)
Menu_add_button("Exit",.MONDAY_ACTION_EXIT)
end

function Options_menu
Menu_add_button("Screen", MONDAY_ACTION_SCREEN_CONFIG)
Menu_add_button("Keyboard", MONDAY_ACTION_KEYBOARD_CONFIG)
Menu_add_button("Joystick", MONDAY_ACTION_JOYSTICK_CONFIG)
Menu_add_button("Audio", MONDAY_ACTION_AUDIO_CONFIG)
Menu_add_button("Exit",.MONDAY_ACTION_GOTO_COMEFROM)
end

function Game_Start

end

menu = 
player = Get_player()
Player_set_position(player, 100, 200)


