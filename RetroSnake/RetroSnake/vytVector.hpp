#ifndef VYT_VECTOR_HPP
#define VYT_VECTOR_HPP

#include <vector>
#include <stdexcept>
#include <functional>

namespace vyt
{
	template <typename T>
	class vector
	{
		std::vector<T*> m_vector;
	public:
		using value_type = T;
		vector() { }
		vector(const vector &rhs) = delete;
		vector& operator=(const vector &rhs) = delete;
		vector(vector &&rhs)
		{
			m_vector = std::move(rhs.m_vector);
		}
		vector& operator=(vector &&rhs)
		{
			if (&rhs == this) return this;
			m_vector = std::move(rhs.m_vector);
			return this;
		}
		~vector()
		{
			for (auto item : m_vector)
				delete item;
		}

		#pragma region Append & AppendRange

		template <typename TItem>
		void Append()
		{
			m_vector.push_back(new TItem());
		}
		template <typename TItem, typename TArg1>
		void Append(TArg1 &arg1)
		{
			m_vector.push_back(new TItem(arg1));
		}
		template <typename TItem, typename TArg1>
		void Append(const TArg1 &arg1)
		{
			m_vector.push_back(new TItem(arg1));
		}
		template <typename TItem, typename TArg1, typename TArg2>
		void Append(TArg1 &arg1, TArg2 &arg2)
		{
			m_vector.push_back(new TItem(arg1, arg2));
		}
		template <typename TItem, typename TArg1, typename TArg2>
		void Append(const TArg1 &arg1, const TArg2 &arg2)
		{
			m_vector.push_back(new TItem(arg1, arg2));
		}
		template <typename TItem, typename TArg1, typename TArg2, typename TArg3>
		void Append(TArg1 &arg1, TArg2 &arg2, TArg3 &arg3)
		{
			m_vector.push_back(new TItem(arg1, arg2, arg3));
		}
		template <typename TItem, typename TArg1, typename TArg2, typename TArg3>
		void Append(const TArg1 &arg1, const TArg2 &arg2, const TArg3 &arg3)
		{
			m_vector.push_back(new TItem(arg1, arg2, arg3));
		}
		void AppendRange(size_t TCount, std::function<T*(size_t)> generator)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(generator(i));
		}
		template <typename TItem>
		void AppendRange(size_t TCount)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(new TItem());
		}
		template <typename TItem, typename TArg1>
		void AppendRange(TArg1 &arg1, size_t TCount)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(new TItem(arg1));
		}
		template <typename TItem, typename TArg1>
		void AppendRange(const TArg1 &arg1, size_t TCount)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(new TItem(arg1));
		}
		template <typename TItem, typename TArg1, typename TArg2>
		void AppendRange(TArg1 &arg1, TArg2 &arg2, size_t TCount)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(new TItem(arg1, arg2));
		}
		template <typename TItem, typename TArg1, typename TArg2>
		void AppendRange(const TArg1 &arg1, const TArg2 &arg2, size_t TCount)
		{
			for (size_t i = 0; i < TCount; ++i)
				m_vector.push_back(new TItem(arg1, arg2));
		}
	class copyable { public: virtual T* copy() const = NULL; };
	/*
	Depend on class CopyAble.
	Function prototype: virtual T* copy() const = NULL;
	Per item will implement this method
	*/
	void AppendRange(const vector& rhs)
	{
		for (size_t i = 0; i < rhs.size(); ++i)
			m_vector.push_back(rhs[i].copy());
	}
	void AppendRange(const vector& rhs, std::function<T*(const T&)> copy)
	{
		for (size_t i = 0; i < rhs.size(); ++i)
			m_vector.push_back(copy(rhs[i]));
	}

#pragma endregion

		#pragma region Delete & Clear

	void Delete(size_t index)
	{
		if (size() <= index)
			throw std::out_of_range("Delete index out of range");
		m_vector.erase(m_vector.begin() + index);
	}

	size_t Delete(const T &item)
	{
		size_t deleteCount = 0;
		for (auto iter = m_vector.begin(); iter != m_vector.end();)
		{
			if (*(*iter) == item)
			{
				T *obj = *iter;
				iter = m_vector.erase(iter);
				delete obj;
				++deleteCount;
			}
			else
				++iter;
		}
		return deleteCount;
	}

	bool DeleteByReference(T *item)
	{
		for (auto iter = m_vector.begin(); iter != m_vector.end(); ++iter)
		{
			if (*iter != item) continue;
			delete *iter;
			iter = m_vector.erase(iter);
			return true;
		}
		return false;
	}

	void Clear()
	{
		for (auto &t : m_vector)
			delete t;
		m_vector.erase(m_vector.begin(), m_vector.end());
	}

#pragma endregion

		#pragma region Index & Size & Iterator

	class iterator : public std::iterator<std::input_iterator_tag, T>
	{
		vector &m_vector;
		size_t m_index;
	public:
		iterator(vector& vector, size_t index) : m_vector(vector), m_index(index) { }
		iterator& operator=(const iterator &iter) { m_index = iter.m_index; }
		bool operator!=(const iterator &iter) const { return m_index != iter.m_index; }
		bool operator==(const iterator &iter) const { return m_index == iter.m_index; }
		bool operator<(const iterator &iter) const { return m_index < iter.m_index; }
		bool operator>(const iterator &iter) const { return m_index > iter.m_index; }
		bool operator<=(const iterator &iter) const { return m_index <= iter.m_index; }
		bool operator>=(const iterator &iter) const { return m_index >= iter.m_index; }
		iterator& operator++() { ++m_index; return *this; }
		iterator& operator++(int) { ++m_index; return iterator(m_vector, m_index - 1); }
		iterator& operator--() { --m_index; return *this; }
		iterator& operator--(int) { --m_index; return iterator(m_vector, m_index + 1); }
		T& operator*() { return m_vector[m_index]; }
	};

	iterator begin() { return iterator(*this, 0); }
	iterator end() { return iterator(*this, size()); }
	T& operator[](size_t index)
	{
		if (index >= m_vector.size())
			throw std::out_of_range("index out of size");
		return *m_vector[index];
	}
	const T& operator[](size_t index) const
	{
		if (index >= m_vector.size())
			throw std::out_of_range("index out of size");
		return *m_vector[index];
	}
	size_t size() const { return m_vector.size(); }

#pragma endregion

		#pragma region Search & IndexOf

	bool Contains(const T &item)
	{
		for (auto &t : m_vector)
			if (t == item)
				return true;
		return false;
	}

	bool IndexOf(const T &item, size_t &i)
	{
		for (i = 0; i < size(); ++i)
			if (m_vector[i] == item)
				return true;
		return false;
	}

#pragma endregion
	};
}

#endif