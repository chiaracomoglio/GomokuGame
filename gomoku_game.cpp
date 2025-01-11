#include <iostream>
#include <vector>
#include <stdexcept>

// Number of rows and coloumns is set to 19 for simplicity
const int righe_Goban = 19;
const int colonne_Goban = 19;

// The Pedina class is represented by a color ('B','N','.') and is used to distinguish the moves of one player from those of the other;
// the character '.' means that the square is empty
class Pedina {
private:
  char colore;
public:
  // Constructor declaration
  Pedina();
  // To access the private field 'colore' (returns the character representing the color of the piece)
  char restituisci_colore() const { return colore; } 
  // Method that allows setting the color of the piece when it is placed on the board (the piece to which it is applied takes on the set color)
  void imposta_colore(char colore_impostato) { colore = colore_impostato; } 
  // Recognizes if a piece is still nonexistent
  bool inesistente() const { return restituisci_colore() == '.'; } 
};

// Constructor definition
Pedina::Pedina() { 
  colore = '.';
}

// The Goban class is a vector of vectors of pieces (Pedine) and represents the board on which the game takes place;  
// the number of rows and columns of the Goban are assigned to two fields, which take the values provided as input during the creation of the Goban
class Goban {
private:
  int numero_righe; // numero_righe >= 0
  int numero_colonne; // numero_colonne > 0
  std::vector<std::vector<Pedina> > data; // data.size() == numero_righe
                                          // 0 <= i < numero_righe : data[i].size() == numero_colonne
public:
  // The constructor is declared with two arguments, which represent the number of rows and columns that are provided to create the Goban
  Goban(int, int);
  void stampa_Goban() const;
  bool posizione_valida(int, int) const;
  bool occupato(int, int) const; 
  bool controllo_vittoria(Pedina, int, int);
  int conta_Pedine(int, int, Pedina, int, int);
  bool mossa(char);
  void partita();
};

// Constructor definition: if the number of rows and columns provided as input is not positive, an exception is thrown
Goban::Goban(int _righe, int _colonne) {
  if (_righe < 0 || _colonne < 0) throw std::domain_error("dimensione del Goban non valida");
  numero_righe = _righe;
  numero_colonne = _colonne;
  // The size becomes that of the rows; the elements added to the end of the vector are vectors of pieces, 
  // each with a length equal to the number of columns
  data.resize(numero_righe, std::vector<Pedina>(numero_colonne) );
}

// Print the Goban: at each iteration, the color of the piece located at [i][j] is displayed.
// To properly display the board, the characters are spaced apart, and after printing each row, a new line is added.
void Goban::stampa_Goban() const {
  for (int i = 0; i < righe_Goban; i++) {
    for (int j = 0; j < colonne_Goban; j++) {
      char colore_sullo_schermo = data[i][j].restituisci_colore();
      std::cout << colore_sullo_schermo << ' ' ;
    }
    std::cout << std::endl;
    } 
}

// Method that checks if we are within the bounds of the Goban
bool Goban::posizione_valida(int i, int j) const {
  return (i >= 0 && i < righe_Goban && j >= 0 && j < colonne_Goban);
}

// Method that checks whether the square is occupied by a piece or not
bool Goban::occupato (int i, int j) const {
  return !data[i][j].inesistente();
}

// Method based on the boolean value of 'vittoria', which becomes true if the count of pieces of a certain color
// in at least one of the allowed directions is greater than or equal to 5;
// the input consists of the piece just placed and its position within the Goban
bool Goban::controllo_vittoria(Pedina pedina, int riga, int colonna) {
  // Introduce 4 directions (row, coloumn, diagonal, anti-diagonal) to be passed as parameters to the method conta_Pedine
  // 0 1 : row
  // 1 0 : coloumn
  // 1 1 : diagonal
  // 1 -1 : anti-diagonal
  bool vittoria = false;
  // If the count in one direction is greater than or equal to 5, the count in the subsequent directions is skipped
  if ((conta_Pedine(0, 1, pedina, riga, colonna) >= 5) || 
      (conta_Pedine(1, 0, pedina, riga, colonna) >= 5) ||
	  (conta_Pedine(1, 1, pedina, riga, colonna) >= 5) ||
	  (conta_Pedine(1, -1, pedina, riga, colonna) >= 5)) 
  vittoria = true;
  return vittoria;
}

// Method that handles the counting of pieces in the direction represented by the integers 'horizontal' and 'vertical';
// the piece and the row and column indices where it has been placed are passed as input to perform the evaluation
int Goban::conta_Pedine(int orizzontale, int verticale, Pedina pedina, int riga, int colonna) { 
  // 'horizontal' and 'vertical' represent the shifts along the x and y axes, respectively
  int pedine_allineate = 1;
  Pedina pedina_vicina; // pedina_vicina rappresents the piece whose color I want to evaluate each time, in order to compare it with the color of the piece just placed
  char colore = pedina.restituisci_colore();
  // Counting the pieces 'preceding' the one just placed
  // Initial conditions:
  int i = riga-orizzontale;
  int j = colonna-verticale;
  // If the position is valid, pedina_vicina takes the value of the piece at position [i][j];
  // otherwise, the while loop is not entered as it stops at the negation of the first necessary condition
  if (posizione_valida(i,j)) pedina_vicina = data[i][j];
  while ( posizione_valida(i,j) && occupato(i,j) && (pedina_vicina.restituisci_colore() == colore)) {
  	pedine_allineate++;
	i-=orizzontale;
	j-=verticale;
  }
  // Counting the pieces 'following' the one just placed
  // Initial conditions:
  i = riga+orizzontale;
  j = colonna+verticale;
  if (posizione_valida(i,j)) pedina_vicina = data[i][j];
  while ( posizione_valida(i,j) && occupato(i,j) && (pedina_vicina.restituisci_colore() == colore)) {
  	pedine_allineate++;
	i+=orizzontale;
	j+=verticale;
  }
  return pedine_allineate;
}

// Method that handles the move of the player who owns the pieces of the color provided as input;
// a piece is created, its color is set, the Goban is printed, the move is requested, and after verifying its validity,
// a check for a possible victory is performed
bool Goban::mossa(char colore_turno) {
  Pedina pedina_inserita;
  int riga_inserita, colonna_inserita;
  stampa_Goban();
  pedina_inserita.imposta_colore(colore_turno);
  std::cout << "Inserire la mossa (riga e colonna)" << std::endl;
  std::cin >> riga_inserita;
  std::cin >> colonna_inserita;
  if (!posizione_valida(riga_inserita-1, colonna_inserita-1))
    throw std::domain_error("mossa inserita non valida");
  if (occupato(riga_inserita-1, colonna_inserita-1)) 
	throw std::domain_error("casella occupata");
  data[riga_inserita-1][colonna_inserita-1] = pedina_inserita;
  return controllo_vittoria(pedina_inserita, riga_inserita-1, colonna_inserita-1);
}

// Method that manages the game; when partita_finita takes a positive value, the game ends with the winner's color displayed
void Goban::partita() {
  bool partita_finita = false;
  while (!partita_finita) {
    // The first move is always made by the Black player
	partita_finita = mossa('N');
    if (partita_finita == true)
	  std::cout << "Vittoria del Nero";
    else {
      // Now it's the White player's turn
      partita_finita = mossa('B');
      if (partita_finita == true)
	    std::cout << "Vittoria del Bianco";
    } 
  }
}

// A Goban of the specified size (in this case, 19x19) is created, and the game begins
int main() {
  Goban goban(righe_Goban, colonne_Goban);
  goban.partita();
}
