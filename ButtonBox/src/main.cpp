#include <Arduino.h>
#include <Joystick.h>
 
// JP1 is an input
int rows[] = {4,5,6,7,8,9};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// JP2 and JP3 are outputs
int cols[] = {10,16,14,15};
const int colCount = sizeof(cols)/sizeof(cols[0]);

int keys[colCount][rowCount];
int butonNumbers[colCount][rowCount] = {
  {4, 8, 12, 16, 20, 21},
  {2, 7, 11, 15, 19, 0},
  {3, 6, 10, 14, 18, 0},
  {1, 5, 9, 13, 17, 0}
};

void readMatrix();

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  21, 0,
  false, false, false,
  false, false, false,
  false, false,
  false, false, false);

void setup() {
  Joystick.begin();
	// Serial.begin(9600);
  // while (!Serial)
  // {
  //   delay(25);
  // }
  

	for(int x=0; x<rowCount; x++) {
		// Serial.print(rows[x]); Serial.println(" as input-pullup");
		pinMode(rows[x], INPUT_PULLUP);
	}

	for (int x=0; x<colCount; x++) {
		// Serial.print(cols[x]); Serial.println(" as output-high");
		pinMode(cols[x], INPUT);
	}
		
}

void loop() {
	readMatrix();
}

void readMatrix() {
	// iterate the columns
	for (int colIndex=0; colIndex < colCount; colIndex++) {
		// col: set to output to low
		byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
		digitalWrite(curCol, LOW);

		// row: interate through the rows
		for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
			byte rowCol = rows[rowIndex];
      bool currentState = digitalRead(rowCol);
			if (currentState != keys[colIndex][rowIndex]) {
				keys[colIndex][rowIndex] = currentState;
        Joystick.setButton(butonNumbers[colIndex][rowIndex] - 1, !currentState);

        // Serial.print("Button "); Serial.print(butonNumbers[colIndex][rowIndex]);
        // Serial.print(" (col "); Serial.print(colIndex); Serial.print(", row "); Serial.print(rowIndex); Serial.print(") ");
        // Serial.print(" is "); Serial.println(!currentState ? "PRESSED" : "RELEASED");
			}
		}
		// disable the column
    pinMode(curCol, INPUT);
	}

  // send the state to the joystick for polling rates
  Joystick.sendState();
  delay(5);
}
