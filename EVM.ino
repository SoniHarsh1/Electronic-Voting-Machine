#include <Keypad.h>
#include <LiquidCrystal.h>

#define S1 12
#define S2 13
#define S3 6
#define S4 2
String party = "";
int partyNumber = 0;
int votes[100] = {0};
int totalParties = 99;
int totalVotes = 0;
int check = 0;

LiquidCrystal lcd(5, 4, 3, A3, A4, A5);


const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A0, A1, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7}; //connect to the column pinouts of the keypad
int LCDCol = 0;
int LCDRow = 0;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup(){
   Serial.begin(9600);
   pinMode(S1, INPUT);
   pinMode(S2,INPUT);
   pinMode(S3, INPUT);
   pinMode(S4, INPUT);
   lcd.begin(16, 2);
  	lcd.print(" Electronic ");
	lcd.setCursor(0,1);
	lcd.print(" Voting Machine ");
	delay(4000);
   digitalWrite(S1, HIGH);
   digitalWrite(S2, HIGH);
   digitalWrite(S3, HIGH);
   digitalWrite(S4, HIGH);
   lcd.clear();
   lcd.setCursor(LCDCol, LCDRow);
   }
  
void tagline(){
  lcd.setCursor(0, 0);
  lcd.print("Enter Party No.");
  lcd.setCursor(0, 1);
  LCDRow = 1;
  LCDCol = 0;
}

void invalidParty(){
  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please Enter");
    lcd.setCursor(0, 1);
    lcd.print("Valid Number..");
    delay(2000);
    lcd.clear();
    party = "";
}

void castVote(){
	int partyNo = 0;
    partyNo = party.toInt();
    party = "";
    votes[partyNo] = votes[partyNo] + 1;
    totalVotes = totalVotes + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Your Vote Casted");
    lcd.setCursor(0, 1);
    lcd.print("Successfully..");
    delay(5000);
    lcd.clear();
    check = 0;
}

void resetVote(){
  party = "";
  lcd.clear();
}

void displayVotes(){
  int partyNo = 0;
    partyNo = party.toInt();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Votes for Party");
    lcd.setCursor(0, 1);
    lcd.print(party);
    lcd.print(" are ");
    lcd.print(votes[partyNo]);
    party = "";
    delay(2000);
    lcd.clear();  
}

void evmNotEnable(){
    lcd.setCursor(0, 0);
    lcd.print("EVM Not Enabled");
}

void displayTotalVotes(){
  lcd.clear();
  lcd.setCursor(0, 0);
    lcd.print("Total Votes Cast");
    lcd.setCursor(0, 1);
    lcd.print("ed are ");
    lcd.print(totalVotes);
    delay(2000);
    lcd.clear();
}

void loop(){
  if(check==1){
  
  tagline();
  if(party.toInt()>totalParties){
    invalidParty();
  }
  else{
  lcd.print(party);
  }
    
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
    
    if(key=='#'){
      castVote();
    }
    else if(key == '*'){
      resetVote();
    }
    else{
    	party = party + key;
    }
       
  }
  
  }
  if(check == 0){
    evmNotEnable();
  }
  if(digitalRead(S1)==0){
    int maxVotes = 0;
    int winnerParty = 0;
    int maxVotesCount = 0;
    for(int i = 1; i<sizeof(votes)/sizeof(votes[0]); i= i+1){
      if(votes[i]>maxVotes){
        winnerParty = i;
        maxVotes = votes[i];
      }
    }
    for(int i = 1; i<sizeof(votes)/sizeof(votes[0]); i= i+1){
      if(votes[i]==maxVotes){
        winnerParty = i;
        maxVotesCount =  maxVotesCount+ 1;
      }
    }
    lcd.clear();
    if(maxVotes==0){
    lcd.print("No Voting....");
    }
    
    // Concept of NOTA
    /*else if(winnerParty==0){
    lcd.print("NOTA ");
    lcd.print(" Wins");
    }*/
    
    
    else if(maxVotesCount==1){
	lcd.print("Party ");
    lcd.print(winnerParty);
    lcd.print(" Wins");
    }
    else{
    lcd.print(" Tie Up Or ");
	lcd.setCursor(0,1);
	lcd.print(" No Result ");
    }
	delay(3000);
	lcd.clear();
  }
  else if(digitalRead(S2)==0){
    for(int i = 0; i<sizeof(votes)/sizeof(votes[0]); i= i+1){
      votes[i] = 0;
    }
    totalVotes = 0;
    lcd.clear();
	lcd.print("EVM reseted ...");
	delay(3000);
	lcd.clear();
  }
  else if(digitalRead(S3)==0){
  	displayVotes();
  }
  else if(digitalRead(S4)==0){
    check = 1 - check;
  }
}