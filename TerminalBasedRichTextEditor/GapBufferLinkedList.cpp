#include "GapBufferLinkedList.h"

GapBufferLinkedList::Line::Line(int capacity)
	: gapBuffer(capacity), prev(nullptr), next(nullptr) {
}

GapBufferLinkedList::GapBufferLinkedList()
	: head(new Line()), currentLine(head), cursorRow(0) {
}

GapBufferLinkedList::~GapBufferLinkedList() {
	while (head) {
		Line* temp = head;
		head = head->next;
		delete temp;
	}
}

void GapBufferLinkedList::insert(char c) {
	if (c == '\n') {
		splitLine();
	}
	else {
		currentLine->gapBuffer.insert(c);
	}
}

void GapBufferLinkedList::insert(const string& s) {
	for (char c : s) {
		insert(c);
	}
}

void GapBufferLinkedList::deleteCharLeft() {
	if (getCursorColumn() > 0) {
		currentLine->gapBuffer.deleteCharLeft();
	}
	else if (currentLine->prev) {
		int prev_ling_length = currentLine->prev->gapBuffer.getLength();
		mergeLine();
		currentLine->gapBuffer.moveCursor(prev_ling_length);
	}
}

void GapBufferLinkedList::deleteCharRight() {
	if (getCursorColumn() < currentLine->gapBuffer.getLength()) {
		currentLine->gapBuffer.deleteCharRight();
	}
	else if (currentLine->next) {
		currentLine->gapBuffer.deleteCharRight();
		mergeLine();
	}
}

void GapBufferLinkedList::moveCursorUp() {
	if (!currentLine->prev)
		return;
	int currentColumn = getCursorColumn();
	currentLine = currentLine->prev;
	currentLine->gapBuffer.moveCursor(min(currentColumn, currentLine->gapBuffer.getLength()));
	--cursorRow;
}

void GapBufferLinkedList::moveCursorDown() {
	if (!currentLine->next)
		return;
	int currentColumn = getCursorColumn();
	currentLine = currentLine->next;
	currentLine->gapBuffer.moveCursor(min(currentColumn, currentLine->gapBuffer.getLength()));
	++cursorRow;
}

void GapBufferLinkedList::moveCursorLeft() {
	if (getCursorColumn() == 0 && currentLine->prev) {
		currentLine = currentLine->prev;
		currentLine->gapBuffer.moveCursor(currentLine->gapBuffer.getLength());
		--cursorRow;
	}
	else
		currentLine->gapBuffer.moveCursorRelative(-1);
}

void GapBufferLinkedList::moveCursorRight() {
	if (getCursorColumn() == currentLine->gapBuffer.getLength() && currentLine->next) {
		currentLine = currentLine->next;
		currentLine->gapBuffer.moveCursor(0);
		++cursorRow;
	}
	else
		currentLine->gapBuffer.moveCursorRelative(1);
}

void GapBufferLinkedList::moveCursor(int x, int y) {
	Line* temp = head;
	for (int i = 0; i < x; i++) {
		if (!temp->next)
			break;
		temp = temp->next;
	}
	currentLine = temp;
	currentLine->gapBuffer.moveCursor(y);
}

void GapBufferLinkedList::display() const {
	Line* temp = head;
	while (temp) {
		temp->gapBuffer.display(-1, -1);
		temp = temp->next;
	}
}

void GapBufferLinkedList::displayCurrentLine(int from = 0, int to = -1) const {
	currentLine->gapBuffer.display(from, to);
}

int GapBufferLinkedList::getCursorRow() const {
	return cursorRow;
}

int GapBufferLinkedList::getCursorColumn() const {
	return currentLine->gapBuffer.getCursorPosition();
}

int GapBufferLinkedList::getLinesCount() const {
	int count = 0;
	Line* temp = head;
	while (temp) {
		++count;
		temp = temp->next;
	}
	return count;
}

pair<int, int> GapBufferLinkedList::getCursorPosition() const {
	return { getCursorRow(), getCursorColumn() };
}

void GapBufferLinkedList::splitLine() {
	int cursorPos = currentLine->gapBuffer.getCursorPosition();
	string rightContent = currentLine->gapBuffer.getTextAfterCursor();
	currentLine->gapBuffer.deleteToEnd();
	Line* newLine = new Line();
	newLine->prev = currentLine;
	newLine->next = currentLine->next;
	if (currentLine->next) {
		currentLine->next->prev = newLine;
	}
	currentLine->next = newLine;
	newLine->gapBuffer.insert(rightContent);
	currentLine = newLine;
	currentLine->gapBuffer.moveCursor(0);
	++cursorRow;
}

void GapBufferLinkedList::mergeLine() {
	Line* temp = currentLine;
	currentLine = currentLine->prev;
	currentLine->gapBuffer.insert(temp->gapBuffer.getTextAfterCursor());
	currentLine->next = temp->next;
	if (temp->next) {
		temp->next->prev = currentLine;
	}
	delete temp;
	--cursorRow;
}

vector<string> GapBufferLinkedList::getLines(int from, int to) const {
	vector<string> lines;
	Line* temp = head;
	for (int i = 0; i < from; ++i) {
		temp = temp->next;
	}
	for (int i = from; i <= to; ++i) {
		lines.push_back(temp->gapBuffer.getLine());
		temp = temp->next;
	}
	return lines;
}