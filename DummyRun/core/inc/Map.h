#pragma once

#include "Sync.h"
#include <map>

namespace Utility
{
	template <class T1, class T2> 
	class Map : public Sync
	{
	public:
		typedef typename std::map<T1, T2>::iterator MAP_POSITION;

		Map();
		virtual ~Map();

		Map( Map<T1,T2>& rhs );
		Map<T1, T2>& operator=( Map<T1,T2>& rhs );

		bool IsEmpty();
		unsigned int Size();
		typename MAP_POSITION HeadPosition();
		typename MAP_POSITION EndPosition();
		typename MAP_POSITION Find( const T1 key );
		bool IsEnd( const MAP_POSITION pos );
		void Insert( const T1 key, const T2 data );
		void Update( const T1 key, const T2 data );
		T2& GetItem( const T1 key );
		T2& GetItem( const MAP_POSITION pos );
		void Remove( const T1 key );
		void RemoveAll();
		void Clear();

	private:
		std::map<T1, T2> m_mapElement;
	};
}

using namespace Utility;

template <class T1, class T2>
Map<T1, T2>::Map()
{
}

template <class T1, class T2>
Map<T1, T2>::~Map()
{
	RemoveAll();
}

template <class T1, class T2>
Map<T1, T2>::Map( Map<T1, T2>& rhs )
{
	Clear();

	m_mapElement.assign( rhs.HeadPosition(), rhs.EndPosition() );
}

template <class T1, class T2>
Map<T1, T2>& Map<T1, T2>::operator=( Map<T1, T2>& rhs )
{
	if( &rhs != this )
	{
		Clear();

		m_mapElement.assign( rhs.HeadPosition(), rhs.EndPosition() );
	}

	return *this;
}

template <class T1, class T2>
bool Map<T1, T2>::IsEmpty()
{
	return m_mapElement.empty();	
}

template <class T1, class T2>
unsigned int Map<T1, T2>::Size()
{
	return static_cast<unsigned int>( m_mapElement.size() );
}

template <class T1, class T2>
typename Map<T1, T2>::MAP_POSITION Map<T1, T2>::HeadPosition() 
{
	return m_mapElement.begin(); 
}

template <class T1, class T2>
typename Map<T1, T2>::MAP_POSITION Map<T1, T2>::EndPosition() 
{
	return m_mapElement.end(); 
}

template <class T1, class T2>
typename Map<T1, T2>::MAP_POSITION Map<T1, T2>::Find( const T1 key )
{
	return m_mapElement.find( key );
}

template <class T1, class T2>
bool Map<T1, T2>::IsEnd( const MAP_POSITION pos )
{
	if( pos == m_mapElement.end() )
		return true;

	return false;	
}

template <class T1, class T2>
void Map<T1, T2>::Insert( const T1 key, const T2 data )
{
	m_mapElement.insert( std::map<T1, T2>::value_type( key, data ) );
}

template <class T1, class T2>
void Map<T1, T2>::Update( const T1 key, const T2 data )
{
	m_mapElement[key] = data;
}

template <class T1, class T2>
T2& Map<T1, T2>::GetItem( const T1 key )
{
	return m_mapElement[key];	
}

template <class T1, class T2>
T2& Map<T1, T2>::GetItem( const MAP_POSITION pos )
{
	return pos->second;
}

template <class T1, class T2>
void Map<T1, T2>::Remove( const T1 key )
{
	m_mapElement.erase( key );
}

template <class T1, class T2>
void Map<T1, T2>::RemoveAll()
{
	m_mapElement.erase( m_mapElement.begin(), m_mapElement.end() );	
}

template <class T1, class T2>
void Map<T1, T2>::Clear()
{
	m_mapElement.clear();
}

