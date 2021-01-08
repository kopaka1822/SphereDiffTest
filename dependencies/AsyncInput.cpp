#include "AsyncInput.h"
#include <cassert>

AsyncInput::AsyncInput()
	:
	AsyncInput(GetStdHandle(STD_INPUT_HANDLE), GetStdHandle(STD_OUTPUT_HANDLE))
{
}

AsyncInput::AsyncInput(HANDLE in, HANDLE out)
	:
	m_in(in),
	m_out(out),
	m_lastSuggestion(m_wordSuggestions.end())
{}

void AsyncInput::setAutoCompleteProvider(std::function<std::vector<std::string>(const std::string&)> func)
{
	m_autoCompleteFunc = move(func);
}

std::string AsyncInput::get()
{
	INPUT_RECORD ir;
	DWORD cNumRead;

	while (true)
	{
		if (!PeekConsoleInputA(
			m_in,
			&ir,
			1,
			&cNumRead))
			break;

		// nothing available to read
		if (!cNumRead) break;

		// process the data
		if (!ReadConsoleInputA(
			m_in,
			&ir,
			1,
			&cNumRead))
			break;

		if (!cNumRead) break;
		if (ir.EventType != KEY_EVENT) continue;

		auto& e = ir.Event.KeyEvent;
		if (!e.bKeyDown) continue;

		// reset and remember tab state
		const auto prevTabPressed = m_tabPressed;
		m_tabPressed = false;

		switch (e.wVirtualKeyCode)
		{
		case VK_RETURN: // accept m_inputLeft
		{
			moveCursor(int(m_inputRight.size()));
			write("\n");

			// add to history
			auto res = m_inputLeft + m_inputRight;
			m_history.push_back(res);
			// reset some settings
			m_currentWordIndex = m_history.size();
			m_inputLeft.clear();
			m_inputRight.clear();

			return res;
		}
		case VK_TAB: // auto complete
		{
			if (!prevTabPressed)
				refreshWordSuggestions();

			m_tabPressed = true;

			if (m_wordSuggestions.empty()) break;

			// erase right part
			moveCursor(int(m_inputRight.size()));
			erase(m_inputRight.size());

			// take the next suggestion
			if (m_lastSuggestion != m_wordSuggestions.end())
			{
				// erase previous suggestion and right part
				erase(m_lastSuggestion->size());

				m_inputLeft.erase(m_inputLeft.size() - m_lastSuggestion->size());
				++m_lastSuggestion;
			}

			// print out new suggestion
			if (m_lastSuggestion == m_wordSuggestions.end())
				m_lastSuggestion = m_wordSuggestions.begin();

			write(*m_lastSuggestion);
			write(m_inputRight);
			moveCursor(-int(m_inputRight.size()));

			m_inputLeft.append(*m_lastSuggestion);
		}
		break;
		case VK_UP:
			// display previous word
			if (m_currentWordIndex > 0)
				selectFromHistory(m_currentWordIndex - 1);
			break;
		case VK_DOWN:
			// display next word
			if (m_currentWordIndex + 1 < m_history.size())
				selectFromHistory(m_currentWordIndex + 1);
			break;
		case VK_LEFT:
			if (m_inputLeft.empty()) break;

			moveCursor(-1);
			// move the last char from left to the first char from right
			m_inputRight.insert(0, 1, m_inputLeft.back());
			m_inputLeft.pop_back();
			break;
		case VK_HOME: // pos1 key
			if (m_inputLeft.empty()) break;

			moveCursor(-int(m_inputLeft.size()));
			// move all chars from left to right
			m_inputRight = m_inputLeft + m_inputRight;
			m_inputLeft.clear();
			break;
		case VK_RIGHT:
			if (m_inputRight.empty()) break;

			moveCursor(1);
			// move the first char from right to the last char from left
			m_inputLeft.push_back(m_inputRight.front());
			m_inputRight.erase(0, 1);
			break;
		case VK_END: // end key
			if (m_inputRight.empty()) break;

			moveCursor(int(m_inputRight.size()));
			// move all chars from right to left
			m_inputLeft += m_inputRight;
			m_inputRight.clear();
			break;
		case VK_BACK:
			// delete last m_inputLeft
			if (m_inputLeft.length())
			{
				moveCursor(int(m_inputRight.size()));
				erase(m_inputRight.size() + 1);
				write(m_inputRight);
				moveCursor(-int(m_inputRight.size()));
				m_inputLeft.pop_back();
			}
			break;
		case VK_DELETE:
			// delete first m_inputRight
			if (!m_inputRight.empty())
			{
				moveCursor(int(m_inputRight.size()));
				erase(m_inputRight.size());
				m_inputRight.erase(0, 1);
				write(m_inputRight);
				moveCursor(-int(m_inputRight.size()));
			}
			break;
		default:
			// don't print the null character or some control characters
			if (e.uChar.UnicodeChar > 31)
			{
				const auto c = char(e.uChar.AsciiChar);
				write(std::string(1, c) + m_inputRight);
				moveCursor(-int(m_inputRight.size()));
				m_inputLeft.push_back(c);
			}
			else
			{
				// some random key was pressed. just preserve the tab state
				m_tabPressed = prevTabPressed;
			}
			break;
		}
	}

	return "";
}

