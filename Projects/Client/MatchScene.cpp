#include "MatchScene.h"
#include "Stage.h"
#include "TextureManager.h"
#include "Fonts.h"
#include "Colors.h"
#include "BattleshipsRCP.h"
#include <time.h>

const std::string enter_message = "Enter Message: ";

/* Local Play Constructor */
MatchScene::MatchScene(MessageSender *ms) :
message_sender(ms),
is_local_play(true),
currentPhase(Enums::MatchPhase::GAMEPLAY),
real_side(Player::PLAYER_ONE),
view_side(Player::PLAYER_ONE),
turn_of(Player::PLAYER_ONE),
is_action_overlay_up(false),
is_meta_menu_up(false),
need_more_input(false),
is_game_over(false),
save_game_name("No Name"),
num_saved_games_shown(7),
saved_game_cursor(0),
gs(std::shared_ptr<GameState>(new GameState()))
{
  gs->set_initial_state();
  gs->set_new_reef_config();
  save_game.set_texture(std::unique_ptr<Texture>(new Texture("Save Game", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  save_game.set_position_by_screen_factor(0.5, 0.45);
  load_game.set_texture(std::unique_ptr<Texture>(new Texture("Load Game", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  load_game.set_position_by_screen_factor(0.5, 0.55);
  quit.set_texture(std::unique_ptr<Texture>(new Texture("Quit", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  quit.set_position_by_screen_factor(0.5, 0.65);
};

/* Online Play Constructor */
MatchScene::MatchScene(MessageSender *ms, Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp) :
message_sender(ms),
turn_of(turn),
view_side( user_type),
real_side( user_type ),
is_action_overlay_up(false),
is_meta_menu_up(false),
need_more_input(false),
is_local_play(false),
currentPhase(mp),
gs(std::move(g)),
is_game_over(false),
num_saved_games_shown(5),
saved_game_cursor(0),
save_game_name("No Name")
{
  reef_accept.set_texture(std::unique_ptr<Texture>(new Texture("Accept Reef?", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  reef_accept.set_position_by_screen_factor(0.5, 0.1);
  reef_reject.set_texture(std::unique_ptr<Texture>(new Texture("Reject Reef?", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  reef_reject.set_position_by_screen_factor(0.5, 0.3);
  ship_setup_done.set_texture(std::unique_ptr<Texture>(new Texture("Done ship setup?", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  ship_setup_done.set_position_by_screen_factor(0.5, 0.2);
  save_game.set_texture(std::unique_ptr<Texture>(new Texture("Save Game", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  save_game.set_position_by_screen_factor(0.5, 0.45);
  load_game.set_texture(std::unique_ptr<Texture>(new Texture("Load Game", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  load_game.set_position_by_screen_factor(0.5, 0.55);
  quit.set_texture(std::unique_ptr<Texture>(new Texture("Quit", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  quit.set_position_by_screen_factor(0.5, 0.65);
};

MatchScene::~MatchScene()
{};

void MatchScene::handle_event_for_all(SDL_Event& e)
{
  if ( is_typing && e.type == SDL_TEXTINPUT )
  {
    /* Add new text onto the end of our text */
    chat_message += e.text.text;
    chat_texture.load_from_rendered_text(chat_message, Fonts::get_instance()->MESSAGE_FONT, Colors::BLACK);
    chat_texture.set_position_by_screen_factor(0.5, 0.5);
  }
  if ( e.type == SDL_KEYDOWN )
  {
    switch ( e.key.keysym.sym )
    {
    case SDLK_ESCAPE:
      if ( !is_game_over )
      {
        is_meta_menu_up = !is_meta_menu_up;
        is_action_overlay_up = false;
        if ( are_saved_matches_up )
        {
          are_saved_matches_up = false;
        }
      }
      break;
    case SDLK_BACKSPACE:
      if ( is_typing )
      {
        if ( chat_message.size() > enter_message.size() )
        {
          chat_message = chat_message.substr(0, chat_message.size() - 1);
          chat_texture.load_from_rendered_text(chat_message, Fonts::get_instance()->MESSAGE_FONT, Colors::BLACK);
          chat_texture.set_position_by_screen_factor(0.5, 0.5);
        }
      }
      else
      {
        need_more_input = false;
      }
      break;
    case SDLK_RETURN:
      std::string actual_chat_message = "";
      if ( is_typing )
      {
        actual_chat_message = chat_message.substr(enter_message.size(), chat_message.size() - enter_message.size());
      }
      if ( is_typing && actual_chat_message != "" )
      {
        if ( actual_chat_message.size() > 3 && actual_chat_message.substr(0, 3).compare("#s ") == 0 )
        {
          int x = 0;
          save_game_name = actual_chat_message.substr(3, actual_chat_message.size());
          size_t p = save_game_name.find_first_not_of(" \t");
          save_game_name.erase(0, p);
          p = save_game_name.find_last_not_of(" \t");
          if ( std::string::npos != p )
          {
            save_game_name.erase(p + 1);
          }
          if ( save_game_name.empty() )
          {
            save_game_name = "No Name";
          }

        }
        else
        {
          BattleshipsRCP rcp(message_sender);
          rcp.send_chat(actual_chat_message);
        }
        chat_message = "";
        chat_texture.load_from_rendered_text(" ", Fonts::get_instance()->MESSAGE_FONT, Colors::BLACK);
        chat_texture.set_position_by_screen_factor(0.5, 0.5);
      }
      if ( is_typing )
      {
        chat_message = "";
        SDL_StopTextInput();
      }
      else
      {
        chat_message += enter_message;
        chat_texture.load_from_rendered_text(chat_message, Fonts::get_instance()->MESSAGE_FONT, Colors::BLACK);
        chat_texture.set_position_by_screen_factor(0.5, 0.5);
        SDL_StartTextInput();
      }
      is_typing = !is_typing;
      break;
    }
    switch ( e.key.keysym.scancode )
    {
      case SDL_SCANCODE_DOWN:
        saved_game_cursor = saved_game_cursor + num_saved_games_shown < saved_matches.size() ? saved_game_cursor + num_saved_games_shown : saved_game_cursor;
        do_render();
        break;
      case SDL_SCANCODE_UP:
        saved_game_cursor = saved_game_cursor - num_saved_games_shown >= 0 ? saved_game_cursor - num_saved_games_shown : saved_game_cursor;
        do_render();
        break;
    }
  }
  else if ( e.type == SDL_QUIT )
  {
    BattleshipsRCP rcp(message_sender);
    rcp.quit_game();
    Stage::get_instance()->quit_game();
  }

  if ( are_saved_matches_up )
  {
    bool needs_cleanup = false;
    for ( auto& m : saved_matches )
    {
      m->handle_event(e);
      if ( m->has_been_clicked() && turn_of == real_side )
      {
        BattleshipsRCP rcp(message_sender);
        rcp.load_game(m->get_game_id());
        is_loading_match = true;
        needs_cleanup = true;
      }
    }
    if ( needs_cleanup )
    {
      saved_matches.clear();
      are_saved_matches_up = false;
    }
  }

  if ( is_meta_menu_up )
  {
    if ( !is_local_play )
    {
      if ( real_side != Player::Side::SPECTATOR && currentPhase != Enums::MatchPhase::WAITING_FOR_OTHER_PLAYER && turn_of == real_side )
      {
        load_game.enable();
      }
      else
      {
        load_game.disable();
      }
      if ( real_side != Player::Side::SPECTATOR && currentPhase == Enums::MatchPhase::GAMEPLAY && !is_game_over )
      {
        save_game.enable();
      }
      else
      {
        save_game.disable();
      }
      save_game.handle_event(e);
      load_game.handle_event(e);
    }
    quit.handle_event(e);


    if ( save_game.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      if ( rcp.save_game(save_game_name) )
      {
        Stage::get_instance()->notify("Match saved!", 4000);
      }
      else
      {
        Stage::get_instance()->notify("Save failed!", 4000);
      }
      is_meta_menu_up = false;
    }
    else if ( load_game.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      auto matches = rcp.get_saved_games();
      if ( matches.size() == 0 )
      {
        Stage::get_instance()->notify("No saved matches with both of you as players to load!", 2000);
      }
      else
      {
        saved_matches.clear();
        saved_game_cursor = 0;
        double curr = 0.1;
        double offset = 1.0 / matches.size();
        for ( auto & m : matches )
        {
          std::shared_ptr<MatchButton> b(new  MatchButton(m.first, m.second));
          b->set_position_by_screen_factor(0.5, curr);
          curr += offset;
          saved_matches.push_back(b);
          if ( curr >= num_saved_games_shown * offset + 0.1 )
          {
            curr = 0.1;
          }
        }
        Stage::get_instance()->notify("Pick the match you wish to load.", 4000);
        Stage::get_instance()->notify("Use the UP and DOWN arrows to navigate through your games.", 8000);
        Stage::get_instance()->notify("Press Escape to return to the metamenu.", 8000);
        are_saved_matches_up = true;
      }
      is_meta_menu_up = false;
    }
    else if ( quit.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      rcp.quit_game();
      if ( real_side == Player::Side::SPECTATOR )
      {
        Stage::get_instance()->pop_scene();
      }
      is_meta_menu_up = false;
    }
  }
}
void MatchScene::handle_event_for_spectator(SDL_Event& e)
{
  if ( e.type == SDL_KEYDOWN && !is_meta_menu_up )
  {
    if ( e.key.keysym.sym == SDLK_F1 )
    {
      toggle_side();
    }
  }
}

void MatchScene::handle_event_for_reef_generation(SDL_Event& e)
{
  reef_accept.handle_event(e);
  reef_reject.handle_event(e);
  if ( !has_made_decision_new_reef )
  {
    if ( reef_accept.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      rcp.submit_reef_configuration_decision(true);
      has_made_decision_new_reef = true;
    }
    else if ( reef_reject.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      rcp.submit_reef_configuration_decision(false);
      has_made_decision_new_reef = true;
    }
  }
}
void MatchScene::handle_event_for_ship_setup(SDL_Event& e)
{
  ship_setup_done.handle_event(e);
  if ( !has_finished_setting_up_ships )
  {
    if ( ship_setup_done.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      rcp.submit_prep_phase_done();
      has_finished_setting_up_ships = true;
    }
    else if ( e.button.button == SDL_BUTTON_LEFT )
    {
      const Unit * un = get_selected_unit_by_click(e.button.x, e.button.y);
      if ( un != nullptr )
      {
        set_selected_unit(un);
      }
    }
    else if ( e.button.button == SDL_BUTTON_RIGHT )
    {
      if ( drawer.get_selected_unit() != nullptr )
      {
        Command cmd;
        cmd.action = Action::Action_enum::SWAP_BOATS;
        if ( drawer.is_unit_selected() )
        {
          cmd.id = drawer.get_selected_unit()->get_id();
        }
        SDL_Point p = Stage::get_instance()->get_screen()->get_grid_location_from_pixel_location(e.button.x, e.button.y);
        cmd.pt = Point(p.x, p.y);
        try_command(cmd);
      }
    }
  }
}
void MatchScene::handle_event_for_gameplay(SDL_Event& e)
{
  if ( turn_of == real_side )
  {
    action_overlay.handle_event(e);
    if ( e.type == SDL_MOUSEBUTTONDOWN && !is_meta_menu_up )
    {
      if ( e.button.button == SDL_BUTTON_RIGHT )
      {
        is_action_overlay_up = true;
        action_overlay.popup_at(e.button.x, e.button.y);
      }
      else if ( e.button.button == SDL_BUTTON_LEFT && is_action_overlay_up && action_overlay.is_action_result_valid() )
      {
        is_action_overlay_up = false;
        selected_action = action_overlay.get_handle_event_action_result();
        std::string msg = "";
        std::string help = "";
        need_more_input = false;
        switch ( selected_action )
        {
        case Action::Action_enum::FIRE_CANNON:
          msg = "Cannon"; need_more_input = true;
          help = "Please click on the spot to shoot at.";
          break; //must select spot to fire onto
        case Action::Action_enum::FIRE_TORPEDO:
          msg = "Torpedo";
          help = "Fired torpedo."; break;
        case Action::Action_enum::TOGGLE_SECONDARY_RADAR:
          msg = "Toggle Radar"; break;
        case Action::Action_enum::LAY_MINE:
          msg = "Mine Drop"; need_more_input = true;
          help = "Please click on the spot to place the Mine.";
          break; //must select spot to lay mine
        case Action::Action_enum::PICK_UP_MINE:
          msg = "Mine Pickup"; need_more_input = true;
          help = "Please click on the Mine to pickup.";
          break; //must choose mine to pickup
        case Action::Action_enum::KAMIKAZE_MOVE:
          msg = "Kamikaze Move";  need_more_input = true;
          help = "Please click on spot to move to.";
          break;
        case Action::Action_enum::EXPLODE:
          msg = "Explode"; need_more_input = true;
          help = "Please click on spot to move to and explode on.";
          break;
        case Action::Action_enum::MOVE_FORWARDS:
          msg = "Move Forwards"; need_more_input = true;
          help = "Please click on the spot where the bow should be.";
          break; //must choose distance to move
        case Action::Action_enum::MOVE_BACKWARDS:
          msg = "Move Backwards"; break;
        case Action::Action_enum::TURN_LEFT:
          msg = "Turn Left"; break;
        case Action::Action_enum::TURN_RIGHT:
          msg = "Turn Right"; break;
        case Action::Action_enum::TURN_BACK:
          msg = "Turn Back"; break;
        case Action::Action_enum::SLIDE_LEFT:
          msg = "Slide Left"; break;
        case Action::Action_enum::SLIDE_RIGHT:
          msg = "Slide Right"; break;
        case Action::Action_enum::REPAIR:
          msg = "Repair"; need_more_input = true;
          help = "Please click on the boat to repair.";
          break; //must choose ship to repair
        }
        Stage::get_instance()->notify("Selected: " + msg, 2000);
        if ( need_more_input )
        {
          Stage::get_instance()->notify(help, 3000);
        }
        else
        {
          Command cmd;
          cmd.action = selected_action;
          if ( drawer.is_unit_selected() )
          {
            cmd.id = drawer.get_selected_unit()->get_id();
          }
          switch ( selected_action )
          {
          case Action::Action_enum::TOGGLE_SECONDARY_RADAR:
            break;
          case Action::Action_enum::MOVE_BACKWARDS:
            cmd.count = 1;  break;
          case Action::Action_enum::TURN_LEFT:
            cmd.rel_dir = RelativeDirection::LEFT;
            cmd.count = 1;
            break;
          case Action::Action_enum::TURN_RIGHT:
            cmd.count = 1;
            cmd.rel_dir = RelativeDirection::RIGHT; break;
          case Action::Action_enum::TURN_BACK:
            cmd.count = 2;
            cmd.rel_dir = RelativeDirection::LEFT;
          case Action::Action_enum::SLIDE_LEFT:
            cmd.rel_dir = RelativeDirection::LEFT; break;
          case Action::Action_enum::SLIDE_RIGHT:
            cmd.rel_dir = RelativeDirection::RIGHT; break;
          case Action::Action_enum::FIRE_TORPEDO:
            srand(time(NULL));
            cmd.random = rand();
            break;
          }
          try_command(cmd);
        }
      }
      else if ( e.button.button == SDL_BUTTON_LEFT && need_more_input )
      {
        SDL_Point sdlP = Stage::get_instance()->get_screen()->get_grid_location_from_pixel_location(e.button.x, e.button.y);
        Point p(sdlP.x, sdlP.y);
        Command cmd;
        cmd.action = selected_action;
        if ( drawer.is_unit_selected() )
        {
          cmd.id = drawer.get_selected_unit()->get_id();
        }
        switch ( selected_action )
        {
        case Action::Action_enum::KAMIKAZE_MOVE:
          cmd.pt = p; break; //must select spot to move to
        case Action::Action_enum::EXPLODE:
          cmd.pt = p; break; //must select spot to move to and explode on
        case Action::Action_enum::FIRE_CANNON:
          cmd.pt = p; break; //must select spot to fire onto
        case Action::Action_enum::LAY_MINE:
          cmd.pt = p; break; //must select spot to lay mine
        case Action::Action_enum::PICK_UP_MINE:
          cmd.pt = p; break; //must choose mine to pickup
        case Action::Action_enum::MOVE_FORWARDS:
          cmd.pt = p; break; //must choose distance to move
        case Action::Action_enum::REPAIR:
          cmd.pt = p; break;
        }
        try_command(cmd);
        need_more_input = false;
      }
      else if ( e.button.button == SDL_BUTTON_LEFT )
      {
        need_more_input = false;
        const Unit * un = get_selected_unit_by_click(e.button.x, e.button.y);
        if ( un != nullptr )
        {
          set_selected_unit(un);
        }
      }
    }
  }
}

const Unit * MatchScene::get_selected_unit_by_click(const int x, const int y) const
{
  SDL_Point sdlP = Stage::get_instance()->get_screen()->get_grid_location_from_pixel_location(x, y);
  Point p(sdlP.x, sdlP.y);
  if ( real_side == Player::Side::PLAYER_ONE )
  {
    for ( auto b : gs->get_player_one().units )
    {
      if ( b->contains_point(p) && !b->is_sunk() )
      {
        return b.get();
      }
    }
    std::shared_ptr<Unit> home = gs->get_player_one().home_base;
    if ( currentPhase == Enums::MatchPhase::GAMEPLAY && home->contains_point(p) && !home->is_sunk() )
    {
      return home.get();
    }
  }
  else if ( real_side == Player::Side::PLAYER_TWO )
  {
    for ( auto b : gs->get_player_two().units )
    {
      if ( b->contains_point(p) && !b->is_sunk() )
      {
        return b.get();
      }
    }
    std::shared_ptr<Unit> home = gs->get_player_two().home_base;
    if ( currentPhase == Enums::MatchPhase::GAMEPLAY && home->contains_point(p) && !home->is_sunk() )
    {
      return home.get();
    }
  }
  return nullptr;
}

void MatchScene::do_handle_event(SDL_Event& e)
{
  handle_event_for_all(e);
  if ( is_game_over || is_loading_match )
  {
    return;
  }
  if ( real_side == Player::Side::SPECTATOR )
  {
    handle_event_for_spectator(e);
  }
  else if ( currentPhase == Enums::WAITING_FOR_OTHER_PLAYER )
  {
    //do nothing
  }
  else if ( currentPhase == Enums::REEF_GENERATION )
  {
    handle_event_for_reef_generation(e);
  }
  else if ( currentPhase == Enums::SHIP_SETUP )
  {
    handle_event_for_ship_setup(e);
  }
  else if ( currentPhase == Enums::GAMEPLAY )
  {
    handle_event_for_gameplay(e);
  }
};

void MatchScene::toggle_side()
{
  if ( view_side == Player::Side::PLAYER_ONE )
  {
    view_side = Player::Side::PLAYER_TWO;
    Stage::get_instance()->notify("Player Two's View", 2000, Colors::RED);
  }
  else if ( view_side == Player::Side::PLAYER_TWO )
  {
    view_side = Player::Side::SPECTATOR;
    Stage::get_instance()->notify("Full Map View", 2000, Colors::RED);
  }
  else if ( view_side == Player::Side::SPECTATOR )
  {
    view_side = Player::Side::PLAYER_ONE;
    Stage::get_instance()->notify("Player One's View", 2000, Colors::RED);
  }
}

void MatchScene::do_deliver_command(const Command& c)
{
  do_handle_command_result(gs->deliver_command(c), true);
  if ( c.action != Action::Action_enum::SWAP_BOATS )
  {
    toggle_turn();
  }
};

void MatchScene::do_deliver_new_reefs(std::vector<Reef> reefs)
{
  gs->set_reef_config(reefs);
  has_made_decision_new_reef = false;
  Stage::get_instance()->notify("New reef acquired!", 5000);
}
void MatchScene::do_deliver_reef_phase_over()
{
  Stage::get_instance()->notify("Reef generation phase over! Ship setup begins!", 5000);
  Stage::get_instance()->notify("Left click to select a ship.", 10000);
  Stage::get_instance()->notify("Right click another boat to swap their positions.", 10000);
  currentPhase = Enums::MatchPhase::SHIP_SETUP;
}
void MatchScene::do_deliver_ship_setup_phase_over()
{
  Stage::get_instance()->notify("Ship setup phase over! Match started!", 5000);
  Stage::get_instance()->notify("Use left click to select a unit!", 10000);
  Stage::get_instance()->notify("Use right click to select an action!", 10000);
  Stage::get_instance()->notify("Use backspace to cancel an action!", 10000);
  currentPhase = Enums::MatchPhase::GAMEPLAY;
}
void MatchScene::do_deliver_notify_other_player_joined_game(std::string description)
{
  Stage::get_instance()->notify(description + " has joined the game!", 5000);
  currentPhase = Enums::MatchPhase::REEF_GENERATION;
}
void MatchScene::do_deliver_other_player_quit_game()
{
  Stage::get_instance()->notify("Match ended because a player quit!", 5000);
  Stage::get_instance()->pop_scene();
}
void MatchScene::do_deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp)
{
  is_loading_match = false;
  is_game_over = false;
  Stage::get_instance()->notify("Match loaded!", 3000);
  turn_of = turn;
  view_side = user_type;
  real_side = user_type;
  gs = std::move(g);
  currentPhase = mp;
  reset_selected_unit();
  need_more_input = false;
  is_action_overlay_up = false;
  is_meta_menu_up = false;
}

void MatchScene::do_deliver_has_match_ended(Player::Side winner)
{
  if ( winner == real_side )
  {
    Stage::get_instance()->notify("You have won!", 10000);
  }
  else
  {
    if ( real_side == Player::Side::PLAYER_ONE || real_side == Player::Side::PLAYER_TWO )
    {
      Stage::get_instance()->notify("You have lost!", 10000);
    }
    else
    {
      Stage::get_instance()->notify("Match over", 10000);
    }
  }
  is_game_over = true;
  is_meta_menu_up = true;
}
void MatchScene::do_render()
{
  drawer.render_game_state(*gs, view_side, selected_action);
  if ( is_typing )
  {
    chat_texture.render();
  }
  if ( is_meta_menu_up )
  {
    if ( !is_local_play )
    {
      save_game.render();
      load_game.render();
    }
    quit.render();
  }
  if ( are_saved_matches_up )
  {
    int i = 0;
    for ( auto& m : saved_matches )
    {
      if ( i >= saved_game_cursor )
      {
        m->render();
      }
      i++;
      if ( i >= saved_game_cursor + num_saved_games_shown )
      {
        break;
      }
    }
  }
  if ( real_side != Player::Side::SPECTATOR && currentPhase == Enums::MatchPhase::REEF_GENERATION && !has_made_decision_new_reef )
  {
    reef_accept.render();
    reef_reject.render();
  }
  else if ( currentPhase == Enums::MatchPhase::SHIP_SETUP && !has_finished_setting_up_ships )
  {
    ship_setup_done.render();
  }
  else if ( currentPhase == Enums::MatchPhase::GAMEPLAY )
  {
    if ( need_more_input )
    {
      drawer.render_action_info(selected_action);
    }
    if ( is_action_overlay_up )
    {
      action_overlay.render();
    }
  }
};

void MatchScene::do_update()
{};

void MatchScene::do_on_enter()
{
  Stage::get_instance()->notify("Use escape to bring up the meta menu", 8000);
  if ( currentPhase == Enums::MatchPhase::WAITING_FOR_OTHER_PLAYER )
  {
    Stage::get_instance()->notify("Please wait for another player to join!", 12000);
  }
  Stage::get_instance()->notify("Press enter/return to chat (enter -> type msg -> enter).", 14000);
  if ( real_side == Player::Side::SPECTATOR )
  {
    Stage::get_instance()->notify("Press F1 to change your view of the board.", 14000);
  }
};

void MatchScene::do_on_exit()
{};

void MatchScene::try_command(const Command & cmd)
{
  CommandResult cr = gs->deliver_command(cmd);
  // Send command to server in the case it's valid.
  if ( cr.is_valid_command() )
  {
    if ( !is_local_play )
    {
      BattleshipsRCP rcp(message_sender);
      rcp.submit_command((Command &) cmd);
    }
    do_handle_command_result(cr, false);
    if ( cmd.action != Action::Action_enum::SWAP_BOATS )
    {
      toggle_turn();
      reset_selected_unit();
    }
  }
  // Invalid command, player must try again
  else
  {
    CommandResult::ErrorType type = cr.error_type;
    std::string msg = "";
    switch ( type )
    {
    case CommandResult::ErrorType::CANNOT_DROP_MINE:
      msg = "Can't drop a mine there!";
      break;
    case CommandResult::ErrorType::NO_MINES_LEFT:
      msg = "No mines left!";
      break;
    case CommandResult::ErrorType::CANNOT_MOVE:
      msg = "Can't move!";
      break;
    case CommandResult::ErrorType::INVALID_OBJECT:
      msg = "Invalid object!";
      break;
    case CommandResult::ErrorType::INVALID_ROTATE:
      msg = "Invalid rotation!";
      break;
    case CommandResult::ErrorType::NO_CANNON:
      msg = "Has no cannon!";
      break;
    case CommandResult::ErrorType::NO_MINE_DROPPER:
      msg = "No mine dropper!";
      break;
    case CommandResult::ErrorType::NO_MINE_TO_PICKUP:
      msg = "No mine to pick up!";
      break;
    case CommandResult::ErrorType::NO_OBJECT:
      msg = "No object!";
      break;
    case CommandResult::ErrorType::NO_TORPEDO:
      msg = "Has no torpedo!";
      break;
    case CommandResult::ErrorType::OUT_OF_BOUND:
      msg = "Out of bounds!";
      break;
    case CommandResult::ErrorType::OUT_OF_RANGE:
      msg = "Out of range!";
      break;
    case CommandResult::ErrorType::CANNOT_MOVE_AT_SPECIFIED_POINT:
      msg = "Cannot move at specified location!";
      break;
    case CommandResult::ErrorType::NO_PATH_EXISTS:
      msg = "No path to that point exists!";
      break;
    case CommandResult::ErrorType::UNKNOWN_COMMAND:
      msg = "Unknown Command!";
      break;
    }
    Stage::get_instance()->notify("Command Failed: " + msg, 3000);
  }
};

void MatchScene::set_selected_unit(const Unit * ut)
{
  drawer.set_selected_unit(ut);
  action_overlay.set_selected_unit(ut);
};

void MatchScene::reset_selected_unit()
{
  drawer.reset_selected_unit();
  action_overlay.reset_selected_unit();
};

void MatchScene::do_handle_command_result(const CommandResult& cr, bool is_enemy_notification)
{

  CommandResult::ResultInformation inf = cr.result_information;
  auto p = cr.result_point;
  switch ( inf )
  {
  case CommandResult::ResultInformation::COLLISION:
    for ( auto p : cr.result_collidables )
    {
      ICollidable::ICollidableType c_type = ICollidable::extract_type(p.second);
      if ( c_type == ICollidable::MINE )
      {
        auto unit = gs->get_unit_by_id(cr.id);
        //Mine Layers cannot explode a mine
        if ( unit->get_unit_type() == Unit::UnitClass::BOAT &&  dynamic_cast<Boat &>( *unit ).get_boat_type() == Boat::MINE_LAYER )
        {
          Stage::get_instance()->notify("Collided with Mine!", 4000);
        }
        else
        {
          Stage::get_instance()->notify("Mine Exploded!", 4000);
        }
      }
      else if ( c_type == ICollidable::BASE )
      {
        Stage::get_instance()->notify("Collision with Base!", 4000);
      }
      else if ( c_type == ICollidable::BOAT )
      {
        Stage::get_instance()->notify("Collision with Boat!", 4000);
      }
      else if ( c_type == ICollidable::REEF )
      {
        Stage::get_instance()->notify("Collision with Reef!", 4000);
      }
    }
    break;
  case CommandResult::ResultInformation::HIT:
    if ( cr.action == Action::Action_enum::FIRE_CANNON )
    {
      Stage::get_instance()->notify("Cannon Shell Exploded!", 4000);
    }
    else if ( cr.action == Action::Action_enum::FIRE_TORPEDO )
    {
      Stage::get_instance()->notify("Torpedo Exploded!", 4000);
    }
    for ( auto p : cr.result_collidables )
    {
      ICollidable::ICollidableType c_type = ICollidable::extract_type(p.second);
      if ( c_type == ICollidable::MINE )
      {
        Stage::get_instance()->notify("Mine Exploded!", 4000);
      }
      else if ( c_type == ICollidable::BASE )
      {
        if ( dynamic_cast<Base &>( *p.second ).is_sunk() )
        {
          Stage::get_instance()->notify("Base Destroyed!", 4000);
        }
        else
        {
          Stage::get_instance()->notify("Hit Base!", 4000);
        }
      }
      else if ( c_type == ICollidable::BOAT )
      {
        if ( dynamic_cast<Boat &>( *p.second ).is_sunk() )
        {
          std::string boat_type = Boat::convert_boat_type_to_string(dynamic_cast<Boat &>( *p.second ).get_boat_type());
          Stage::get_instance()->notify(boat_type + " Sunk!", 4000);
          Player::Side winning_side;
          if (gs->is_won(winning_side))
          {
            if (!is_local_play)
            {
              if (!is_enemy_notification)
              {
                //BattleshipsRCP rcp(message_sender);
                //rcp.won_game();
              }
            }
            else
            {
              // End play locally.
            }
          }
        }
        else
        {
          Stage::get_instance()->notify("Hit Boat!", 4000);
        }
      }
      else if ( c_type == ICollidable::REEF )
      {
        Stage::get_instance()->notify("Hit Reef!", 4000);
      }
    }
    break;
  case CommandResult::ResultInformation::MISS:
    if ( cr.action == Action::Action_enum::FIRE_CANNON )
    {
      Stage::get_instance()->notify("Miss! Shot fell in water.", 4000);
    }
    else if ( cr.action == Action::Action_enum::FIRE_TORPEDO && !is_enemy_notification )
    {
      Stage::get_instance()->notify("Miss! Torpedo did not hit anything.", 4000);
    }
    break;
  case CommandResult::ResultInformation::NO_INFORMATION:
    break;
  }
  for ( auto p : cr.result_collidables )
  {
    Stage::get_instance()->ping(p.first, 3000);
  }
}

void MatchScene::toggle_turn()
{
  if ( Player::Side::PLAYER_ONE == turn_of )
  {
    if ( is_local_play ) { view_side = real_side = Player::Side::PLAYER_TWO; }
    turn_of = Player::Side::PLAYER_TWO;
  }
  else if ( Player::Side::PLAYER_TWO == turn_of )
  {
    if ( is_local_play ) { view_side = real_side = Player::Side::PLAYER_ONE; }
    turn_of = Player::Side::PLAYER_ONE;
  }
  if ( turn_of == real_side && !is_local_play )
  {
    Stage::get_instance()->notify("Your turn", 4000, Colors::RED);
  }
}