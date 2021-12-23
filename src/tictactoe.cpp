#include <eosio/eosio.hpp>

#include "../include/tictactoe.hpp"

using namespace eosio;
using namespace std;

// class [[eosio:contract]]
CONTRACT tictactoe : public contract{
  public:

    using contract::contract;

    TABLE game {
      uint16_t boardWidth = 3;
      uint16_t boardHeight = 3;
        
      game() : board(boardWidth * boardHeight, 0){}

      name challenger, host, turn, winner; 

      vector<uint8_t> board;

      void initializeBoard(){
        board.assign(boardWidth * boardHeight, 0);
      }

      void resetGame(){
          initializeBoard();
          turn = host;
      }

		  auto primary_key() const { return challenger.value; }
      EOSLIB_SERIALIZE( game, (challenger)(host)(turn)(winner)(board))
    };
    typedef multi_index<"games"_n, game> games;

    // [[eosio:action]] void
    ACTION welcome(name challenger, name host) {
      print("Welcome, challengers ", host," and ", challenger, "!");
    }

    ACTION create( const name &challenger, name &host ) {
      require_auth(host);
      check(challenger != host, "challenger == host.");

      games existingHostGames(get_self(), host.value);
      auto itr = existingHostGames.find(challenger.value);
      check(itr == existingHostGames.end(), "Game already exists.");

      existingHostGames.emplace(host, [&](auto &g) {
          g.challenger = challenger;
          g.host = host;
          g.turn = host;
      });
    }

    ACTION close( const name &challenger, const name &host ) {
      check(has_auth(host), "Only the host has this action.");
      require_auth(host);

      games existingHostGames(get_self(), host.value);

      auto itr = existingHostGames.find(challenger.value);
      check(itr != existingHostGames.end(), "Game does not exist.");

      existingHostGames.erase(itr);
    }

  private:

};