std::string AsyncInput::peek() const
{
	return m_inputLeft + m_inputRight;
}

std::basic_streambuf<char>::int_type AsyncInput::overflow(int_type i)
{
	if (i == std::char_traits<char>::eof()) return 0;

	try
	{
		if (!m_inputLeft.empty() || !m_inputRight.empty())
		{
			// erase input and write it below
			moveCursor(int(m_inputRight.size()));
			erase(m_inputLeft.size() + m_inputRight.size());
		}

		// write character
		const char c = char(i);
		write(std::string(1, c));

		// write previous values again
		if (!m_inputLeft.empty() || !m_inputRight.empty())
		{
			// erase input and write it below
			write(m_inputLeft);
			write(m_inputRight);
			moveCursor(-int(m_inputRight.size()));
		}
	}
	catch (const std::exception&)
	{
		// usually caused by user mouse interaction
	}

	return 0;
}

void AsyncInput::write(char c)
{
	SetConsoleTextAttribute(m_out, getTextColor(c));

	DWORD written;
	if(!WriteConsoleA(m_out, &c, 1, &written, nullptr))
		throw std::runtime_error("WriteConsoleA");
}

void AsyncInput::write(const std::string_view& text)
{
	for (const auto& c : text)
		write(c);
}

void AsyncInput::erase(size_t numChars)
{
	for (size_t i = 0; i < numChars; ++i)
		write("\b \b");
}

void AsyncInput::selectFromHistory(size_t index)
{
	assert(index < m_history.size());
	moveCursor(int(m_inputRight.size()));
	erase(m_inputLeft.length() + m_inputRight.length());

	m_currentWordIndex = index;

	m_inputLeft = m_history[m_currentWordIndex];
	m_inputRight.clear();
	write(m_inputLeft);
}

void AsyncInput::moveCursor(int direction)
{
	if (direction == 0) return;

	CONSOLE_SCREEN_BUFFER_INFO i;
	if (!GetConsoleScreenBufferInfo(m_out, &i))
		throw std::runtime_error("GetConsoleScreenBufferInfo");

	COORD pos = i.dwCursorPosition;

	if (direction < 0) // move in negative direction
	{
		direction *= -1;
		pos.X -= direction;
		while (pos.X < 0)
		{
			pos.X += i.dwSize.X;
			pos.Y -= 1;
		}
	}
	else // positive direction
	{
		pos.X += direction;
		while (pos.X >= i.dwSize.X)
		{
			pos.X -= i.dwSize.X;
			pos.Y += 1;
		}
	}

	if (!SetConsoleCursorPosition(m_out, pos))
		throw std::runtime_error("SetConsoleCursorPosition");
}

void AsyncInput::refreshWordSuggestions()
{
	m_wordSuggestions.clear();
	if (!m_autoCompleteFunc) return;

	m_wordSuggestions = m_autoCompleteFunc(m_inputLeft);
	m_lastSuggestion = m_wordSuggestions.end();
}

WORD AsyncInput::getTextColor(char c)
{
	const auto DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	const auto STRING_COLOR = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
	const auto DIGIT_COLOR = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE;

	auto prevDigit = m_prevCharDigit;
	m_prevCharDigit = false;

	if (c == '\n')
	{
		m_isString = false;
	}

	if(c == '"' || c == '\'')
	{
		m_isString = !m_isString;
		return STRING_COLOR;
	}

	if (m_isString) return STRING_COLOR;

	switch(c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		m_prevCharDigit = true;
		return DIGIT_COLOR;
	case '.':
		if (prevDigit) return DIGIT_COLOR;
		break;
			
	}

	m_prevCharDigit = false;
	return DEFAULT_COLOR;
}
