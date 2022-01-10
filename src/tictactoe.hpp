// 5. Import the eosio base library.
#include <eosio/eosio.hpp>

//6. use the eosio namespace
using namespace eosio;

// 7. Declare the class. 8. Use the [[eosio::contract(contract_name)]] attribute. 9. Inherit from the base class. 
class[[eosio::contract("tictactoe")]] tictactoe : public contract
{
public:
    
    // 10 Introduce base class members. 
    using contract::contract;
    
    // 11. Use the base class constructor.
    tictactoe(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

    struct [[eosio::table]] game
    {
        static constexpr uint16_t boardWidth = 3;
        static constexpr uint16_t boardHeight = boardWidth;
        
        game() : board(boardWidth * boardHeight, 0){}

        name challenger, host, turn, winner; // = account name of host, challenger and turn to store whose turn it is.
         // = none/ draw/ name of host/ name of challenger

        std::vector<uint8_t> board;

        // Initialize the board with empty cell
        void initializeBoard(){
            board.assign(boardWidth * boardHeight, 0);
        }

        // Reset game
        void resetGame(){
            initializeBoard();
            turn = host;
            winner = "none"_n;
        }

        // 13. primary key accessor
		auto primary_key() const { return challenger.value; }

        //14. EOSLIB_SERIALIZE macro defining how the abi serializes / deserializes  
		EOSLIB_SERIALIZE( game, (challenger)(host)(turn)(winner)(board))
    };

    typedef eosio::multi_index<"games"_n, game> games;


    [[eosio::action]]
    void create(const name &challenger, name &host);
    
    [[eosio::action]]
    void restart(const name &challenger, const name &host, const name &by);

    [[eosio::action]]
    void close(const name &challenger, const name &host);
    
    [[eosio::action]]   
    void move(const name &challenger, const name &host, const name &by, const uint16_t &row, const uint16_t &column);

    private:
        bool isEmptyCell(const uint8_t &cell);
        bool isValidMove(const uint16_t &row, const uint16_t &column, const std::vector<uint8_t> &board);
        name getWinner(const game &currentGame);

};