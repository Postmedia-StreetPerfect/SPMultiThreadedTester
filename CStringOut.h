#pragma once
#include <string>
class CStringOut
{
public:

	CStringOut(int buf_size, const char* src = nullptr);
	~CStringOut();

	char* reset(const char* src = nullptr);
	void copy(const char* src);
	operator char* () { return m_buf; }
	char * buf() { return m_buf; }

	char* cstr() { return trim_end(m_buf); }
	int size() { return m_buf_size; }
	std::string str() { return std::string(cstr()); }

private:
	int m_buf_size;
	char *m_buf;
	char* trim_end(char* str);
};

