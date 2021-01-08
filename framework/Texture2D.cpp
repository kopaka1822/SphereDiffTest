#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stb_image.h"
#include "error.h"

uint32_t convertToBits(const glm::vec4& c)
{
	uint8_t br = uint8_t(255.0f * glm::clamp(c.r, 0.0f, 1.0f));
	uint8_t bg = uint8_t(255.0f * glm::clamp(c.g, 0.0f, 1.0f));
	uint8_t bb = uint8_t(255.0f * glm::clamp(c.b, 0.0f, 1.0f));
	uint8_t ba = uint8_t(255.0f * glm::clamp(c.a, 0.0f, 1.0f));
	return br | bg << 8 | bb << 16 | ba << 24;
}

Texture2D::~Texture2D()
{
	dispose();
}

void Texture2D::resize(GLsizei width, GLsizei height)
{
	dassert(width > 0);
	dassert(height > 0);
	m_data.resize(width * height);
	m_width = width;
	m_height = height;
}

void Texture2D::fill(const glm::vec4& value)
{
	std::fill(m_data.begin(), m_data.end(), convertToBits(value));
}

void Texture2D::putPixel(GLsizei x, GLsizei y, const glm::vec4& color)
{
	dassert(x < m_width);
	dassert(y < m_height);
	dassert(x >= 0);
	dassert(y >= 0);
	m_data[y * m_width + x] = convertToBits(color);
}

void Texture2D::dispose()
{
	if(m_id)
	{
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
}

void Texture2D::loadFromFile(const std::string& filename)
{
	int nChannels;
	int width;
	int height;
	// load image data with stb_image
	const auto data = stbi_load(filename.c_str(), &width, &height, &nChannels, 4);
	if (!data)
		throw std::runtime_error("could not load image " + filename);

	m_width = width;
	m_height = height;
	m_data.reserve(m_width * m_height);

	// copy data
	memcpy(m_data.data(), data, m_width * m_height * sizeof(uint32_t));

	stbi_image_free(data);
}

void Texture2D::uploadToGpu()
{
	// delete old texture
	dispose();

	dassert(m_width > 0);
	dassert(m_height > 0);

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glDebugError("Texture2D::uploadToGpu");
}

void Texture2D::bind(GLuint slot) const
{
	// was the texture data uploaded to the gpu?
	dassert(m_id);
	dassert(slot < 32);

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glDebugError("Texture2D::bind");
}

void Texture2D::setMinifyFilter(GLenum filter)
{
	// filter can only be set after texture generation
	dassert(m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

	glDebugError("Texture2D::setMinifyFilter");
}

void Texture2D::setMagnifyFilter(GLenum filter)
{
	// filter can only be set after texture generation
	dassert(m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glDebugError("Texture2D::setMinifyFilter");
}
