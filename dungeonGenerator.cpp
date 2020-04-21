#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <string>
#include <endian.h>
#include <limits.h>
#include <unistd.h>
#include <ncurses.h>
#include <fstream>
#include <vector>


#include "heap.h"

#define hardnesspair(pair) 1 + (hardness[pair[dim_y]][pair[dim_x]] / 85)

#define NPC_SMART   0x00000001
#define NPC_TELE    0x00000002
#define NPC_TUNNEL  0x00000004
#define NPC_ERRATIC 0x00000008
#define NPC_PASS    0x00000032
#define NPC_PICKUP  0x00000064
#define NPC_DESTROY 0x00000128
#define NPC_UNIQ    0x00000256
#define NPC_BOSS    0x00000512

#define ROCK_PAIR     1
#define OPEN_PAIR     2
#define PC_PAIR       3
#define MONSTER_PAIR  4
#define BLACK 5
#define RED 6
#define GREEN 7
#define YELLOW 8
#define BLUE 9
#define MAGENTA 10
#define CYAN 11
#define WHITE 12
#define WHITE_D 13
#define SHOP_PAIR 14

using namespace std;

string top_text = "";
string top_text_n = "";
const char *triumph =
  "\n"
  "                ,:/+/-                              ##   ###   ##       \n"
  "                /M/              .,-=;@/;-          ##   ###   ##       \n"
  "           .:/= ;MH/,    ,=/+&$XH@MM#@:             ##   ###   ##       \n"
  "          -$##@+$###@H@MMM#######H:.    -/H#        ##   ###   ##       \n"
  "     .,H@H@ X######@ -H#####@+-     -+H###@X        '## ##'## ##'       \n"
  "      .,@##H;      +XM##M/,     =&@###@X;-           '###' '###'        \n"
  "    X&-  :M##########$.    .:&M###@&:                                   \n"
  "    M##H,   +H@@@$/-.  ,;$M###@&,          -        #############       \n"
  "     M####M=,,---,.-%%H####M$:          ,+@##             ###            \n"
  "    @##################@/.         :&H##@$-              ###            \n"
  "    M###############H,         ;HM##M$=                  ###            \n"
  "    #################.    .=$M##M$=                      ###            \n"
  "    ################H..;XM##M$=          .:+        #############       \n"
  "    M###################@&=           =+@MH&                            \n"
  "    @#################M/.         =+H#X&=              ###   ##         \n"
  "    =+M###############M,      ,/X#H+:,                 ####  ##         \n"
  "      .;XM###########H=   ,/X#H+:;                     ##### ##         \n"
  "        .=+HM#######M+/+HM@+=.                         ## #####         \n"
  "          ,:/%XM####H/.                              ##  ####         \n"
  "                  ,.:=-.                               ##   ###         \n"
  "                                   You win!\n\n";

const char *ganon =
  "\n"
  "                                GAME OVER\n"
  "                             RETURN OF GANON\n"
  "                      (&,                     ,/&)\n"
  "                       #@@@@*.             *&@@@)\n"
  "                        *@@@@@.,&&@@@@@%*.%@@@&\n"
  "                          .*&@@@@@@@@&(&&@@&#.\n"
  "                             .(%#   %&&   @@)*\n"
  "                  ,,**&&%%/%%@@@@%%@//\\&@@@@&&)/(&&/&,\n"   
  "               *&@@@@@#@&/@@@@@@@@//'@@'\\@@@@@@@)@#&@@@@&*\n"
  "           .,*#@@@@@@@@@(@@@@@@@@//'@@@@'\\@@@@@@@)@@@@@@@@\\.\n"
  "          *@@@@@@@@@@@@@(@@@@@@@@@@@@@@@@@@@@@@@/@@@@@@@@@@@\\,\n"
  "         (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "        /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\\n"
  "       /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@).\n"
  "      /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "    *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "   (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@),\n"
  "  /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@),\n"
  " /@@@@@@@@@@@@/@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\@@@@@@@@@@).\n"
  ",)@@@@@@@@@@/' @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\@@@@@@@@@@(\n"  
  "            You're dead.  Better luck in the next life.\n\n";

  const char *quitRLG327 =
  "\n"
  "                                                                                \n"
  "                                                                                \n"
  "                                                                                \n"
  " ,ggg,         gg                                                               \n"  
  "dP''Y8a        88                                                           I8  \n"
  "Yb, `88        88                                                           I8  \n" 
  " `'  88        88                                                    gg  8888888\n"
  "     88        88                                                    ''     I8  \n"
  "     88        88    ,ggggg, gg      gg       ,gggg,gg  gg      gg   gg     I8  \n" 
  "     88       ,88   dP'  'Y8gI8      8I      dP'  'Y8I  I8      8I   88     I8  \n" 
  "     Y8b,   ,d888  i8'    ,8II8,    ,8I     i8'    ,8I  I8,    ,8I   88    ,I8, \n"
  "      'Y88888P'88,,d8,   ,d8'd8b,  ,d8b,   ,d8,   ,d8b ,d8b,  ,d8b, ,88,  ,d88b,\n"
  "           ,ad8888P'Y8888P' 8P''Y88P'`Y8   P'Y8888P'88d8P''Y88P'`Y88P''Y888P''Y8\n"
  "          d8P' 88                                   I8P                         \n"
  "        ,d8'   88                                   I8'                         \n"
  "        d8'    88                                   I8                          \n"
  "        88     88                                   I8                          \n"
  "        Y8,   ,88                                   I8                          \n"
  "         'Y888P'                                    I8                          \n"
  "                                                                                \n"
  "                                                                                \n";
char symbols[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

int shop_loc[4];

#define has_characteristic(character, bit) (((NPC*)character)->type & NPC_##bit) 

class Dice {
public:
  int base;
  int dice;
  int sides;
  
  Dice()
  {
    base = 0;
    dice =  0;
    sides = 0;
  }
  Dice( const Dice &old)
  {
    base = old.base;
    dice = old.dice;
    sides = old.sides;
  }
 
  int roll()
  {
    int result = base;
    int i;
    for( i = 0; i < dice; i++ )
      {
	result += (rand() % sides) + 1;
      }
    return result;
  }
  string print()
  {
    return to_string(base) + "+" + to_string(dice) + "d" + to_string(sides); 
  }
};

class Obj {
public:
  string type;
  string name;
  string desc;
  vector<string> color;
  
  int speed;
  int hit;
  Dice dam_die;
  int dodge;
  int def;
  int weight;
  int attr;
  int val;

  int art; // coded artifact values -1 for not an artifact, 0 for an artifact, 1 for a used artifact
  int rarity;
  
  Obj()
  {
    def = 0;
  }
  Obj* copy()
  {
    Obj* temp = new Obj();
    temp->type = type;
    temp->name = name;
    temp->desc = desc;
    temp->color = color;
    temp->speed = speed;
    temp->hit = hit;
    temp->dam_die = dam_die;
    temp->dodge = dodge;
    temp->def = def;
    temp->weight = weight;
    temp->attr = attr;
    temp->val = val;
    temp->art = art;
    temp->rarity = rarity;
    return temp;
  }
};
class Obj_desc {
public:
  string type;
  string name;
  string desc;
  vector<string> color;
  
  Dice speed_die;
  Dice hit_die;
  Dice dam_die;
  Dice dodge_die;
  Dice def_die;
  Dice weight_die;
  Dice attr_die;
  Dice val_die;
  
  int art; // coded artifact values -1 for not an artifact, 0 for an artifact, 1 for a used artifact
  int rarity;
  
  Obj_desc()
  {
    
  }

  Obj* spawn()
  {
    Obj* item = new Obj();
    item->type = type;
    item->name = name;
    item->desc = desc;
    item->color = color;
    item->dam_die = dam_die;
    item->speed = speed_die.roll();
    item->hit = hit_die.roll();
    item->dodge = dodge_die.roll();
    item->def = def_die.roll();
    item->weight = weight_die.roll();
    item->attr = attr_die.roll();
    item->val = val_die.roll();
    item->art = art;
    item->rarity = rarity;
    
    return item;
  }
  int print()
  {
    int i;
    cout << name << endl;
    cout << "Description: " << desc << endl;
    cout << type << endl;
    
    for( i = 0; i < color.size(); i++ )
      {
	cout << color[i];
      } 
    cout << endl;
    
    cout << hit_die.base << "+" << hit_die.dice << "d" << hit_die.sides << endl;

    cout << dam_die.base << "+" << dam_die.dice << "d" << dam_die.sides << endl;

    cout << dodge_die.base << "+" << dodge_die.dice << "d" << dodge_die.sides << endl;

    cout << def_die.base << "+" << def_die.dice << "d" << def_die.sides << endl;

    cout << weight_die.base << "+" << weight_die.dice << "d" << weight_die.sides << endl;

    cout << speed_die.base << "+" << speed_die.dice << "d" << speed_die.sides << endl;

    cout << attr_die.base << "+" << attr_die.dice << "d" << attr_die.sides << endl;

    cout << val_die.base << "+" << val_die.dice << "d" << val_die.sides << endl;
    
    if( art == true )
      cout << "An Artifact" << endl;
    else
      cout << "Not an Artifact" << endl;

    cout << rarity << endl;

    cout << endl;
    return 0;
  }
};

class Character {
public:
  heap_node_t *hn;
  int exists;
  int location[2];
  int last_seen[2];
  int speed;
  int seq_num;
  int next_turn;
  char symbol;
  
  Character()
  {
    hn = NULL;
    exists = 1;
    location[0] = 0;
    location[1] = 0;
    last_seen[0] = 0;
    last_seen[1] = 0;
    speed = 0;
    seq_num = 0;
    next_turn = 0;
    symbol = 'z';
  }
  Character(int pc_loc[2])
  {
    location[0] = pc_loc[0];
    location[1] = pc_loc[1];
    speed = 10;
    exists = 1;
    seq_num = 0;
    next_turn = 0;
    symbol = '@';
  }
  Character(int x, int y, int num)
  {
    location[0] = x;
    location[1] = y;
    speed = (rand() % 16) + 5;
    exists = 1;
    seq_num = num + 1;
    next_turn = 0;
  }
};

class PC : public Character {
public:
  int isa;
  int win;
  int hp;
  int dam;
  int def;
  int base_dam;
  int base_def;
  int base_speed;
  int coins;
  vector<Obj> inventory;
  vector<Obj> equip;
  PC(int pc_loc[2]) 
    : Character( pc_loc)
  {
    isa = 1;
    win = 0;
    hp = (rand() % 101)+100;
    base_dam = (rand() % 21) + 20;
    dam = base_dam;
    def = 0;
    base_def = 0;
    base_speed = speed;
    coins = rand() % 10;
  }
};

class NPC : public Character {
public:
  int isa;
  int type;
  string name;
  string desc;
  vector<string> color;
  int speed;
  vector<string> abil;
  int hp;
  Dice dam_die;
  int rarity;
  
  
  NPC()
  {
    
  }
  NPC(int x, int y, int num)
    : Character(x, y, num)
  {
    type = rand() % 16;
    isa = 0;
  }
};
  
  
class NPC_desc {
public:
  string name;
  string desc;
  vector<string> color;
  Dice speed_die;
  vector<string> abil;
  Dice hp_die;
  Dice dam_die;
  char symbol;
  int rarity;
  int unique; // -1 for not unique, 0 for unique, 1 for unique and used
  
  NPC_desc()
  {
    unique = -1;
  }

  NPC* spawn( int x, int y, int num )
  {
    NPC* npc = new NPC( x, y, num );

    npc->name = name;
    npc->desc = desc;
    npc->color = color;
    npc->speed = speed_die.roll();
    npc->abil = abil;
    npc->hp = hp_die.roll();
    npc->dam_die = dam_die;
    npc->symbol = symbol;
    npc->rarity = rarity;
    return npc;
  }
  int print()
  {
    int i;
    cout << name << endl;
    cout << "Description: " << desc << endl;
    
    for( i = 0; i < color.size(); i++ )
      {
	cout << color[i];
      } 
    cout << endl;
    
    cout << speed_die.base << "+" << speed_die.dice << "d" << speed_die.sides << endl;
    
    for( i = 0; i < abil.size(); i++ )
      {
	cout << abil[i];
      } 
    cout << endl;
    
    cout << hp_die.base << "+" << hp_die.dice << "d" << hp_die.sides << endl;
    cout << dam_die.base << "+" << dam_die.dice << "d" << dam_die.sides << endl;
    cout << symbol << endl;
    cout << rarity << endl;

    cout << endl;
    return 0;
  }
};

class Room {
public:
  int x_pos;
  int y_pos;
  int x_length;
  int y_length;
  
  Room()
  {
    x_pos = -1;
    y_pos = -1;
    x_length = 0;
    y_length = 0;
  }
  
};

class dungeon_path {
public:
  heap_node_t *hn;
  uint8_t pos[2];
  uint8_t from[2];
  int32_t cost;
};

void io_init_terminal(void);

vector<NPC_desc> parse_monsters();

vector<Obj_desc> parse_objects();

int file_save( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], uint32_t version, uint32_t filesize );
int file_load( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], uint32_t version, uint32_t filesize );

