#pragma once
#include <glad/glad.h>
#include "glmmath.h"

// helper to convert some types

namespace gltype
{
	// if Type is equal to ErrorType it could not be translated
	constexpr GLenum ErrorType = 0;
	template<typename T> constexpr GLenum Type = ErrorType;
	template<> constexpr GLenum Type<GLfloat> = GL_FLOAT;
	template<> constexpr GLenum Type<GLint> = GL_INT;
	template<> constexpr GLenum Type<GLuint> = GL_UNSIGNED_INT;
	template<> constexpr GLenum Type<GLshort> = GL_SHORT;
	template<> constexpr GLenum Type<GLushort> = GL_UNSIGNED_SHORT;
	template<> constexpr GLenum Type<GLbyte> = GL_BYTE;
	template<> constexpr GLenum Type<GLubyte> = GL_UNSIGNED_BYTE;
	template<typename T> constexpr GLenum Type<glm::tvec1<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tvec2<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tvec3<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tvec4<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat2x2<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat2x3<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat2x4<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat3x2<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat3x3<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat3x4<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat4x2<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat4x3<T>> = Type<T>;
	template<typename T> constexpr GLenum Type<glm::tmat4x4<T>> = Type<T>;

	template<typename T> constexpr GLsizei NumComponents = 0;
	template<> constexpr GLsizei NumComponents<GLfloat> = 1;
	template<> constexpr GLsizei NumComponents<GLint> = 1;
	template<> constexpr GLsizei NumComponents<GLuint> = 1;
	template<> constexpr GLsizei NumComponents<GLshort> = 1;
	template<> constexpr GLsizei NumComponents<GLushort> = 1;
	template<> constexpr GLsizei NumComponents<GLbyte> = 1;
	template<> constexpr GLsizei NumComponents<GLubyte> = 1;
	template<typename T> constexpr GLsizei NumComponents<glm::tvec1<T>> = 1;
	template<typename T> constexpr GLsizei NumComponents<glm::tvec2<T>> = 2;
	template<typename T> constexpr GLsizei NumComponents<glm::tvec3<T>> = 3;
	template<typename T> constexpr GLsizei NumComponents<glm::tvec4<T>> = 4;
	template<typename T> constexpr GLenum NumComponents<glm::tmat2x2<T>> = 2 * 2;
	template<typename T> constexpr GLenum NumComponents<glm::tmat2x3<T>> = 2 * 3;
	template<typename T> constexpr GLenum NumComponents<glm::tmat2x4<T>> = 2 * 4;
	template<typename T> constexpr GLenum NumComponents<glm::tmat3x2<T>> = 3 * 2;
	template<typename T> constexpr GLenum NumComponents<glm::tmat3x3<T>> = 3 * 3;
	template<typename T> constexpr GLenum NumComponents<glm::tmat3x4<T>> = 3 * 4;
	template<typename T> constexpr GLenum NumComponents<glm::tmat4x2<T>> = 4 * 2;
	template<typename T> constexpr GLenum NumComponents<glm::tmat4x3<T>> = 4 * 3;
	template<typename T> constexpr GLenum NumComponents<glm::tmat4x4<T>> = 4 * 4;
}
