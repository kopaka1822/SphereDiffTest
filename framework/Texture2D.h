#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>
#include "glmmath.h"

class Texture2D
{
public:
	Texture2D() = default;
	~Texture2D();
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D(Texture2D&& m) noexcept
	{
		swap(m);
	}
	Texture2D& operator=(Texture2D&& m) noexcept
	{
		swap(m);
		return *this;
	}
	void swap(Texture2D& o) noexcept
	{
		std::swap(m_id, o.m_id);
		std::swap(m_data, o.m_data);
		std::swap(m_width, o.m_width);
		std::swap(m_height, o.m_height);
	}
	
	/// \brief resizes the image on CPU
	/// \param width width in pixles
	/// \param height height in pixles
	void resize(GLsizei width, GLsizei height);

	/// \brief fills the image with a constant value on CPU
	/// \param value pixel color [0,1]
	void fill(const glm::vec4& value);

	/// \brief sets a pixel on CPU
	/// \param x coordinate
	/// \param y coordinate
	/// \param color pixel color [0,1]
	void putPixel(GLsizei x, GLsizei y, const glm::vec4& color);
	
	/// \brief loads a texture from a file
	/// \param filename source file
	void loadFromFile(const std::string& filename);
	
	/// \brief creates a texture on gpu and uploads the data
	void uploadToGpu();

	/// \brief binds a texture on the gpu
	/// \param slot binding slot. Most gpus support slot 0 - 7. 0 - 31 are supported by the opengl API
	void bind(GLuint slot) const;

	/// \brief sets the magnify filter
	/// \param filter GL_LINEAR GL_NEAREST or a mipmap filter e.g. GL_LINEAR_MIPMAP_LINEAR
	void setMinifyFilter(GLenum filter);

	// \brief sets the magnify filter
	// \param filter GL_LINEAR or GL_NEAREST
	void setMagnifyFilter(GLenum filter);
private:
	void dispose();

private:
	GLuint m_id = 0;
	std::vector<uint32_t> m_data;
	GLsizei m_width = 0;
	GLsizei m_height = 0;
};
