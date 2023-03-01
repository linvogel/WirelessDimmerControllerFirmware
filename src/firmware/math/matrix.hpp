#pragma once

#include <functional>
#include <cstdlib>
#include <memory>

// although this does not quite fit here: since this is the main math file of the firmware
const double PI = 3.14159265358979323846;
const double PI2 = 2*PI;

namespace dim {
	namespace math {
		
		template<typename T, size_t W, size_t H>
		class matrix {
		private:
			uint8_t m_data[W*H * sizeof(T)];
			std::function<size_t(size_t, size_t)> idx;
		public:
			matrix() : idx([](size_t i, size_t j) { return j*W + i; }) {}
			matrix(std::initializer_list<T> l) : matrix() {
				if (l.size() != W*H) {
					throw "matrix error: invalid initializer list";
				}
				// copy all elements from the initializer list to the matrix data
				size_t i = 0;
				for (auto it = l.begin(); it != l.end(); it++) ((T*)(this->m_data))[i++] = *it;
			}
			
			size_t cols() const { return W; }
			size_t rows() const { return H; }
			size_t size() const { return W*H; }
			T* get_data() const { return (T*)this->m_data; }
			
			static matrix identity()
			{
				matrix out;
				for (size_t i = 0; i < W; i++) {
					for (size_t j = 0; j < H; j++) {
						out(i,j) = (i == j) ? (T)1 : (T)0;
					}
				}
				return out;
			}
			
			T& operator()(size_t i, size_t j) const {
				// runtime checks to prevent segmentation faults
				if (i >= W || j >= H) {
					throw "matrix error: index out of bounds";
				}
				
				return ((T*)(this->m_data))[idx(i,j)];
			}
			
			T& operator()(size_t i) const {
				// runtime checks to prevent segmentation faults
				if (i >= W*H) {
					throw "matrix error: index out of bounds";
				}
				
				return ((T*)(this->m_data))[i];
			}
			
			T squared_length(const matrix<T,1,H> &v) {
				T out = 0;
				for (int i = 0; i < H; i++) out += v(i) * v(i);
				return out;
			}
			
			matrix<T,H,W> transpose() const {
				matrix<T,H,W> out;
				for (size_t i = 0; i < W; i++) {
					for (size_t j = 0; j < H; j++) {
						out(j,i) = this->operator()(i,j);
					}
				}
				return out;
			}
		};
		
		using matrix2f = matrix<float,2,2>;
		using matrix3f = matrix<float,3,3>;
		using matrix4f = matrix<float,4,4>;
		using vector2f = matrix<float,1,2>;
		using vector3f = matrix<float,1,3>;
		using vector4f = matrix<float,1,4>;
		
		template<typename T, size_t W, size_t H>
		matrix<T, W, H> operator+(const matrix<T, W, H> &a, const matrix<T, W, H> &b)
		{
			matrix<T,W,H> out;
			for (size_t i = 0; i < a.size(); i++) out(i) = a(i) + b(i);
			return out;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T, W, H>& operator+=(matrix<T, W, H> &a, const matrix<T, W, H> &b)
		{
			for (size_t i = 0; i < a.size(); i++) a(i) += b(i);
			return a;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T, W, H> operator-(const matrix<T, W, H> &a, const matrix<T, W, H> &b)
		{
			matrix<T,W,H> out;
			for (size_t i = 0; i < a.size(); i++) out(i) = a(i) - b(i);
			return out;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T, W, H>& operator-=(matrix<T, W, H> &a, const matrix<T, W, H> &b)
		{
			for (size_t i = 0; i < a.size(); i++) a(i) -= b(i);
			return a;
		}
		
		template<typename T, size_t W, size_t M, size_t H>
		matrix<T, W, H> operator*(const matrix<T,M,H> &a, const matrix<T,W,M> &b)
		{
			matrix<T,W,H> out;
			for (size_t i = 0; i < W; i++) {
				for (size_t j = 0; j < H; j++) {
					out(i,j) = 0;
					for (size_t k = 0; k < M; k++) {
						out(i,j) += a(k,j) * b(i,k);
					}
				}
			}
			return out;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T,W,H> operator*(const matrix<T,W,H> &a, const T f)
		{
			matrix<T,W,H> out;
			for (size_t i = 0; i < a.size(); i++) out(i) = a(i) * f;
			return out;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T,W,H>& operator*=(matrix<T,W,H> &a, const matrix<T,W,W> &b)
		{
			T* tmp = (T*)alloca(sizeof(T) * W);
			for (size_t j = 0; j < H; j++) {
				for (size_t i = 0; i < W; i++) {
					tmp[i] = 0;
					for (size_t k = 0; k < W; k++) {
						tmp[i] += a(k,j) * b(i,k);
					}
				}
				for (int i = 0; i < W; i++) a(i,j) = tmp[i];
			}
			return a;
		}
		
		template<typename T, typename F, size_t W, size_t H>
		matrix<T,W,H>& operator*=(matrix<T,W,H> &a, const F f)
		{
			for (size_t i = 0; i < a.size(); i++) a(i) *= f;
			return a;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T,W,H> operator/(const matrix<T,W,H> &a, const T f)
		{
			matrix<T,W,H> out;
			for (size_t i = 0; i < a.size(); i++) out(i) = a(i) / f;
			return out;
		}
		
		template<typename T, size_t W, size_t H>
		matrix<T,W,H>& operator/=(const matrix<T,W,H> &a, const T f)
		{
			for (size_t i = 0; i < a.size(); i++) a(i) /= f;
			return a;
		}
	}
}

#ifndef _DIM_DISABLE_GLOBAL_MATRICES
using matrix2f = dim::math::matrix<float,2,2>;
using matrix3f = dim::math::matrix<float,3,3>;
using matrix4f = dim::math::matrix<float,4,4>;
using vector2f = dim::math::matrix<float,1,2>;
using vector3f = dim::math::matrix<float,1,3>;
using vector4f = dim::math::matrix<float,1,4>;
#endif