int run_game( int hardness[21][80], vector<NPC_desc> monsters, vector<Obj_desc> items, int rooms[4][1000], int *num_rooms, int stairs[2][9], Character *characters[21][80], int pc_loc[2], int nummon, int shop_num );

int inside_shop( int rooms[4][1000], int shop_num, int x, int y );

int inside_shop( int room[4], int x, int y );

int generate( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], int shop_num );

static int32_t turn_cmp(const void *key, const void *with);

int gen_characters( int hardness[21][80], Character *characters[21][80], int rooms[4][1000], vector<NPC_desc> monsters, PC* p, int pc_loc[2], int shop_num, int nummon, heap_t *turn_h );

int gen_items( int hardness[21][80], Obj *item_map[21][80], int rooms[4][1000], vector<Obj_desc> items, int num, int shop_num);
int gen_shop( int shop_loc[4], Obj *item_map[21][80], vector<Obj_desc> items );

int inventory_menu( PC *p,  Obj *item_map[21][80], int pc_loc[2] );

int search_abil( NPC *c, string abil );

int search_equip( vector<Obj> equip,  string type );

int equip_menu( PC *p, Obj *item_map[21][80], int pc_loc[2] );

int list_monster( Character *characters[21][80], int pc_loc[2] );

int teleport_mode( char basic_map[21][80], char fog_map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] );

int look_mode( char basic_map[21][80], char fog_map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] );

int pc_view( int x, int y, int new_x, int new_y );

int update_t_map( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], char t_map[21][80] );
int update_map( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], Character *characters[21][80], char basic_map[21][80] );
int init_fog( char fog_map[21][80] );
int update_fog( char basic_map[21][80], char fog_map[21][80], char t_map[21][80], int pc_loc[2] );

int check_win( Character *characters[21][80] );

int see_pc( Character *c, int rooms[4][1000], int *num_rooms, int pc[2] );

int on_downstairs( int pc[2], int stairs[2][9] );
int on_upstairs( int pc[2], int stairs[2][9] );

int random_move_t( int hardness[21][80], Character *characters[21][80], Character *c );
int random_move( int hardness[21][80], Character *characters[21][80], Character *c );

int directed_move( int hardness[21][80], Character *characters[21][80], vector<Obj> &inventory, Obj *item_map[21][80], Character *c, int num );

int move_to_pc( int hardness[21][80], Character *characters[21][80], Character *c, int distance[21][80] );
int move_to_pc_t( int hardness[21][80], Character *characters[21][80], Character *c, int distance[21][80] );

int cpy_delete_char_t( int hardness[21][80], Character *characters[21][80], Character *c, int dest_x, int dest_y );

int cpy_delete_char_pc( int hardness[21][80], Character *characters[21][80], Obj *item_map[21][80], vector<Obj> &inventory, Character *c, int dest_x, int dest_y );

int monster_split( int hardness[21][80], Character *characters[21][80], Character *c );

int update_mods( PC *p );

int get_color( string color );

char get_symbol( string type );

int print_d( char map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] );

int initialize_area( int hardness[21][80] );
int generate_room( int rooms[4][1000] );
int corridors( int rooms[4][1000], int hardness[21][80] );
int stairsANDPC( int hardness[21][80], int stairs[2][9], int pc[2], int rooms[4][1000], int shop_num );
static void dijkstra_tunneling( int hardness[21][80], int pc[2], int distance_map[21][80]);
static void dijkstra_non_tunneling( int hardness[21][80], int pc[2], int distance_map[21][80]);

typedef enum dim {
  dim_x,
  dim_y,
  num_dims
} dim_t;

int nummon = 10;
vector<NPC_desc> monsters = parse_monsters();
vector<Obj_desc> items = parse_objects();
heap_t *turn_h = (heap_t*) malloc(sizeof(heap_t));

int main( int argc, char *argv[])
{
  srand(time(NULL)); //Seed random number
  
  int i;
  uint32_t version = 2;
  uint32_t filesize = 0;
  int hardness[21][80];
  Character *characters[21][80];
  int *num_rooms, n;
  int stairs[2][9]; //top row for upstairs bottom for downstairs. First column stores number of stairs currently max number of one type of stairs is 4 cant imagine the need for more than 4 on one floor. 
  int pc[2];
  
  int shop_num;

  int rooms[4][1000];
  //this array uses the first row for the x coordinate of the top-left corner. This will be -1 if there is no room
  //the second row is the y-coordinate
  //third row, x-length
  //fourth, y-length

  //Dungeons will have anywhere from 6-10 rooms
  //Room size will be from 4-6 in the x and 3-5 in the y
  //for now rooms are rectangular

  int load = 0;
  int save = 0;

  for( i = 0; i < argc; i++ )
    {
      if( !strcmp( argv[i], "--load" ) )
	{
	  load = 1;
	}
      if( !strcmp( argv[i], "--save" ) )
	{
	  save = 1;
	}
      if( !strcmp( argv[i], "--nummon" ) && argc >= i + 2 )
	{
	  nummon = atoi(argv[i+1]);
	}
    }
  
  if(load == 1)
    {
      n = -1;
      num_rooms = &n;
      file_load( hardness, rooms, num_rooms, stairs, pc, version, filesize );
      shop_num = rand() % *num_rooms;
    }
  if(load == 0)
    {
      n = (rand() % 6) + 5;
      num_rooms = &n;
      shop_num = rand() % *num_rooms;
      generate( hardness, rooms, num_rooms, stairs, pc, shop_num );
    }
  if(save == 1)
    {
      file_save( hardness, rooms, num_rooms, stairs, pc, version, filesize );
    }
  
  io_init_terminal();

  run_game( hardness, monsters, items, rooms, num_rooms, stairs, characters, pc, nummon, shop_num );

  return 0;
}

void io_init_terminal(void)
{
  initscr();
  raw();
  noecho();
  keypad(stdscr, TRUE);

  if (has_colors() == FALSE)
    {
      endwin();
      printf("Your terminal does not support color\n");
      }
  start_color();
  init_pair(ROCK_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(OPEN_PAIR, COLOR_BLACK, COLOR_WHITE);
  init_pair(PC_PAIR, COLOR_GREEN, COLOR_WHITE);
  init_pair(MONSTER_PAIR, COLOR_RED, COLOR_WHITE);
  init_pair(SHOP_PAIR, COLOR_WHITE, COLOR_GREEN);

  init_pair(BLACK, COLOR_BLACK, COLOR_WHITE);
  init_pair(RED, COLOR_RED, COLOR_WHITE);
  init_pair(GREEN, COLOR_GREEN, COLOR_WHITE);
  init_pair(YELLOW, COLOR_YELLOW, COLOR_WHITE);
  init_pair(BLUE, COLOR_BLUE, COLOR_WHITE);
  init_pair(MAGENTA, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(CYAN, COLOR_CYAN, COLOR_WHITE);
  init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);

  init_pair(WHITE_D, COLOR_WHITE, COLOR_BLACK);
}

vector<NPC_desc> parse_monsters()
{

  ifstream inFile;
  string line;

  char *home = getenv("HOME");
  char *path;
  path = (char*) malloc(strlen(home) + strlen("/.rlg327/monster_desc.txt") + 1);
  strcpy(path, home);
  strcat(path, "/.rlg327/monster_desc.txt");

  inFile.open(path);
  free(path);

  NPC_desc cur_monster;
  vector<NPC_desc> monsters; 
  string::size_type sz;

  Dice hitp;
  Dice spd;
  Dice dm;
  
  if( !inFile )
    {
      cerr << "Unable to open file monster_desc.txt";
      return monsters;
    }
  getline( inFile, line );
  getline( inFile, line );

  while( inFile.peek() != std::ifstream::traits_type::eof() )
    {
      getline( inFile, line );
      if( line.compare("BEGIN MONSTER") == 0 )
	{
	  getline( inFile, line );
	  
	  while( line.compare("END") != 0 )
	    {
	      if( line.substr(0, 2).compare("HP") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      hitp.base = stoi(line.substr(3, line.find("+")), &sz);
		      hitp.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      hitp.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_monster.hp_die = hitp;
		    }
		}
	      if( line.substr(0, 4).compare("NAME") == 0 )
		{
		  cur_monster.name = line.substr(5);
		}
	      if( line.substr(0, 4).compare("SYMB") == 0 )
		{
		  char symbol_s[2];
		  strcpy(symbol_s, line.substr(5).c_str());
		  cur_monster.symbol = (char) symbol_s[0];
		}
	      if( line.substr(0, 4).compare("DESC") == 0 )
		{
		  getline( inFile, line );
		  cur_monster.desc = "\n";
		  while( line.compare(".") != 0 )
		    {
		      cur_monster.desc.append(line);
		      cur_monster.desc.append("\n");
		      getline( inFile, line );
		    }
		}
	      if( line.substr(0, 3).compare("DAM") == 0 )
		{
		  dm.base = stoi(line.substr(4, line.find("+")), &sz);
		  dm.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		  dm.sides = stoi(line.substr(line.find("d") + 1), &sz);
		  cur_monster.dam_die = dm;
		}
	      if( line.substr(0, 4).compare("ABIL") == 0 )
		{
		  cur_monster.abil.clear();
		  line = line.substr(5);
		  if( line.find(" ") == (long unsigned int) -1 )
		    {
		      cur_monster.abil.push_back(line);
		      continue;
		    }
		  while( line.find(" ") != (long unsigned int) -1 )
		    {
		      cur_monster.abil.push_back(line.substr(0, line.find(" ")));
		      line = line.substr(line.find(" ") + 1);
		    }
		  cur_monster.abil.push_back(line);
		}
	      if( line.substr(0, 4).compare("RRTY") == 0 )
		{
		  cur_monster.rarity = stoi(line.substr(5), &sz);
		}
	      if( line.substr(0, 5).compare("SPEED") == 0 )
		{
		  spd.base = stoi(line.substr(6, line.find("+")), &sz);
		  spd.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		  spd.sides = stoi(line.substr(line.find("d") + 1), &sz);
		  cur_monster.speed_die = spd;
		}
	      if( line.substr(0, 5).compare("COLOR") == 0 )
		{
		  cur_monster.color.clear();
		  line = line.substr(6);
		  if( line.find(" ") == (long unsigned int) -1 )
		    {
		      cur_monster.color.push_back(line);
		      continue;
		    }
		  while( line.find(" ") != (long unsigned int)-1 )
		    {
		      cur_monster.color.push_back(line.substr(0, line.find(" ")));
		      line = line.substr(line.find(" ") + 1);
		    }
		  cur_monster.color.push_back(line);
		}
	      getline( inFile, line );
	    }
	  monsters.push_back(cur_monster);
	}
    }
  
  inFile.close();
  return monsters;
}

