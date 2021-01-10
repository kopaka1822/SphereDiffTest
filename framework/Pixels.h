#pragma once
#include <vector>

template<typename T>
class Pixels
{
public:
	Pixels(int width, int height)
		: m_width(width), m_height(height)
	{
		assert(width > 0);
		assert(height > 0);

		m_pixels.resize(width * height);
	}
	
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	const T* data() const { return m_pixels.data(); }

	void clear()
	{
		memset(m_pixels.data(), 0, m_pixels.size() * sizeof(T));
	}
private:
	std::vector<T> m_pixels;
	int m_width;
	int m_height;
};
