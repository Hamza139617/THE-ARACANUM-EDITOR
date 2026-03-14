#pragma once

class CustomSettings {
private:
	bool screenState;
	int numOfColumns;
	int numOfLines;
	int numOfChar;
public:
	CustomSettings() {
		numOfColumns = 2;
		numOfLines = 20;
		numOfChar = 40;
		screenState = false;
	}

	CustomSettings(int c, int l, int ch, bool st) {
		numOfColumns = c;
		numOfLines = l;
		numOfChar = ch;
		screenState = st;
	}

	void setColumns(int n) {
		numOfColumns = n;
		return;
	}

	void setLines(int n) {
		numOfLines = n;
		return;
	}

	void setChar(int n) {
		numOfChar = n;
		return;
	}

	void setState(bool s) {
		screenState = s;
		return;
	}

	int getColumns() const { return numOfColumns;  }
	int getLines() const { return numOfLines; }
	int getChars() const { return numOfChar; }
	bool getState() const { return screenState; }

	void validSettings() {

		numOfColumns = ((numOfColumns > 0 && numOfColumns <= 6) ? numOfColumns : 2);
		numOfChar = ((numOfChar >= 40 && numOfChar <= 100) ? numOfChar : 40);
		numOfLines = ((numOfLines >= 20 && numOfLines <= 45) ? numOfLines : 25);

	}

} appSettings;