vector<Obj_desc> parse_objects()
{

  ifstream inFile;
  string line;

  char *home = getenv("HOME");
  char *path;
  path = (char*) malloc(strlen(home) + strlen("/.rlg327/object_desc.txt") + 1);
  strcpy(path, home);
  strcat(path, "/.rlg327/object_desc.txt");

  inFile.open(path);
  free(path);

  Obj_desc cur_obj;
  vector<Obj_desc> objects; 
  string::size_type sz;

  Dice hit;
  Dice dam;
  Dice dodge;
  Dice def;
  Dice weight;
  Dice spd;
  Dice attr;
  Dice val;
  
  if( !inFile )
    {
      cerr << "Unable to open file object_desc.txt";
      return objects;
    }
  getline( inFile, line );
  getline( inFile, line );

  while( inFile.peek() != std::ifstream::traits_type::eof() )
    {
      getline( inFile, line );
      if( line.compare("BEGIN OBJECT") == 0 )
	{
	  getline( inFile, line );
	  
	  while( line.compare("END") != 0 )
	    {
	      if( line.substr(0, 3).compare("ART") == 0 )
		{
		  if( line.substr(4).compare("TRUE") == 0)
		    cur_obj.art = 0;
		  else
		    cur_obj.art = -1;
		}
	      if( line.substr(0, 3).compare("VAL") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      val.base = stoi(line.substr(4, line.find("+")), &sz);
		      val.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      val.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.val_die = val;
		    }
		}
	      if( line.substr(0, 3).compare("HIT") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      hit.base = stoi(line.substr(4, line.find("+")), &sz);
		      hit.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      hit.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.hit_die = hit;
		    }
		}
	      if( line.substr(0, 3).compare("DEF") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      def.base = stoi(line.substr(4, line.find("+")), &sz);
		      def.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      def.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.def_die = def;
		    }
		}
	      if( line.substr(0, 3).compare("DAM") == 0 )
		{
		  dam.base = stoi(line.substr(4, line.find("+")), &sz);
		  dam.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		  dam.sides = stoi(line.substr(line.find("d") + 1), &sz);
		  cur_obj.dam_die = dam;
		}
	      if( line.substr(0, 4).compare("NAME") == 0 )
		{
		  cur_obj.name = line.substr(5);
		}
	      if( line.substr(0, 4).compare("TYPE") == 0 )
		{
		  cur_obj.type = line.substr(5);
		}
	      if( line.substr(0, 4).compare("DESC") == 0 )
		{
		  getline( inFile, line );
		  cur_obj.desc = "\n";
		  while( line.compare(".") != 0 )
		    {
		      cur_obj.desc.append(line);
		      cur_obj.desc.append("\n");
		      getline( inFile, line );
		    }
		}
	      if( line.substr(0, 4).compare("ATTR") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      attr.base = stoi(line.substr(4, line.find("+")), &sz);
		      attr.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      attr.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.attr_die = attr;
		    }
		}
	      if( line.substr(0, 4).compare("RRTY") == 0 )
		{
		  cur_obj.rarity = stoi(line.substr(5), &sz);
		}
	      if( line.substr(0, 5).compare("DODGE") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      dodge.base = stoi(line.substr(6, line.find("+")), &sz);
		      dodge.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      dodge.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.dodge_die = dodge;
		    }
		}
	      if( line.substr(0, 5).compare("SPEED") == 0 )
		{
		  spd.base = stoi(line.substr(6, line.find("+")), &sz);
		  spd.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		  spd.sides = stoi(line.substr(line.find("d") + 1), &sz);
		  cur_obj.speed_die = spd;
		}
	      if( line.substr(0, 5).compare("COLOR") == 0 )
		{
		  cur_obj.color.clear();
		  line = line.substr(5);
		  if( line.find(" ") == (long unsigned int) -1 )
		    {
		      cur_obj.color.push_back(line);
		      continue;
		    }
		  while( line.find(" ") != (long unsigned int)-1 )
		    {
		      cur_obj.color.push_back(line.substr(0, line.find(" ")));
		      line = line.substr(line.find(" ") + 1);
		    }
		  cur_obj.color.push_back(line);
		}
	      if( line.substr(0, 6).compare("WEIGHT") == 0 )
		{
		  if( line.find("+") != (long unsigned int) -1 )
		    {
		      weight.base = stoi(line.substr(7, line.find("+")), &sz);
		      weight.dice = stoi(line.substr(line.find("+") + 1, line.find("d")), &sz);
		      weight.sides = stoi(line.substr(line.find("d") + 1), &sz);
		      cur_obj.weight_die = weight;
		    }
		}
	      getline( inFile, line );
	    }
	  objects.push_back(cur_obj);
	}
    }
  
  inFile.close();
  return objects;
}

int file_load( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], uint32_t version, uint32_t filesize )
{
  char header[13] = "RLG327-S2019";
  uint8_t temp_8;
  uint16_t temp_16;
  uint32_t temp_32;
  int i, j;

  char *home = getenv("HOME");
  char *path;
  path = (char*) malloc(strlen(home) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, home);
  strcat(path, "/.rlg327/dungeon"); // "/.rlg327/dungeon"

  FILE *f;

  f = fopen(path, "r");
  fread(header, sizeof(header), 1, f);

  fread(&temp_32, sizeof(temp_32), 1, f);
  version = be32toh(temp_32);

  fread(&temp_32, sizeof(temp_32), 1, f);
  filesize = be32toh(temp_32);

  fread(&pc[0], sizeof(temp_8), 1, f);
  fread(&pc[1], sizeof(temp_8), 1, f);

  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  fread(&hardness[i][j], sizeof(temp_8), 1, f);
	  hardness[i][j] = (uint8_t) hardness[i][j];
	}
    }

  fread(&temp_16, sizeof(temp_16), 1, f);
  *num_rooms = be16toh(temp_16);
      
  for( i = 0; i < *num_rooms; i++ )
    {
      fread(&rooms[0][i], sizeof(temp_8), 1, f);
      fread(&rooms[1][i], sizeof(temp_8), 1, f);
      fread(&rooms[2][i], sizeof(temp_8), 1, f);
      fread(&rooms[3][i], sizeof(temp_8), 1, f);
    }
      
  fread(&temp_16, sizeof(temp_16), 1, f);
  stairs[0][0] = be16toh(temp_16);
      
  for( i = 1; i <= stairs[0][0] * 2; i = i + 2 )
    {
      fread(&stairs[0][i], sizeof(temp_8), 1, f);
      fread(&stairs[0][i+1], sizeof(temp_8), 1, f);
    }

  fread(&temp_16, sizeof(temp_16), 1, f);
  stairs[1][0] = be16toh(temp_16);

  for( i = 1; i <= stairs[1][0] * 2; i = i + 2 )
    {
      fread(&stairs[1][i], sizeof(temp_8), 1, f);
      fread(&stairs[1][i+1], sizeof(temp_8), 1, f);
    }

  fclose(f);
  return 0;
}

int file_save( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], uint32_t version, uint32_t filesize )
{
  char header[13] = "RLG327-S2019";
  uint8_t temp_8;
  uint16_t temp_16;
  uint32_t temp_32;
  int i, j;

  char *home = getenv("HOME");
  char *path;
  path = (char*) malloc(strlen(home) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, home);
  strcat(path, "/.rlg327/dungeon"); // "/.rlg327/dungeon"

  FILE *f;

  if( !(f = fopen(path, "w")) )
    {
      fprintf(stderr, "Failed to open file");
      return -1;
    }
      
  fwrite( header, sizeof(header), 1, f);
      
  temp_32 = htobe32(version);
  fwrite( &temp_32, sizeof(temp_32), 1, f);

  filesize = 1708 + (*num_rooms * 4) + (stairs[0][0] * 2) + (stairs[1][0] * 2);
  temp_32 = htobe32(filesize);
  fwrite( &temp_32, sizeof(temp_32), 1, f);
      
  fwrite( &pc[0], sizeof(temp_8), 1, f);
  fwrite( &pc[1], sizeof(temp_8), 1, f);
      
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  fwrite( &hardness[i][j], sizeof(temp_8), 1, f);
	}
    }

  temp_16 = htobe16(*num_rooms);
  fwrite(&temp_16, sizeof(temp_16), 1, f);

  for( i = 0; i < *num_rooms; i++ )
    {
      fwrite(&rooms[0][i], sizeof(temp_8), 1, f);
      fwrite(&rooms[1][i], sizeof(temp_8), 1, f);
      fwrite(&rooms[2][i], sizeof(temp_8), 1, f);
      fwrite(&rooms[3][i], sizeof(temp_8), 1, f);
    }
  temp_16 = htobe16(stairs[0][0]);
  fwrite(&temp_16, sizeof(temp_16), 1, f);
      
  for( i = 1; i <= stairs[0][0] * 2; i = i + 2 )
    {
      fwrite(&stairs[0][i], sizeof(temp_8), 1, f);
      fwrite(&stairs[0][i+1], sizeof(temp_8), 1, f);
    }
      
  temp_16 = htobe16(stairs[1][0]);
  fwrite(&temp_16, sizeof(temp_16), 1, f);
      
  for( i = 1; i <= stairs[1][0] * 2; i = i + 2 )
    {
      fwrite(&stairs[1][i], sizeof(temp_8), 1, f);
      fwrite(&stairs[1][i+1], sizeof(temp_8), 1, f);
    }
      
  fclose(f);
  return 0;
}

