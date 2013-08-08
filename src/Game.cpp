/*!
 * \file Game.cpp
 * \brief Definitions for anything in Game.h
 */
#include "Game.h"
#include <thread>
#include "Timer.hpp"
#include "GameStates/MenuGameState.h"
namespace pong
{
  /*!
   * \brief The game begins and ends within this function.
   */
  int Game::run(int, char*[])
  {
    //Load up GameSettings
    GameSettings settings =
                          loadSettings("/home/luke/.ultimate_pong/config.json");

    //Initialize Components!
    if(!this->initializeSDL(settings.width, settings.height)) { return 1; }
    this->font_renderer_ = settings.font_renderer.get();
    Timer<> fps_timer;

    //Set up our initial GameState: The menu.
    this->pushGameState(std::unique_ptr<GameState>(new MenuGameState(this)));

    //We *need* game states, and we *need* to be running
    while(!this->game_state_stack_.empty() && this->running_)
    {
      //Make sure the object stays alive!
      std::shared_ptr<GameState> game_state = this->game_state_stack_.top();

      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
        game_state->handleEvent(event);
      }

      if(fps_timer.hasBeen(std::chrono::milliseconds(10)))
      {
        game_state->update();

        SDL_FillRect(this->main_surface_, NULL,
                     SDL_MapRGB(this->main_surface_->format, 0x00, 0x00, 0x00));
        game_state->render(this->main_surface_);
        SDL_Flip(this->main_surface_);

        fps_timer.reset();

        //We have nothing to for a bit, though leave 5 milliseconds for
        //handling events, after that it's show (render) time!
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
      }
    }

    this->uninitializeSDL();
    return 0;
  }

  /*!
   * \brief Initializes SDL, the window, the surface, etc...
   *
   * If this function returns false, all memory that could be has been freed.
   * It's time to bail out!
   */
  bool Game::initializeSDL(unsigned int width, unsigned int height)
  {
    if(SDL_Init(SDL_INIT_EVERYTHING |
                SDL_INIT_NOPARACHUTE) < 0)
    {
      return false;
    }
    this->main_surface_ = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE |
                                                              SDL_DOUBLEBUF);
    if(!this->main_surface_)
    {
      SDL_Quit();
      return false;
    }
    return true;
  }

  /*!
   * \brief Cleans up SDL normally, as if everything had went fine during
   * initialization.
   */
  void Game::uninitializeSDL()
  {
    SDL_FreeSurface(this->main_surface_);
    SDL_Quit();
  }

  /*!
   * \brief Push the game state to the top of the stack, it will become
   * active starting at the next game loop.
   *
   * The unique pointer should have been `std::move`d into this function, since
   * the Game wants sole ownership.
   */
  void Game::pushGameState(std::unique_ptr<GameState> gamestate)
  {
    this->game_state_stack_.push(std::move(gamestate));
  }
  /*!
   * \brief Remove the top game state from the stack.
   */
  void Game::popGameState()
  {
    this->game_state_stack_.pop();
  }
};

/*!
 * \brief The main function.
 *
 * Basically a wrapper of pong::Game::run.
 */
int main(int argc, char* argv[])
{
  pong::Game game;
  return game.run(argc, argv);
}
