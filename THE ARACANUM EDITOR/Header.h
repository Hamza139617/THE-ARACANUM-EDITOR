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

// for the settings part 

// main layout is below 



class lines {
private:
	int current;
	wchar_t* sentence = nullptr;
	int max;
public:
	lines() {
		current = 0;
		max = 40;
		sentence = new wchar_t[max];
		sentence[0] = '\0';
	}

	lines(int m) {

		current = 0;
		max = m;
		sentence = new wchar_t[max];
		sentence[0] = '\0';
	}

	lines(const lines& other) {
		max = other.max;
		current = other.current;
		sentence = new wchar_t[max];

		for (int i = 0; i <= current; i++)
			sentence[i] = other.sentence[i];



	}

	int getCurrent() const { return current; }
	int getMax() const { return max; }

	void setCurrent(int n) { current = n; }

	bool addChar(wchar_t c) {
		if (current < max - 1) {
			sentence[current] = c;
			current++;
			sentence[current] = L'\0';
			return true;
		}
		return false;
	}

	void incrementLine() {
		current++;
	}

	wchar_t* getSentence() const { return sentence; }

	void setMax(int m) { max = m; }

	~lines() {
		delete[] sentence;
	}

};

class Columns {

private:
	int maxLines;
	int currentLine;
	lines** cols = nullptr;
	
public:
	Columns() {
		maxLines = 20;
		currentLine = 0;
		cols = new lines*[maxLines];
		int maxCh = 40;
		

		for (int i = 0; i < maxLines; i++)
			cols[i] = new lines(maxCh);

	}

	Columns(int n,int maxC) {
		maxLines = n;
		currentLine = 0;
		
		cols = new lines * [maxLines];

		for (int i = 0; i < maxLines; i++)
			cols[i] = new lines(maxC);

		
	}

	void setNumOfLine(int n) {
		maxLines = n;
		return;
	}

	
	int getNumOfLine() const { return maxLines; }
	int getCurrentLine() const { return currentLine; }

	lines* getLine(int index) const { return cols[index]; }

	void incrementLine() {
		currentLine++;
	}

	~Columns() {
		for (int i = 0; i < maxLines; i++)
			delete cols[i];

		delete[] cols;
	}

};

class Pages {
private:
	int currentCol;
	int maxCol;
	Columns** pages = nullptr;

public:
	Pages() {
		currentCol = 0;
		maxCol = 6;

		pages = new Columns * [maxCol];

		for (int i = 0; i < maxCol; i++)
			pages[i] = new Columns;

	}

	Pages(int max, int numOfLines, int maxCharacters) {
		this->maxCol = max;
		currentCol = 0;

		pages = new Columns * [max];

		for (int i = 0; i < max; i++)
			pages[i] = new Columns(numOfLines, maxCharacters);
	}

	void setMaxColumns(int maxC) {
		maxCol = maxC;
		return;
	}

	int getCurrentColumns() const {
		return currentCol;
	}

	int getMaxColumns() const {
		return maxCol;
	}

	Columns* getColumn(int index) { return pages[index]; }

	void incrementCol() {
		currentCol++;
	}

	~Pages() {

		for (int i = 0; i < maxCol; i++)
			delete pages[i];

		delete[] pages;

	}

};

class Document {
private:
	int currentPage;
	int maxPage;
	Pages** docs;

public:
	Document() {
		currentPage = 0;
		maxPage = 1;

		docs = new Pages * [maxPage];
		
		for (int i = 0; i < maxPage; i++)
			docs[i] = new Pages;
	}

	Document(int maxPage, int maxCols, int maxLines, int maxChars) {
		currentPage = 0;
		this->maxPage = maxPage;

		docs = new Pages * [maxPage];

		for (int i = 0; i < maxPage; i++)
			docs[i] = new Pages(maxCols, maxLines, maxChars);

	}

	int getCurrentPage() const { return currentPage; }
	int getMaxPage() const { return maxPage; }
	Pages* getPage(int index) { return docs[index]; }

	void setMaxPage(int max) { maxPage = max; }

	void incrementPage() {
		currentPage++;
	}

	~Document() {

		for (int i = 0; i < maxPage; i++)
			delete docs[i];

		delete[] docs;

	}


};