int run_game( int hardness[21][80], vector<NPC_desc> monsters, vector<Obj_desc> items, int rooms[4][1000], int *num_rooms, int stairs[2][9], Character *characters[21][80], int pc_loc[2], int nummon, int shop_num )
{
  int i, j;
  heap_init( turn_h, turn_cmp, NULL );

  int tunnel_map[21][80];
  int non_tunnel_map[21][80];
  char basic_map[21][80]; //map of the dungeon in chars
  char fog_map[21][80];
  char t_map[21][80]; //map of just terrain
  int fog = 1;
  Obj *item_map[21][80];

  shop_loc[0] = rooms[0][shop_num];
  shop_loc[1] = rooms[1][shop_num];
  shop_loc[2] = rooms[2][shop_num];
  shop_loc[3] = rooms[3][shop_num];
  
  gen_characters( hardness, characters, rooms, monsters, NULL, pc_loc, shop_num,  nummon, turn_h );
  gen_items( hardness, item_map, rooms, items, 20, shop_num); //hardcoded to 20 items per level
  gen_shop( shop_loc, item_map, items );
  update_map( hardness, rooms, num_rooms, stairs, pc_loc, characters, basic_map );
  update_t_map( hardness, rooms, num_rooms, stairs, pc_loc, t_map );
  init_fog( fog_map );

  Character *p;
  int ch, choice, n;
  while(1)
    {
    begin:
      if( check_win( characters ) == 1 )
	{
	  if( fog == 1 )
	    print_d( fog_map, characters, item_map, pc_loc );
	  if( fog == 0 )
	    print_d( basic_map, characters, item_map, pc_loc );
	    endwin();
	  printf(triumph);
	  break;
	}
      dijkstra_tunneling( hardness, pc_loc, tunnel_map );
      dijkstra_non_tunneling( hardness, pc_loc, non_tunnel_map );

      p =  (Character*) heap_remove_min(turn_h);
      p->hn = NULL;
      
      if( p->exists == 0 )
	{
	  if( ((PC*)p)->isa == 1 )
	    {
	      if( fog == 1 )
		print_d( fog_map, characters, item_map, pc_loc );
	      if( fog == 0 )
		print_d( basic_map, characters, item_map, pc_loc );
	      endwin();
	      printf(ganon);
	      break;
	    }
	  continue;
	}
      if(((PC*) p)->isa == 1 )
	{
	no_turn:
	  if( ((PC*) p)->win == 1 )
	    {
	      if( fog == 1 )
		print_d( fog_map, characters, item_map, pc_loc );
	      if( fog == 0 )
		print_d( basic_map, characters, item_map, pc_loc );
	      endwin();
	      printf(triumph);
	      break;
	    }
	  update_mods( (PC*) p );
	  update_map( hardness, rooms, num_rooms, stairs, pc_loc, characters, basic_map );
	  update_t_map( hardness, rooms, num_rooms, stairs, pc_loc, t_map );
	  update_fog( basic_map, fog_map, t_map, pc_loc );
	  if( fog == 1 )
	    print_d( fog_map, characters, item_map, pc_loc );
	  if( fog == 0 )
	    print_d( basic_map, characters, item_map, pc_loc );
	  top_text = "";
	  top_text_n = "";
	  if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
	    {
	      top_text = "Welcome to the item shop! Destroy items in the green area to get coin!";
	    }
	  mvprintw( 22, 0, "HP: %d    Damage: %d    Defense: %d    Speed: %d    Coins: %d", ((PC*)p)->hp, ((PC*)p)->dam, ((PC*)p)->def, ((PC*)p)->speed, ((PC*)p)->coins );
	  ch = getch();
	  switch(ch)
	    {
	    case '5':
	    case ' ':
	    case '.':
	      choice = 0;
	      break;
	    case 'k':
	    case '8':
	      choice = 1;
	      break;
	    case 'u':
	    case '9':
	      choice = 2;
	      break;
	    case 'l':
	    case '6':
	      choice = 3;
	      break;
	    case 'n':
	    case '3':
	      choice = 4;
	      break;
	    case 'j':
	    case '2':
	      choice = 5;
	      break;
	    case 'b':
	    case '1':
	      choice = 6;
	      break;
	    case 'h':
	    case '4':
	      choice = 7;
	      break;
	    case 'y':
	    case '7':
	      choice = 8;
	      break;
	    case '>':
	      if( on_downstairs( pc_loc, stairs ) == 1 )
		{
		  heap_init( turn_h, turn_cmp, NULL );
		  n = (rand() % 6) + 5;
		  num_rooms = &n;
		  generate( hardness, rooms, num_rooms, stairs, pc_loc, shop_num );
		  shop_loc[0] = rooms[0][shop_num];
		  shop_loc[1] = rooms[1][shop_num];
		  shop_loc[2] = rooms[2][shop_num];
		  shop_loc[3] = rooms[3][shop_num];
		  gen_characters( hardness, characters, rooms, monsters, (PC*)p, pc_loc, shop_num, nummon, turn_h );
		  gen_items( hardness, item_map, rooms, items, 10, shop_num);
		  gen_shop( shop_loc, item_map, items );
		  update_map( hardness, rooms, num_rooms, stairs, pc_loc, characters, basic_map );
		  init_fog( fog_map );
		  goto begin;
		}  
	      else
		goto no_turn;
	      break;
	    case '<':
	      if( on_upstairs( pc_loc, stairs ) == 1 )
		{
		  heap_init( turn_h, turn_cmp, NULL );
		  n = (rand() % 6) + 5;
		  num_rooms = &n;
		  generate( hardness, rooms, num_rooms, stairs, pc_loc, shop_num );
		  shop_loc[0] = rooms[0][shop_num];
		  shop_loc[1] = rooms[1][shop_num];
		  shop_loc[2] = rooms[2][shop_num];
		  shop_loc[3] = rooms[3][shop_num];
		  gen_characters( hardness, characters, rooms, monsters, (PC*)p,pc_loc, shop_num, nummon, turn_h );
		  gen_items( hardness, item_map, rooms, items, 10, shop_num);
		  gen_shop( shop_loc, item_map, items );
		  update_map( hardness, rooms, num_rooms, stairs, pc_loc, characters, basic_map );
		  init_fog( fog_map );
		  goto begin;
		}
	      else
		goto no_turn;
	      break;
	    case 'm':
	      list_monster( characters, pc_loc );
	      goto no_turn;
	      break;
	    case 'Q':
	      endwin();
	      printf(quitRLG327);
	      return 0;
	      break;
	    case 'f':
	      if( fog == 1 )
		fog = 0;
	      else
		fog = 1;
	      goto no_turn;
	      break;
	    case 't':
	      teleport_mode( basic_map, fog_map, characters, item_map, pc_loc );
	      cpy_delete_char_pc( hardness, characters, item_map, ((PC*)p)->inventory, p, pc_loc[0], pc_loc[1] );
	      goto no_turn;
	      break;
	    case 'L':
	      look_mode( basic_map, fog_map, characters, item_map, pc_loc );
	      break;
	    case 'i':
	      inventory_menu( (PC*) p, item_map, pc_loc );
	      goto no_turn;
	      break;
	    case 'e':
	      equip_menu( (PC*) p, item_map, pc_loc );
	      break;
	    default:
	      goto no_turn;
	    }
	  directed_move( hardness, characters, ((PC*) p)->inventory, item_map,  p, choice );
	  if( fog == 1 )
	    print_d( fog_map, characters, item_map, pc_loc );
	  if( fog == 0 )
	    print_d( basic_map, characters, item_map, pc_loc );
	  pc_loc[0] = p->location[0];
	  pc_loc[1] = p->location[1];
	  p->next_turn = p->next_turn + 1000/(p->speed);
	  p->hn = heap_insert(turn_h, p);
	  continue;
	}
      if(((NPC*) p)->isa == 0 )
	{
	  if( search_abil(((NPC*) p), "PASS") == 1 )
	    {
	      if( search_abil(((NPC*) p), "ERRATIC") == 1 )
		{
		  if( rand() % 2 == 1 )
		    {
		      random_move_t( hardness, characters, p );
		      p->next_turn = p->next_turn + 1000/(p->speed);
		      p->hn = heap_insert(turn_h, p);
		    }
		  else if( search_abil(((NPC*) p), "TELE") == 1 )
		    {
		      //erratic telepathic ghost
		      if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else
			{
			  move_to_pc( hardness, characters, p, tunnel_map );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		    }
		  else
		    {
		      //erratic ghost
		      if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			{
			  move_to_pc( hardness, characters, p, non_tunnel_map );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		    }
		}
	      else
		{
		  if( search_abil(((NPC*) p), "TELE") == 1 )
		    {
		      //telepathic ghost
		      if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else
			{
			  move_to_pc( hardness, characters, p, tunnel_map );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		    }
		  else
		    {
		      //ghost
		      if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			{
			  move_to_pc( hardness, characters, p, non_tunnel_map );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		    }
		}
	    }
	  else
	    {
	      if( search_abil(((NPC*) p), "ERRATIC") == 1 )
		{
		  if( search_abil(((NPC*) p), "TUNNEL") == 1 )
		    {
		      if( rand() % 2 == 1 )
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else if( search_abil(((NPC*) p), "TELE") == 1 )
			{
			  //erratic tunnelling telepathic
			  if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			    {
			      random_move_t( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      move_to_pc( hardness, characters, p, tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		      else
			{
			  //erratic tunnelling
			  if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      random_move_t( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		    }
		  else
		    {
		      if( rand() % 2 == 1 )
			{
			  random_move_t( hardness, characters, p );
			  p->next_turn = p->next_turn + 1000/(p->speed);
			  p->hn = heap_insert(turn_h, p);
			}
		      else if( search_abil(((NPC*) p), "TELE") == 1 )
			{
			  //erratic telepathic
			  if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			    {
			      random_move( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		      else
			{
			  //erratic
			  if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      random_move( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		    }
		}
	      else
		{
		  if( search_abil(((NPC*) p), "TUNNEL") == 1 )
		    {
		      if( search_abil(((NPC*) p), "TELE") == 1 )
			{
			  //tunnelling telepathic
			  if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			    {
			      random_move_t( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      move_to_pc( hardness, characters, p, tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		      else
			{
			  //tunneling
			  if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			      continue;
			    }
			  else
			    {
			      random_move_t( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			      continue;
			    }
			}
		    }
		  else
		    {
		      if( search_abil(((NPC*) p), "TELE") == 1 )
			{
			  //telepathic
			  if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
			    {
			      random_move( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			  else
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			    }
			}
		      else
			{
			  //nothing oof
			  if( see_pc( p, rooms, num_rooms, pc_loc ) == 1 )
			    {
			      move_to_pc( hardness, characters, p, non_tunnel_map );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			      continue;
			    }
			  else
			    {
			      random_move_t( hardness, characters, p );
			      p->next_turn = p->next_turn + 1000/(p->speed);
			      p->hn = heap_insert(turn_h, p);
			      continue;
			    }
			}
		    }
		}
	    }
	  if( search_abil( (NPC*) p, "DESTROY" ) == 1 &&  item_map[p->location[1]][p->location[0]] != NULL )
	    {
	      item_map[p->location[1]][p->location[0]] = NULL;
	    }
	}
    } //end of while

  for( j = 0; j < 21; j++ )
    {
      for( i = 0; i < 80; i++ )
	{
	  if(characters[j][i] != NULL )
	    {
		delete characters[j][i];
	    }
	}
    }
  
  return 0;
}

int inside_shop( int rooms[4][1000], int shop_num, int x, int y )
{
  if( (rooms[0][shop_num] <= x && x <= rooms[0][shop_num] + rooms[2][shop_num]) && (rooms[1][shop_num] <= y && y <= rooms[1][shop_num] + rooms[3][shop_num] ))
    return 1;
  else
    return 0;
    
}
int inside_shop( int room[4], int x, int y )
{
  if( (room[0] <= x && x <= room[0] + room[2]) && (room[1] <= y && y <= room[1] + room[3] ))
    return 1;
  else
    return 0;
}

int generate( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], int shop_num )
{
  initialize_area( hardness );
  
  int i, j, k; 
  int pass;

  for( i = 0; i < 10; i++ )
    {
      rooms[0][i] = -1;
    }
  
  for( k = 0; k < *num_rooms; k++ )
    {
      pass = 1;
      
      generate_room( rooms );

      for( i = rooms[1][k] - 1; i < rooms[1][k] + rooms[3][k] + 1; i++ )
	{
	  for( j = rooms[0][k] - 1; j < rooms[0][k] + rooms[2][k] + 1; j++ )
	    {
	      if( hardness[i][j] == 255 || hardness[i][j] == 0 )
		{
		  pass = 0;
		  break;
		}
	    }
	  if( pass == 0 )
	    break;
	}
      if( pass == 0 )
	{
	  rooms[0][k] = -1;
	  k = k - 1;
	}
      else
	{
	  for( i = rooms[1][k]; i < rooms[1][k] + rooms[3][k]; i++ )
	    {
	      for( j = rooms[0][k]; j < rooms[0][k] + rooms[2][k]; j++ )
		{
		  hardness[i][j] = 0;
		}
	    }
	}
    }
  corridors( rooms, hardness );
  stairsANDPC( hardness, stairs, pc, rooms, shop_num );
  return 0;
}

static int32_t turn_cmp(const void *key, const void *with)
{
  if( ((Character *) key)->next_turn == ((Character *) with)->next_turn )
    {
      return ((Character *) key)->seq_num - ((Character *) with)->seq_num; 
    }
  else
    {
      return ((Character *) key)->next_turn - ((Character *) with)->next_turn;
    }
}

int gen_characters( int hardness[21][80], Character *characters[21][80], int rooms[4][1000], vector<NPC_desc> monsters, PC* p, int pc_loc[2], int shop_num, int nummon, heap_t *turn_h )
{
  int i, j, temp_x, temp_y;
  int done = 0;
  int temp;

  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  characters[i][j] = NULL;
	}
    }
  if( p != NULL )
    {
      characters[pc_loc[1]][pc_loc[0]] = p;
      characters[pc_loc[1]][pc_loc[0]]->location[0] = pc_loc[0];
      characters[pc_loc[1]][pc_loc[0]]->location[1] = pc_loc[1];
    }
  else
    {
      characters[pc_loc[1]][pc_loc[0]] = new PC(pc_loc);
    }

  characters[pc_loc[1]][pc_loc[0]]->hn = heap_insert(turn_h, characters[pc_loc[1]][pc_loc[0]]);

  for( i = 0; i < nummon; i++ )
    {
      done = 0;
      while( !done )
	{
	  temp_x = (rand() % 78) + 1;
	  temp_y = (rand() % 19) + 1;
  
	  if( hardness[temp_y][temp_x] == 0 && characters[temp_y][temp_x] == NULL && inside_shop(rooms, shop_num, temp_x, temp_y ) == 0 )
	    {
	      while( !done )
		{
		  temp = rand() % monsters.size();
		  if( monsters[temp].unique != 1 )
		    {
		      if( (rand() % 100) < monsters[temp].rarity )
			{
			  if( monsters[temp].unique == 0 )
			    monsters[temp].unique = 1;
			  characters[temp_y][temp_x] = monsters[temp].spawn( temp_x, temp_y, i);

			  characters[temp_y][temp_x]->hn = heap_insert(turn_h, characters[temp_y][temp_x]);
			  done = 1;
			}
		    }
		}
	    }
	}
    }
  return 0;
}

int gen_items( int hardness[21][80], Obj *item_map[21][80], int rooms[4][1000], vector<Obj_desc> items, int num, int shop_num)
{
  int i, j, temp_x, temp_y, temp;
  int done = 0;
  
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  item_map[i][j] = NULL;
	}
    }
  
for( i = 0; i < num; i++ )
    {
      done = 0;
      while( !done )
	{
	  temp_x = (rand() % 78) + 1;
	  temp_y = (rand() % 19) + 1;
  
	  if( hardness[temp_y][temp_x] == 0 && item_map[temp_y][temp_x] == NULL && inside_shop(rooms, shop_num, temp_x, temp_y ) == 0 )
	    {
	      while( !done )
		{
		  temp = rand() % items.size();
		  if( items[temp].art != 1 )
		    {
		      if( (rand() % 100) < items[temp].rarity )
			{
			  if( items[temp].art == 0 )
			    items[temp].art = 1;
			  item_map[temp_y][temp_x] = items[temp].spawn();

			  done = 1;
			}
		    }
		}
	    }
	}
    }
  return 0;
}

int gen_shop( int shop_loc[4], Obj *item_map[21][80], vector<Obj_desc> items )
{
  int x = shop_loc[0];
  int y = shop_loc[1];
  int x_mid, y_mid, pick, temp;
  pick = 0;
  
  if( shop_loc[2] % 2 == 0 )
    x_mid = (shop_loc[2] / 2) - 1;
  else
    x_mid = shop_loc[2] / 2;

  if( shop_loc[3] % 2 == 0 )
    y_mid = (shop_loc[2] / 2) - 1;
  else
    y_mid = shop_loc[3] / 2;
  
  while( pick < 4 )
    {
      temp = rand() % items.size();
      if( items[temp].art != 1 )
	{
	  if( items[temp].rarity <= 50 && (rand() % 50) < items[temp].rarity )
	    {
	      if( items[temp].art == 0 )
		items[temp].art = 1;
	      if( pick == 0 )
		item_map[y_mid-1+y][x_mid-1+x] = items[temp].spawn();

	      else if( pick == 1 )
		item_map[y_mid-1+y][x_mid+2+x] = items[temp].spawn();

	      else if( pick == 2 )
		item_map[y_mid+2+y][x_mid-1+x] = items[temp].spawn();

	      else
	        item_map[y_mid+2+y][x_mid+2+x] = items[temp].spawn();

	      pick++;
	    }
	}
    }
  
  return 0;
}

int inventory_menu( PC *p, Obj *item_map[21][80], int pc_loc[2] )
{
  int i, j;
  int cursor = 0;
  int buffer = 0;

  while(1)
  {
    clear();
    mvprintw(2, 5, "***************************");
    mvprintw(3, 5, "*        INVENTORY        *");
    mvprintw(4, 5, "***************************");
    buffer = 0;
    for( i = 0; i < p->inventory.size(); i++ )
    {
      if( i == cursor )
	{
	  mvprintw(buffer+6, 2, "-> %s    Value: %d", p->inventory[i].name.c_str(), p->inventory[i].val);
	}
      mvprintw(buffer+6, 5, "%s    Value: %d", p->inventory[i].name.c_str(), p->inventory[i].val);
      buffer++;
    }
    mvprintw(buffer+9, 5, "w to equip    d to drop    x to destroy    I to inspect");
    mvprintw(buffer+11, 5, "Current Stats: Damage: %d      Defense: %d      Speed: %d", p->dam, p->def, p->speed );
    mvprintw(buffer+13, 5, "Base Stats:    Damage: %d      Defense: %d      Speed: %d", p->base_dam, p->base_def, p->base_speed );
    int ch;
    ch = getch();
    switch(ch)
      {
      case 27:
	return 0;
	break;
      case KEY_DOWN:
	if( cursor < p->inventory.size()-1 )
	  {
	    cursor++;
	    move(cursor+6, 4);
	  }
	else
	  {
	    
	  }
	break;
      case KEY_UP:
	if( cursor != 0 )
	  {
	    cursor--;
	    move(cursor+6, 4);
	  }
	else
	  {
	    
	  }
	break;
      case 'w':
        if( search_equip(p->equip, p->inventory[cursor].type) == 1 )
	  {
	    p->equip.push_back(p->inventory[cursor]);
	    p->inventory.erase(p->inventory.begin() + cursor);
	  }
	else
	  {
	    mvprintw(buffer+8, 5, "Press any key to continue");
	    mvprintw(buffer+7, 5, "You already have something else equipped in this slot!");
	  }
	break;
      case 'd':
	if(item_map[pc_loc[1]][pc_loc[0]] != NULL )
	  {
	    mvprintw(buffer+8, 5, "Press any key to continue");
	    mvprintw(buffer+7, 5, "There's something already there!");
	    ch = getch();
	  }
	else
	  {
	    item_map[pc_loc[1]][pc_loc[0]] = p->inventory[cursor].copy();
	    p->inventory.erase(p->inventory.begin() + cursor);
	  }
	break;
      case 'x':
	if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
	  {
	    int x = shop_loc[0];
	    int y = shop_loc[1];
	    int x_mid, y_mid;
  
	    if( shop_loc[2] % 2 == 0 )
	      x_mid = (shop_loc[2] / 2) - 1;
	    else
	      x_mid = shop_loc[2] / 2;

	    if( shop_loc[3] % 2 == 0 )
	      y_mid = (shop_loc[2] / 2) - 1;
	    else
	      y_mid = shop_loc[3] / 2;
  
	    x_mid += x;
	    y_mid += y;
      
	    if( (pc_loc[0] == x_mid || pc_loc[0] == x_mid + 1) && (pc_loc[1] == y_mid || pc_loc[1] == y_mid + 1) )
	      {
		p->coins += p->inventory[cursor].val;
		p->inventory.erase(p->inventory.begin() + cursor);
	      }
	    else
	      p->inventory.erase(p->inventory.begin() + cursor);
	  }
	else
	  p->inventory.erase(p->inventory.begin() + cursor);
	break;
      case 'I':
	mvprintw(cursor+5, 5, "Press any key to continue");
	mvprintw(cursor+7, 5, "%s", p->inventory[cursor].desc.c_str());
	ch = getch();
	break;
      default:
	break;
      }  
    refresh();
  }

  return 0;
}

int search_abil( NPC *c, string abil )
{
  int i;
  
  for( i = 0; i < c->abil.size(); i++ )
    {
      if( c->abil[i].compare(abil) == 0 )
	return 1;
    }
  return 0;
}

int search_equip( vector<Obj> equip,  string type ) //searches equipped items for compatability with a new type
{
  int i;
  int count = 0;

  if( type.compare("RING") == 0 )
    {
      for( i = 0; i < equip.size(); i++ )
	{
	  if( equip[i].type.compare("RING") == 0 )
	    {
	      count++;
	    }
	}
      if( count > 1 && type.compare("RING") == 0 )
	return 0;
    }
  for( i = 0; i < equip.size(); i++ )
    {
      if( equip[i].type.compare(type) == 0 && type.compare("RING") != 0)
	return 0;
      if( equip[i].type.compare("SCROLL") == 0 || equip[i].type.compare("BOOK") == 0 || equip[i].type.compare("FLASK") == 0 || equip[i].type.compare("GOLD") == 0 || equip[i].type.compare("AMMUNITION") == 0 || equip[i].type.compare("FOOD") == 0 || equip[i].type.compare("WAND") == 0 || equip[i].type.compare("CONTAINER") == 0 )
	return 0;
    }
  return 1;
}

int equip_menu( PC *p,  Obj *item_map[21][80], int pc_loc[2] )
{
  int i, j;
  int cursor = 0;
  int buffer = 0;

  while(1)
  {
    clear();
    mvprintw(2, 5, "****************************");
    mvprintw(3, 5, "*      EQUIPPED ITEMS      *");
    mvprintw(4, 5, "****************************");
    buffer = 0;
    for( i = 0; i < p->equip.size(); i++ )
    {
      if( i == cursor )
	{
	  mvprintw(buffer+6, 2, "-> %s Type: %s Damage: %s Defense: %d Speed: %d", p->equip[i].name.c_str(), p->equip[i].type.c_str(), p->equip[i].dam_die.print().c_str(), p->equip[i].def, p->equip[i].speed );
	}
      else
	{
	  mvprintw(buffer+6, 5, "%s Type: %s", p->equip[i].name.c_str(), p->equip[i].type.c_str());
	}
      buffer++;
    }
    mvprintw(buffer+9, 5, "t to unequip    d to drop    x to destroy    I to inspect");
    int ch;
    ch = getch();
    switch(ch)
      {
      case 27:
	return 0;
	break;
      case KEY_DOWN:
	if( cursor < p->equip.size()-1 )
	  {
	    cursor++;
	    move(cursor+6, 4);
	  }
	else
	  {
	    
	  }
	break;
      case KEY_UP:
	if( cursor != 0 )
	  {
	    cursor--;
	    move(cursor+6, 4);
	  }
	else
	  {
	    
	  }
	break;
      case 't':
        if( p->inventory.size() < 10 )
	  {
	    p->inventory.push_back(p->equip[cursor]);
	    p->equip.erase(p->equip.begin() + cursor);
	  }
	break;
      case 'd':
	if(item_map[pc_loc[1]][pc_loc[0]] != NULL )
	  {
	    mvprintw(buffer+8, 5, "Press any key to continue");
	    mvprintw(buffer+7, 5, "There's something already there!");
	    ch = getch();
	  }
	else
	  {
	    item_map[pc_loc[1]][pc_loc[0]] = p->equip[cursor].copy();
	    p->equip.erase(p->equip.begin() + cursor);
	  }
	break;
      case 'x':
	if( inside_shop( shop_loc, pc_loc[0], pc_loc[1] ) == 1 )
	  {
	    int x = shop_loc[0];
	    int y = shop_loc[1];
	    int x_mid, y_mid;
  
	    if( shop_loc[2] % 2 == 0 )
	      x_mid = (shop_loc[2] / 2) - 1;
	    else
	      x_mid = shop_loc[2] / 2;

	    if( shop_loc[3] % 2 == 0 )
	      y_mid = (shop_loc[2] / 2) - 1;
	    else
	      y_mid = shop_loc[3] / 2;
  
	    x_mid += x;
	    y_mid += y;
      
	    if( (pc_loc[0] == x_mid || pc_loc[0] == x_mid + 1) && (pc_loc[1] == y_mid || pc_loc[1] == y_mid + 1) )
	      {
		p->coins += p->equip[cursor].val;
		p->equip.erase(p->equip.begin() + cursor);
	      }
	    else
	      p->equip.erase(p->equip.begin() + cursor);
	  }
	else
	  p->equip.erase(p->equip.begin() + cursor);
	break;
      case 'I':
	mvprintw(cursor+5, 5, "Press any key to continue");
	mvprintw(cursor+7, 5, "%s", p->equip[cursor].desc.c_str());
	ch = getch();
	break;
      default:
	break;
      }  
    refresh();
  }

  return 0;
}

int list_monster( Character *characters[21][80], int pc_loc[2] )
{
  int i, j, x, y, nummon, num;
  // top is where in the monster list the top is. bottom is bottom. cursor is which monster its on. window is 5 mosnters
  int top = 0;
  int bottom = 4;
  int cursor = 0;
  int buffer = 0; 
  nummon = 0;
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  if( characters[i][j] != NULL && ((NPC*)characters[i][j])->isa == 0 )
	    nummon++;
	}
    }
  Character *monsters[nummon];
  num = 0;
  for( i = 0; i < 21; i++ )
  {
    for( j = 0; j < 80; j++ )
    {
      if( characters[i][j] != NULL && ((NPC*)characters[i][j])->isa == 0 )
      {
        monsters[num] = characters[i][j];
        num++;
      }
    }
  }

  //built list of monsters now for displaying and moving them
  char n[6] = "NORTH";
  char s[6] = "SOUTH";
  char e[5] = "EAST";
  char w[5] = "WEST";
  char *tmp;
  char *tmp2;
  while(1)
  {
    clear();
    mvprintw(2, 5, "********************");
    mvprintw(3, 5, "*   MONSTER LIST   *");
    mvprintw(4, 5, "********************");
    buffer = 0;
    for( i = top; i < nummon && i <= bottom; i++ )
    {
      x = monsters[i]->location[0] - pc_loc[0];
      y = monsters[i]->location[1] - pc_loc[1];
      if( y > 0 )
	tmp = s;
      if( y < 0 )
	tmp = n;
      if( y == 0 )
	tmp = n;
      if( x > 0 )
	tmp2 = e;
      if( x < 0 )
	tmp2 = w;
      if( x == 0 )
	tmp2 = w;
      x = abs(x);
      y = abs(y);
      if( i == cursor )
	{
	  mvprintw(buffer+6, 2, "-> A scary %c, %d %s and %d %s", monsters[i]->symbol, x, tmp2, y, tmp);
	}
      mvprintw(buffer+6, 5, "A scary %c, %d %s and %d %s", monsters[i]->symbol, x, tmp2, y, tmp);
      buffer++;
    }
    int ch;
    ch = getch();
    switch(ch)
      {
      case 27:
	return 0;
	break;
      case KEY_DOWN:
	if( cursor != bottom )
	  {
	    cursor++;
	    move(cursor+6, 4);
	  }
	else if( cursor == bottom && bottom + 1 < nummon )
	  {
	    bottom++;
	    top++;
	    cursor = bottom;
	  }
	break;
      case KEY_UP:
	if( cursor != top )
	  {
	    cursor--;
	    move(cursor+6, 4);
	  }
	else if( cursor == top && top - 1 >= 0 )
	  {
	    top--;
	    bottom--;
	    cursor = top;
	  }
	break;
      default:
	break;
      }
    refresh();
  }

  return 0;
}

int teleport_mode( char basic_map[21][80], char fog_map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] )
{
  int t_x, t_y, i, j;
  int ch;
  int fog = 0;
  char temp_map[21][80];
  char t_char;
  
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  temp_map[i][j] = basic_map[i][j];
	}
    }
  t_x = pc_loc[0];
  t_y = pc_loc[1];
  
  while(1)
    {
      if( temp_map[t_y][t_x] != '*' )
	{
	  t_char = temp_map[t_y][t_x];
	  temp_map[t_y][t_x] = '*';
	}
      print_d( temp_map, characters, item_map, pc_loc );
      ch = getch();
      switch(ch)
	{
	case '5':
	case ' ':
	case '.':
	  //stay in place
	  break;
	case 'k':
	case '8':
	  if( t_y - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_y--;
	    }
	  break;
	case 'u':
	case '9':
	  if( t_y - 1 > 0 && t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x++;
	      t_y--;
	    }
	  break;
	case 'l':
	case '6':
	  if( t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x++;
	    }
	  break;
	case 'n':
	case '3':
	  if( t_y + 1 < 20 && t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x++;
	      t_y++;
	    }
	  break;
	case 'j':
	case '2':
	  if( t_y + 1 < 20 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_y++;
	    }
	  break;
	case 'b':
	case '1':
	  if( t_y + 1 < 20 && t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x--;
	      t_y++;
	    }
	  break;
	case 'h':
	case '4':
	  if( t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x--;
	    }
	  break;
	case 'y':
	case '7':
	  if( t_y - 1 > 0 && t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      t_x--;
	      t_y--;
	    }
	  break;
	case 'm':
	  list_monster( characters, pc_loc );
	  break;
	case 'Q':
	  endwin();
	  return 0;
	  break;
	case 'f':
	  if( fog == 1 )
	    fog = 0;
	  else
	    fog = 1;
	  break;
	case 't':
	  pc_loc[0] = t_x;
	  pc_loc[1] = t_y;
	  return 0;
	  break;
	default:
	  break;
	}
    }
}

int look_mode( char basic_map[21][80], char fog_map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] )
{
  int t_x, t_y, i, j;
  int ch;
  int fog = 0;
  char temp_map[21][80];
  char t_char;
  
  t_x = pc_loc[0];
  t_y = pc_loc[1];
  
  top_text = "Press space to select or esc to quit";
  while(1)
    {
      if( fog = 0 )
	{
	  for( i = 0; i < 21; i++ )
	    {
	      for( j = 0; j < 80; j++ )
		{
		  temp_map[i][j] = basic_map[i][j];
		}
	    }
	}
      if( fog = 1 )
	{
	  for( i = 0; i < 21; i++ )
	    {
	      for( j = 0; j < 80; j++ )
		{
		  temp_map[i][j] = fog_map[i][j];
		}
	    }
	}
      if( temp_map[t_y][t_x] != '*' )
	{
	  t_char = temp_map[t_y][t_x];
	  temp_map[t_y][t_x] = '*';
	}
      print_d( temp_map, characters, item_map, pc_loc );
      ch = getch();
      switch(ch)
	{
	case '5':
	case '.':
	  //stay in place
	  break;
	case 'k':
	case '8':
	  if( t_y - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x, t_y-1) == 1 )
		{
		  t_y--;
		}
	    }
	  break;
	case 'u':
	case '9':
	  if( t_y - 1 > 0 && t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x+1, t_y-1) == 1 )
		{
		  t_x++;
		  t_y--;
		}
	    }
	  break;
	case 'l':
	case '6':
	  if( t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x+1, t_y) == 1 )
		{
		  t_x++;
		}
	    }
	  break;
	case 'n':
	case '3':
	  if( t_y + 1 < 20 && t_x + 1 < 79 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x+1, t_y+1) == 1 )
		{
		  t_x++;
		  t_y++;
		}
	    }
	  break;
	case 'j':
	case '2':
	  if( t_y + 1 < 20 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x, t_y+1) == 1 )
		{
		  t_y++;
		}
	    }
	  break;
	case 'b':
	case '1':
	  if( t_y + 1 < 20 && t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x-1, t_y+1) == 1 )
		{
		  t_x--;
		  t_y++;
		}
	    }
	  break;
	case 'h':
	case '4':
	  if( t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x-1, t_y) == 1 )
		{
		  t_x--;
		}
	    }
	  break;
	case 'y':
	case '7':
	  if( t_y - 1 > 0 && t_x - 1 > 0 )
	    {
	      temp_map[t_y][t_x] = t_char;
	      if( fog == 0 || pc_view(t_x, t_y, t_x-1, t_y-1) == 1 )
		{
		  t_x--;
		  t_y--;
		}
	    }
	  break;
	case 'm':
	  list_monster( characters, pc_loc );
	  break;
	case 'Q':
	  endwin();
	  return 0;
	  break;
	case 'f':
	  if( fog == 1 )
	    fog = 0;
	  else
	    fog = 1;
	  break;
	case 'L':
	  return 0;
	  break;
	case ' ':
	  if( characters[t_y][t_x] != NULL && ((NPC*)characters[t_y][t_x])->isa == 0 )
	    {
	      clear();
	      for( i = 0; i < ((NPC*)characters[t_y][t_x])->abil.size(); i++ )
		{
		  mvprintw(1, 0, "%s", ((NPC*)characters[t_y][t_x])->abil[i].c_str());
		}
	      mvprintw(4, 40, "%s", ((NPC*)characters[t_y][t_x])->name.c_str());
	      mvprintw(5, 40, "%s", ((NPC*)characters[t_y][t_x])->desc.c_str());
	      mvprintw(0, 0, "Press any key to continue");
	      ch = getch();
	    }
	  break;
	default:
	  break;
	}
    }
}

