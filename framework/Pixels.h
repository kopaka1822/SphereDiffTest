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
	Pixels() : m_width(0), m_height(0) {}
	Pixels(const Pixels&) = delete;
	Pixels& operator=(const Pixels&) = delete;
	Pixels(Pixels&& o) noexcept  : Pixels()
	{
		swap(o);
	}
	Pixels& operator=(Pixels&& o) noexcept
	{
		swap(o);
		return *this;
	}
	void swap(Pixels& o) noexcept
	{
		std::swap(o.m_width, m_width);
		std::swap(o.m_height, m_height);
		std::swap(o.m_pixels, m_pixels);
	}
	
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	const T* data() const { return m_pixels.data(); }
	T* begin() { return m_pixels.data(); }
	T* end() { return m_pixels.data() + m_pixels.size(); }
	const T* begin() const { return m_pixels.data(); }
	const T* end() const { return m_pixels.data() + m_pixels.size(); }

	T get(int x, int y) const
	{
		if (m_width == 0 || m_height == 0) return T(0);
		x = glm::clamp(x, 0, m_width - 1);
		y = glm::clamp(y, 0, m_height - 1);
		return m_pixels[y * m_width + x];
	}
	
	void clear()
	{
		memset(m_pixels.data(), 0, m_pixels.size() * sizeof(T));
	}
private:
	std::vector<T> m_pixels;
	int m_width;
	int m_height;
};
