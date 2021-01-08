#pragma once
#include <vector>
#include <functional>
#include <iostream>
// windows dependencies
#include "../framework/Window.h"

/// retrieves console input asynchronously.
/// can be connected with cout and cerr via cout.set_rdbuf(this)
class AsyncInput : public std::streambuf
{
public:
	using HANDLE = void*;

	AsyncInput();
	/// \param in console input handle
	/// \param out console output handle
	AsyncInput(HANDLE in, HANDLE out);

	/// \brief set callback functions that provides word suggestions when a tab was pressed
	/// \param func the parameter is the current input and the return values should be word continuation suggestions
	void setAutoCompleteProvider(std::function<std::vector<std::string>(const std::string&)> func);

	/// \brief checks if new input is available
	/// \return empty string or input if available
	std::string get();

	/// \brief returns the current input
	std::string peek() const;

protected:
	/// \brief put character handler from the std::streambuf
	int_type overflow(int_type i) override;

private:
	void write(char c);
	void write(const std::string_view& text);
	/// \brief erases the last numChars chars
	void erase(size_t numChars);
	/// \brief takes the word from the history as new input
	void selectFromHistory(size_t index);
	/// \brief moves the cursor directions characters to the left or right
	void moveCursor(int direction);
	/// \brief retrieves new suggestions from the callback function
	void refreshWordSuggestions();

	WORD getTextColor(char c);

	// console handles
	HANDLE m_in;
	HANDLE m_out;
	// part left of the console cursor
	std::string m_inputLeft;
	// part right of the console cursor
	std::string m_inputRight;
	bool m_tabPressed = false;

	std::vector<std::string> m_history;
	size_t m_currentWordIndex = 0;

	std::function<std::vector<std::string>(const std::string&)> m_autoCompleteFunc;
	std::vector<std::string> m_wordSuggestions;
	std::vector<std::string>::iterator m_lastSuggestion;
	bool m_isString = false;
	bool m_prevCharDigit = false;
};