int pc_view( int x, int y, int new_x, int new_y )
{
  if( (new_x >= x-2) && (new_x <= x+2) && (new_y >= y-2) && (new_y <= y+2) )
    return 1;
  else
    return 0;
}

int update_map( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], Character *characters[21][80], char basic_map[21][80] )
{
  int i, j, k, roomx, roomy, roomxl, roomyl;
  int stop = 0;
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  if( characters[i][j] != NULL && characters[i][j]->exists == 1 )
	    {
	      //if(((NPC*)characters[i][j])->isa = 0 )
	      //	basic_map[i][j] = ((NPC*)characters[i][j])->symbol;
	      //else
	      
		basic_map[i][j] = characters[i][j]->symbol;
	      continue;
	    }
	  if( i == 0 || i == 20 )
	    {
	      basic_map[i][j] = '-';
	      continue;
	    }
	  if( j == 0 || j == 79 )
	    {
	      basic_map[i][j] = '|';
	      continue;
	    }
	  if( hardness[i][j] > 0 )
	    {
	      basic_map[i][j] = ' ';
	      continue;
	    }
	  if( hardness[i][j] == 0 )
	    {
	      for( k = 1; k <= stairs[0][0] * 2; k = k + 2 ) //find upstairs
		{
		  if( j == stairs[0][k] && i == stairs[0][k+1] )
		    {
		      basic_map[i][j] = '<';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      for( k = 1; k <= stairs[1][0] * 2; k = k + 2 ) //find downstairs
		{
		  if( j == stairs[1][k] && i == stairs[1][k+1] )
		    {
		      basic_map[i][j] = '>';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      for( k = 0; k < *num_rooms; k++ ) //find rooms
		{
		  roomx = rooms[0][k];
		  roomy = rooms[1][k];
		  roomxl = rooms[2][k];
		  roomyl = rooms[3][k];

		  if( (roomx <= j && j < (roomx + roomxl)) && (roomy <= i && i < (roomy + roomyl)) )
		    {
		      basic_map[i][j] = '.';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      basic_map[i][j] = '#';
	      continue;		
	    }
	  basic_map[i][j] = ' ';
	}
    }
  return 0;
}

int update_t_map( int hardness[21][80], int rooms[4][1000], int *num_rooms, int stairs[2][9], int pc[2], char t_map[21][80] )
{
  int i, j, k, roomx, roomy, roomxl, roomyl;
  int stop = 0;
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  if( i == 0 || i == 20 )
	    {
	      t_map[i][j] = '-';
	      continue;
	    }
	  if( j == 0 || j == 79 )
	    {
	      t_map[i][j] = '|';
	      continue;
	    }
	  if( hardness[i][j] > 0 )
	    {
	      t_map[i][j] = ' ';
	      continue;
	    }
	  if( hardness[i][j] == 0 )
	    {
	      for( k = 1; k <= stairs[0][0] * 2; k = k + 2 ) //find upstairs
		{
		  if( j == stairs[0][k] && i == stairs[0][k+1] )
		    {
		      t_map[i][j] = '<';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      for( k = 1; k <= stairs[1][0] * 2; k = k + 2 ) //find downstairs
		{
		  if( j == stairs[1][k] && i == stairs[1][k+1] )
		    {
		      t_map[i][j] = '>';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      for( k = 0; k < *num_rooms; k++ ) //find rooms
		{
		  roomx = rooms[0][k];
		  roomy = rooms[1][k];
		  roomxl = rooms[2][k];
		  roomyl = rooms[3][k];

		  if( (roomx <= j && j < (roomx + roomxl)) && (roomy <= i && i < (roomy + roomyl)) )
		    {
		      t_map[i][j] = '.';
		      stop = 1;
		      continue;
		    }
		}
	      if( stop == 1 )
		{
		  stop = 0;
		  continue;
		}
	      t_map[i][j] = '#';
	      continue;		
	    }
	  t_map[i][j] = ' ';
	}
    }
  return 0;
}

int init_fog( char fog_map[21][80] )
{
  int i, j;
  for( i = 0; i < 21; i++ )
  {
    for( j = 0; j < 80; j++ )
      {
	if( i == 0 || i == 20 )
	  {
	    fog_map[i][j] = '-';
	    continue;
	  }
	if( j == 0 || j == 79 )
	  {
	    fog_map[i][j] = '|';
	    continue;
	  }
	else
	  fog_map[i][j] = ' ';
      }
  }
  return 0;
}
int update_fog( char basic_map[21][80], char fog_map[21][80], char t_map[21][80], int pc_loc[2] ) //basic_map must be updated before running
{
  int i, j, x, y;

  x = pc_loc[0];
  y = pc_loc[1];
  for( i = 0; i < 21; i++ )
  {
    for( j = 0; j < 80; j++ )
      {
	if( j >= x-1 && j <= x+1 && x-1 >= 0 && x+1 < 79 && i >= y-1 && i <= y+1 && y-1 >= 0 && y+1 < 20 )
	  {
	    fog_map[i][j] = basic_map[i][j];
	    continue;
	  }
	if( j >= x-2 && j <= x+2 && i >= y-2 && i <= y+2 && x-2 >= 0 && x+2 < 79 && y-2 >= 0 && y+2 < 20 )
	  {
	    fog_map[i][j] = basic_map[i][j];
	    continue;
	  }
	else if( x-1 >= 0 && x+1 < 79 && y-1 >= 0 && y+1 < 20 && fog_map[i][j] != ' ' && fog_map[i][j] != t_map[i][j] )
	  fog_map[i][j] = t_map[i][j];
	else if( x-2 >= 0 && x+2 < 79 && y-2 >= 0 && y+2 < 20 && fog_map[i][j] != ' ' && fog_map[i][j] != t_map[i][j] )
	  fog_map[i][j] = t_map[i][j];
      }
  }
  return 0;
}
int check_win( Character *characters[21][80] )
{
  int i, j;
  
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  if( characters[i][j] != NULL && ((NPC*)characters[i][j])->isa == 0 )
	    {
	      return 0;
	    }
	}
    }
  return 1;
}
int see_pc( Character *c, int rooms[4][1000], int *num_rooms, int pc[2] )
{
  //first find the room the pc is in
  int i;
  int x = c->location[0];
  int y = c->location[1];

  for( i = 0; i < *num_rooms; i++ )
    {
      if( (rooms[0][i] <= pc[0] && pc[0] <= rooms[0][i] + rooms[2][i]) && (rooms[1][i] <= pc[1] && pc[1] <= rooms[1][i] + rooms[3][i] ) )
	{
	  if( (rooms[0][i] <= x && x <= rooms[0][i] + rooms[2][i]) && (rooms[1][i] <= y && y <= rooms[1][i] + rooms[3][i] ))
	    return 1;
	  else
	    return 0;
	}
    }
  return -1;
}

int on_downstairs( int pc[2], int stairs[2][9] )
{
  int i;
  for( i = 1; i < 8; i = i + 2 )
    {
      if( pc[0] == stairs[1][i] && pc[1] == stairs[1][i+1] )
	return 1;
    }
  return 0;
}

int on_upstairs( int pc[2], int stairs[2][9] )
{
  int i;
  for( i = 1; i < 8; i = i + 2 )
    {
      if( pc[0] == stairs[0][i] && pc[1] == stairs[0][i+1] )
	return 1;
    }
  return 0;
}

int move_to_pc( int hardness[21][80], Character *characters[21][80], Character *c, int distance[21][80] )
{
  int i, j;
  int best_x = c->location[0];
  int best_y = c->location[1];

  if( distance[c->location[1]][c->location[0]] == 0 )
    {
      return 0;
    }
  if( search_abil( (NPC*) c, "TUNNEL" ) == 1 )
    {
      for( i = -1; i < 2; i++ )
	{
	  for( j = -1; j < 2; j++ )
	    {
	      if( distance[c->location[1] + i][c->location[0] + j] < distance[best_y][best_x] )
		{
		  best_x = c->location[0] + j;
		  best_y = c->location[1] + i;
		}
	    }
	}
    }
  else
    {
      for( i = -1; i < 2; i++ )
	{
	  for( j = -1; j < 2; j++ )
	    {
	      if( distance[c->location[1] + i][c->location[0] + j] < distance[best_y][best_x] && hardness[c->location[1] + i][c->location[0] + j] == 0 )
		{
		  best_x = c->location[0] + j;
		  best_y = c->location[1] + i;
		}
	    }
	}
    }

  cpy_delete_char_t( hardness, characters, c, best_x, best_y );
  return 0;
}

int random_move( int hardness[21][80], Character *characters[21][80], Character *c )
{
  if( c->exists == 0 )
    return 0;
  int num = rand() % 9;
  if( num == 1 )
    {
      if( hardness[c->location[1] - 1][c->location[0]] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c,  c->location[0], c->location[1] - 1 );
	      return 0;
	    }
	}
    }
  else if( num == 2 )
    {
      if( hardness[c->location[1] - 1][c->location[0] + 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 1 || c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	  cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] - 1 );
	  return 0;
	    }
	}
    }
  else if( num == 3 )
    {
      if( hardness[c->location[1]][c->location[0] + 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] );
	      return 0;
	    }
	}
    }
  else if( num == 4 )
    {
      if( hardness[c->location[1] + 1][c->location[0] + 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 19 || c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  else if( num == 5 )
    {
      if( hardness[c->location[1] + 1][c->location[0]] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 19 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0], c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  else if( num == 6 )
    {
      if( hardness[c->location[1] + 1][c->location[0] - 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 19 || c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  else if( num == 7 )
    {
      if( hardness[c->location[1]][c->location[0] - 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] );
	      return 0;
	    }
	}
    }
  else if( num == 8 )
    {
      if( hardness[c->location[1] - 1][c->location[0] - 1] != 0 )
	num = rand() % 9;
      else
	{
	  if( c->location[1] == 1 || c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] - 1 );
	      return 0;
	    }
	}
    }
  
  return 0;
}

int directed_move( int hardness[21][80], Character *characters[21][80], vector<Obj> &inventory, Obj *item_map[21][80], Character *c, int num )
{
  int temp;
  if( c->exists == 0 )
    return 0;
  if( num == 0 )
    return 0;
  if( num == 1 )
    {
      if( hardness[c->location[1] - 1][c->location[0]] != 0 )
        temp = 0;
      else
	{
	  if( c->location[1] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0], c->location[1] - 1 );
	      return 0;
	    }
	}
    }
  if( num == 2 )
    {
      if( hardness[c->location[1] - 1][c->location[0] + 1] != 0 )
	temp = 0;
      else
	{
	  if( c->location[1] == 1 || c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	  cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0] + 1, c->location[1] - 1 );
	  return 0;
	    }
	}
    }
  if( num == 3 )
    {
      if( hardness[c->location[1]][c->location[0] + 1] != 0 )
        temp = 0;
      else
	{
	  if( c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0]+1, c->location[1] );
	      return 0;
	    }
	}
    }
  if( num == 4 )
    {
      if( hardness[c->location[1] + 1][c->location[0] + 1] != 0 )
        temp = 0;
      else
	{
	  if( c->location[1] == 19 || c->location[0] == 78 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0]+1, c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  if( num == 5 )
    {
      if( hardness[c->location[1] + 1][c->location[0]] != 0 )
        temp = 0;
      else
	{
	  if( c->location[1] == 19 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0], c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  if( num == 6 )
    {
      if( hardness[c->location[1] + 1][c->location[0] - 1] != 0 )
        temp = 0;
      else
	{
	  if( c->location[1] == 19 || c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0]-1, c->location[1] + 1 );
	      return 0;
	    }
	}
    }
  if( num == 7 )
    {
      if( hardness[c->location[1]][c->location[0] - 1] != 0 )
        temp = 0;
      else
	{
	  if( c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0]-1, c->location[1] );
	      return 0;
	    }
	}
    }
  if( num == 8 )
    {
      if( hardness[c->location[1] - 1][c->location[0] - 1] != 0 )
        temp = 0;
      else
	{
	  if( c->location[1] == 1 || c->location[0] == 1 )
	    num = rand() % 9;
	  else
	    {
	      cpy_delete_char_pc( hardness, characters, item_map, inventory,  c,  c->location[0]-1, c->location[1] - 1 );
	      return 0;
	    }
	}
    }
  temp++;
  num = temp;
  return 0;
}

