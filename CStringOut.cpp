#include "CStringOut.h"


CStringOut::CStringOut(int buf_size, const char* src)
{
	m_buf_size = buf_size + 1; // +1 for null terminator
	m_buf = new char[m_buf_size];

	memset(m_buf, ' ', m_buf_size);
	m_buf[m_buf_size - 1] = '\0';

	if (src) {
		copy(src);
	}
}

CStringOut::~CStringOut()
{
	if (m_buf) {
		delete[] m_buf;
		m_buf = nullptr;
	}
}

char* CStringOut::reset(const char* src)
{
	memset(m_buf, ' ', m_buf_size);
	m_buf[m_buf_size - 1] = '\0';
	if (src) {
		copy(src);
	}
	return m_buf;
}

void CStringOut::copy(const char* src)
{
	strncpy(m_buf, src, m_buf_size - 1);
	m_buf[m_buf_size - 1] = '\0';
}

// private
char* CStringOut::trim_end(char* str)
{
	if (str == nullptr) {
		return nullptr;
	}
	char* end = str + strlen(str) - 1;
	while (end >= str && isspace((unsigned char)*end)) {
		*end = '\0';
		end--;
	}
	return str;
}