int random_move_t( int hardness[21][80], Character *characters[21][80], Character *c ) //move for tunneling monsters
{
  if( c->exists == 0 )
    return 0;
  int num = rand() % 9;
  if( num == 1 )
    {
      if( c->location[1] == 1 )
	num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c,  c->location[0], c->location[1] - 1 );
	}
    }
  else if( num == 2 )
    {
      if( c->location[1] == 1 || c->location[0] == 78 )
	num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] - 1 );
	}
    }
  else if( num == 3 )
    {
      if( c->location[0] == 78 )
	num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] );
	}
    }
  else if( num == 4 )
    {
      if( c->location[1] == 19 || c->location[0] == 78 )
        num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] + 1, c->location[1] + 1 );
	}
    }
  else if( num == 5 )
    {
      if( c->location[1] == 19 )
        num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0], c->location[1] + 1 );	      
	}
    }
  else if( num == 6 )
    {
      if( c->location[1] == 19 || c->location[0] == 1 )
        num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] + 1 );
	}
    }
  else if( num == 7 )
    {
      if( c->location[0] == 1 )
        num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] );
	}
    }
  else if( num == 8 )
    {
      if( c->location[1] == 1 || c->location[0] == 1 )
        num = rand() % 9;
      else
	{
	  cpy_delete_char_t( hardness, characters, c, c->location[0] - 1, c->location[1] - 1 );
	}
    }
  return 0;
}

int cpy_delete_char_t( int hardness[21][80], Character *characters[21][80], Character *c, int dest_x, int dest_y )
{
  if( c->location[0] == dest_x && c->location[1] == dest_y )
    return 0;
  if( inside_shop( shop_loc, dest_x, dest_y ) == 1 )
    {
      return 0;
    }
  if( characters[dest_y][dest_x] != NULL )
    {
      int damage = ((NPC*) c)->dam_die.roll();
      if( ((PC*) characters[dest_y][dest_x])->isa == 1 )
	{
	  damage -= ((PC*) characters[dest_y][dest_x])->def;
	  if( damage < 0 )
	    damage = 0;
	  ((PC*)characters[dest_y][dest_x])->hp -= damage;
	  if(((PC*)characters[dest_y][dest_x])->hp <= 0 )
	    {
	      characters[dest_y][dest_x]->exists = 0;
	    }
	  else
	    {
	      top_text_n = ((NPC*) c)->name + " damaged you  by " + to_string(damage);
	      return 0;
	    }
	}
      else
	{
	  
	}
      //combat 
    }
  if( hardness[dest_y][dest_x] != 0 && search_abil( (NPC*)c, "PASS" ) == 0 )
    {
      hardness[dest_y][dest_x] -= 85;
      if( hardness[dest_y][dest_x] < 0 )
	hardness[dest_y][dest_x] = 0;
      return 0;
    }
  int original_x, original_y;
  original_x = c->location[0];
  original_y = c->location[1];
  characters[dest_y][dest_x] = c;
  characters[dest_y][dest_x]->location[0] = dest_x;
  characters[dest_y][dest_x]->location[1] = dest_y;
  characters[original_y][original_x] = NULL;
  return 0;
}
int cpy_delete_char_pc( int hardness[21][80], Character *characters[21][80], Obj *item_map[21][80], vector<Obj> &inventory, Character *c, int dest_x, int dest_y ) //the destination cannot be the same or else it will be erased
{
  if( item_map[dest_y][dest_x] != NULL && inventory.size() < 10 && inside_shop( shop_loc, dest_x, dest_y ) == 1 )
    {
      char ch;
      mvprintw(0, 0, "Do you want to buy %s for %d coins?                           ", item_map[dest_y][dest_x]->name.c_str(), item_map[dest_y][dest_x]->val ); 
      mvprintw(1, 0, "y for yes n for no");
      ch = getch();
      switch(ch)
	{
	case 'y':
	  if( ((PC*)c)->coins < item_map[dest_y][dest_x]->val )
	    {
	      top_text = "Not enough coins! Sell stuff!                        ";
	      return 0;
	    }
	  else
	    {
	      ((PC*)c)->coins -= item_map[dest_y][dest_x]->val;
	      top_text = "You picked up " + item_map[dest_y][dest_x]->name;
	      inventory.push_back(*item_map[dest_y][dest_x]);
	      item_map[dest_y][dest_x] = NULL;
	    }
	  break;
	case 'n':
	  top_text = "Come again.                                              ";
	  return 0;
	  break;
	default:
	  return 0;
	  break;
	}
    }
  else if( item_map[dest_y][dest_x] != NULL && inventory.size() < 10 )
    {
      top_text = "You picked up " + item_map[dest_y][dest_x]->name;
      inventory.push_back(*item_map[dest_y][dest_x]);
      item_map[dest_y][dest_x] = NULL;
    }
  if( c->location[0] == dest_x && c->location[1] == dest_y )
    return 0;
  if( characters[dest_y][dest_x] != NULL )
    {
      int damage = ((PC*) c)->dam;
      ((NPC*)characters[dest_y][dest_x])->hp -= damage;
      if(((NPC*)characters[dest_y][dest_x])->hp <= 0 )
	{
	  if( search_abil(((NPC*)characters[dest_y][dest_x]), "BOSS") == 1 )
	    ((PC*) c)->win = 1;
	  if( search_abil(((NPC*)characters[dest_y][dest_x]), "SPLIT") == 1 )
	    {
	      monster_split( hardness, characters, characters[dest_y][dest_x] );
	      top_text = ((NPC*)characters[dest_y][dest_x])->name + " split!";
	      characters[dest_y][dest_x]->exists = 0;
	    }
	  else
	    {
	      top_text = "You killed " + ((NPC*)characters[dest_y][dest_x])->name;
	      characters[dest_y][dest_x]->exists = 0;
	    }
	}
      else
	{
	  top_text = "You damaged " + ((NPC*)characters[dest_y][dest_x])->name + " by " + to_string(damage);
	  return 0;
	}
      //combat code to go here
    }
  int original_x, original_y;
  original_x = c->location[0];
  original_y = c->location[1];
  characters[dest_y][dest_x] = c;
  characters[dest_y][dest_x]->location[0] = dest_x;
  characters[dest_y][dest_x]->location[1] = dest_y;
  hardness[dest_y][dest_x] = 0;
  characters[original_y][original_x] = NULL;
  return 0;
}

int monster_split( int hardness[21][80], Character *characters[21][80], Character *c )
{
  int i, j, k, x, y, count;
  count = 0;
  
  for( i = 0; i < monsters.size(); i++ )
    {
      if( tolower(((NPC*)c)->symbol) == monsters[i].symbol && ((NPC*)c)->color[0].compare(monsters[i].color[0]) == 0 )
	{
	  for( k = -1; k < 2; k++ )
	    {
	      if( count == 2 )
		break;
	      for( j = -1; j < 2; j++ )
		{
		  if( count == 2 )
		    break;
		  x = c->location[0] + k;
		  y = c->location[1] + j;
		  if( characters[y][x] == NULL )
		    {
		      nummon++;
		      characters[y][x] = monsters[i].spawn( x, y, nummon);
		      characters[y][x]->hn = heap_insert(turn_h, characters[y][x]);
		      hardness[y][x] = 0;
		      count++;
		    }
		}
	    }
	  return 0;
	}
    }
  count = 0;
  for( k = -1; k < 2; k++ )
    {
      if( count == 2 )
	break;
      for( j = -1; j < 2; j++ )
	{
	  if( count == 2 )
	    break;
	  x = c->location[0] + k;
	  y = c->location[1] + j;
	  if( characters[y][x] == NULL )
	    {
	      nummon++;
	      characters[y][x] = monsters[rand() % monsters.size()].spawn( x, y, nummon);
	      characters[y][x]->hn = heap_insert(turn_h, characters[y][x]);
	      count++;
	    }
	}
    }

  return 0;
}

int update_mods( PC *p )
{
  int i;
  
  p->dam = p->base_dam;
  p->def = p->base_def;
  p->speed = p->base_speed;
  
  for( i = 0; i < p->equip.size(); i++ )
    {
      p->dam += p->equip[i].dam_die.roll();
      p->def += p->equip[i].def;
      p->speed += p->equip[i].speed;
    }

  return 0;
} 

int get_color( string color )
{
  if( color.compare("BLACK") == 0 )
    return 5;
  if( color.compare("RED") == 0 )
    return 6;
  if( color.compare("GREEN") == 0 )
    return 7;
  if( color.compare("YELLOW") == 0 )
    return 8;
  if( color.compare("BLUE") == 0 )
    return 9;
  if( color.compare("MAGENTA") == 0 )
    return 10;
  if( color.compare("CYAN") == 0 )
    return 11;
  if( color.compare("WHITE") == 0 )
    return 12;
  return 6;
}

char get_symbol( string type )
{
  if( type.compare("WEAPON") == 0 )
    return '|';
  if( type.compare("OFFHAND") == 0 )
    return ')';
  if( type.compare("RANGED") == 0 )
    return '}';
  if( type.compare("ARMOR") == 0 )
    return '[';
  if( type.compare("HELMET") == 0 )
    return ']';
  if( type.compare("CLOAK") == 0 )
    return '(';
  if( type.compare("GLOVES") == 0 )
    return '{';
  if( type.compare("BOOTS") == 0 )
    return '\\';
  if( type.compare("RING") == 0 )
    return '=';
  if( type.compare("AMULET") == 0 )
    return '"';
  if( type.compare("LIGHT") == 0 )
    return '_';
  if( type.compare("SCROLL") == 0 )
    return '~';
  if( type.compare("BOOK") == 0 )
    return '?';
  if( type.compare("FLASK") == 0 )
    return '!';
  if( type.compare("GOLD") == 0 )
    return '$';
  if( type.compare("AMMUNITION") == 0 )
    return '/';
  if( type.compare("FOOD") == 0 )
    return ',';
  if( type.compare("WAND") == 0 )
    return '-';
  if( type.compare("CONTAINER") == 0 )
    return '%';
  if( type.compare("STACK") == 0 )
    return '&';
  return '?';
}
int print_d( char map[21][80], Character *characters[21][80], Obj *item_map[21][80], int pc_loc[2] )
{
  int i, j;

  int x = shop_loc[0];
  int y = shop_loc[1];
  int x_mid, y_mid;
  
  if( shop_loc[2] % 2 == 0 )
    x_mid = (shop_loc[2] / 2) - 1;
  else
    x_mid = shop_loc[2] / 2;

  if( shop_loc[3] % 2 == 0 )
    y_mid = (shop_loc[2] / 2) - 1;
  else
    y_mid = shop_loc[3] / 2;
  
  x_mid += x;
  y_mid += y;

  clear();

  attron(COLOR_PAIR(WHITE_D));
  mvprintw(0, 0, top_text.c_str());
  mvprintw(0, 40, top_text_n.c_str());
  attroff(COLOR_PAIR(WHITE_D));

  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++ )
	{
	  if( map[i][j] == '*' )
	    {
	      attron(COLOR_PAIR(PC_PAIR));
	      mvaddch(i+1, j, map[i][j]);
	      attroff(COLOR_PAIR(PC_PAIR));
	      continue;
	    }
	  if( map[i][j] == '@' )
	    {
	      attron(COLOR_PAIR(PC_PAIR));
	      mvaddch(i+1, j, map[i][j]);
	      attroff(COLOR_PAIR(PC_PAIR));
	      continue;
	    }
	  if( map[i][j] == ' ' || map[i][j] == '|' || map[i][j] == '-' )
	    {
	      attron(COLOR_PAIR(ROCK_PAIR));
	      mvaddch(i+1, j, map[i][j]);
	      attroff(COLOR_PAIR(ROCK_PAIR));
	      continue;
	    }
	  if( (j == x_mid || j == x_mid + 1) && (i == y_mid || i == y_mid + 1) )
	    {
	      attron(COLOR_PAIR(SHOP_PAIR));
	      mvaddch(i+1, j, ' ');
	      attroff(COLOR_PAIR(SHOP_PAIR));
	      continue;
	    }
	  if(characters[i][j] != NULL && ((NPC*) characters[i][j])->isa == 0 )
	    {
	      if( pc_view( pc_loc[0], pc_loc[1], j, i ) == 0 )
		{
		  if( (int) map[i][j] > 64 )
		    {
		      attron(COLOR_PAIR(get_color((((NPC*)characters[i][j])->color)[0])));
		      mvaddch(i+1, j, map[i][j]);
		      attroff(COLOR_PAIR(get_color(((NPC*)characters[i][j])->color[0])));
		    }
		  else
		    {
		      attron(COLOR_PAIR(OPEN_PAIR));
		      mvaddch(i+1, j, map[i][j]);
		      attroff(COLOR_PAIR(OPEN_PAIR));
		    }
		  continue;
		}
	      if( !(((NPC*)characters[i][j])->color).empty() )
		{
		  attron(COLOR_PAIR(get_color((((NPC*)characters[i][j])->color)[0])));
		  mvaddch(i+1, j, map[i][j]);
		  attroff(COLOR_PAIR(get_color(((NPC*)characters[i][j])->color[0])));
		  continue;
		}
	    }
	  if( item_map[i][j] != NULL )
	    {
	      attron(COLOR_PAIR(get_color(((item_map[i][j])->color)[0])));
	      mvaddch(i+1, j, get_symbol( item_map[i][j]->type ));
	      attroff(COLOR_PAIR(get_color(((item_map[i][j])->color)[0])));
	      continue;
	    }

	  else if( map[i][j] == '.' || map[i][j] == '#' || map[i][j] == '<' || map[i][j] == '>' )
	    {
	      attron(COLOR_PAIR(OPEN_PAIR));
	      mvaddch(i+1, j, map[i][j]);
	      attroff(COLOR_PAIR(OPEN_PAIR));
	    }
	  else if( map[i][j] == '@' )
	    {
	      attron(COLOR_PAIR(PC_PAIR));
	      mvaddch(i+1, j, map[i][j]);
	      attroff(COLOR_PAIR(PC_PAIR));
	    }
	}
    }
  refresh();
  return 0;
}

int corridors( int rooms[4][1000], int hardness[21][80] )
{
  int i, x, y, num, rand_num;
  for( i = 0; i < 9; i++ )
    {
      if( rooms[0][i] == -1 ||  rooms[0][i+1] == -1 )
	break;
      x = rooms[0][i];
      y = rooms[1][i];
      rand_num = rand() % 2;
      if( rand_num == 0 )
	{
	  while( x != rooms[0][i+1] && y != rooms[1][i+1] )
	    {
	      rand_num = rand() % 2;
	      if( rand_num == 0 )
      		{
		  if( x == rooms[0][i+1] )
		    continue;
		  if( hardness[y][x] > 0 && hardness[y][x] != 255 )
		      hardness[y][x] = 0;
		  num = (rooms[0][i+1] - x) / abs(rooms[0][i+1] - x);
		  x = x + num;
      		}
	      if( rand_num == 1 )
      		{
		  if( y == rooms[1][i+1] )
		    continue;
		  if( hardness[y][x] > 0 && hardness[y][x] != 255 )
		      hardness[y][x] = 0;
		  num = (rooms[1][i+1] - y) / abs(rooms[1][i+1] - y);
		  y = y + num; 
      		}
	    }
	}

      if( rand_num == 1 )
	{
	  while( x != rooms[0][i+1] )
	    {
	      if( hardness[y][x] > 0 && hardness[y][x] != 255 )
		hardness[y][x] = 0;
	      num = (rooms[0][i+1] - x) / abs(rooms[0][i+1] - x);
	      x = x + num;
	    }
	  while( y != rooms[1][i+1] )
	    {
	      if( hardness[y][x] > 0 && hardness[y][x] != 255 )
		hardness[y][x] = 0;
	      num = (rooms[1][i+1] - y) / abs(rooms[1][i+1] - y);
	      y = y + num; 
	    }
	}
      while( x != rooms[0][i+1] )
	{
	  if( hardness[y][x] > 0 && hardness[y][x] != 255 )
	    hardness[y][x] = 0;
	  num = (rooms[0][i+1] - x) / abs(rooms[0][i+1] - x);
	  x = x + num;
	}
      while( y != rooms[1][i+1] )
	{
	  if( hardness[y][x] > 0 && hardness[y][x] != 255 )
	    hardness[y][x] = 0;
	  num = (rooms[1][i+1] - y) / abs(rooms[1][i+1] - y);
	  y = y + num; 
	}
    }
  return 0;  
}

int stairsANDPC( int hardness[21][80], int stairs[2][9], int pc[2], int rooms[4][1000], int shop_num )
{
  int x, y, done;
  done = 0;
  
  stairs[0][0] = 1;
  stairs[1][0] = 1;
  while( !done ) //upstairs
    {
      stairs[0][1] = (rand() % 78) + 1;
      stairs[0][2] = (rand() % 19) + 1;
      x = stairs[0][1];
      y = stairs[0][2];
  
      if( hardness[y][x] == 0 && inside_shop( rooms, shop_num, x, y ) == 0 )
	  done = 1;
    }
  done = 0;
  while( !done ) //downstairs
    {
      stairs[1][1] = (rand() % 78) + 1;
      stairs[1][2] = (rand() % 19) + 1;
      x = stairs[1][1];
      y = stairs[1][2];
  
      if( hardness[y][x] == 0 && inside_shop( rooms, shop_num, x, y ) == 0 )
	  done = 1;
    }
  done = 0;
  while( !done )
    {
      pc[0] = (rand() % 78) + 1;
      pc[1] = (rand() % 19) + 1;
  
      if( hardness[pc[1]][pc[0]] == 0 )
	  done = 1;
    }
  return 0;
}

int generate_room( int rooms[4][1000] ) //generates on room and adds it onto the current list of rooms. If the list of rooms is full does nothing
{
  int i;
  for( i = 0; i < 10; i++ )
    {
      if( rooms[0][i] == -1 )
	{
	  rooms[0][i] = (rand() % 78) + 1;
	  rooms[1][i] = (rand() % 19) + 1;
	  rooms[2][i] = (rand() % 5) + 6;
	  rooms[3][i] = (rand() % 5) + 5;
	  break;
	}
    }
  return 0;
}

int initialize_area( int hardness[21][80] )
{
	int i, j;
	for( i = 0; i < 21; i++ )
	{
		for( j = 0; j < 80; j++ )
		{
		  hardness[i][j] = rand() % 254 + 1;
		}
	}

	for( i = 0; i < 21; i++ )
	{
		for( j = 0; j < 80; j++ )
		{
			if( i == 0 || i == 20 )
			{
				hardness[i][j] = 255;
			}
			else if( j == 0 || j == 79 )
			{
				hardness[i][j] = 255;
			}
		}
	}
	return 0;
}

static int32_t corridor_path_cmp(const void *key, const void *with) {
  return ((dungeon_path *) key)->cost - ((dungeon_path *) with)->cost;
}

static void dijkstra_tunneling( int hardness[21][80], int pc[2], int distance_map[21][80])
{
  static dungeon_path path[21][80], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < 21; y++) {
      for (x = 0; x < 80; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < 21; y++) {
    for (x = 0; x < 80; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[pc[1]][pc[0]].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < 21; y++) {
    for (x = 0; x < 80; x++) {
      if (hardness[y][x] != 255) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = (dungeon_path*) heap_remove_min(&h))) {
    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
				   [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
				   [p->pos[dim_x] + 1].hn);
    }
  }

  int i,j;
  
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++)
	{
	  distance_map[i][j] = path[i][j].cost;
	}
    }
}

static void dijkstra_non_tunneling( int hardness[21][80], int pc[2], int distance_map[21][80])
{
  static dungeon_path path[21][80], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < 21; y++) {
      for (x = 0; x < 80; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < 21; y++) {
    for (x = 0; x < 80; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  path[pc[1]][pc[0]].cost = 0;

  heap_init(&h, corridor_path_cmp, NULL);

  for (y = 0; y < 21; y++) {
    for (x = 0; x < 80; x++) {
      if (hardness[y][x] == 0) {
        path[y][x].hn = heap_insert(&h, &path[y][x]);
      } else {
        path[y][x].hn = NULL;
      }
    }
  }

  while ((p = (dungeon_path*) heap_remove_min(&h))) {
    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y]    ][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
				   [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost >
         p->cost + hardnesspair(p->pos))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost =
        p->cost + hardnesspair(p->pos);
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
				   [p->pos[dim_x] + 1].hn);
    }
    if( path[p->pos[dim_y]][p->pos[dim_x]].cost == INT_MAX )
      {
	path[p->pos[dim_y]][p->pos[dim_x]].cost = -1;
      }
  }
  int i,j;
  
  for( i = 0; i < 21; i++ )
    {
      for( j = 0; j < 80; j++)
	{
	  distance_map[i][j] = path[i][j].cost;
	}
    }
}
